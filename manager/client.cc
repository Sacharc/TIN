#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <sys/time.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../common/messageType.h"
#include "../common/message.h"
#include "MessageHandler.h"
#include "../http_module/HttpHandler.h"
#include "cli/CommandLineInterface.h"

const char* eRuraSubnet = "192.168.1";
const int eRuraPortNumber = 2137;

std::vector<int> findDetectors() {
    struct hostent *hp;
    struct timeval times;
    times.tv_sec=0;
    times.tv_usec=10000;

    std::vector<int> sockets;
    //  TODO wydaje mi sie ze nie potrzebne do przetestowania
    std::vector<struct sockaddr_in> servs;

    for (int i = 1; i < 255; i++) {
        char buf[16];
        struct sockaddr_in serv;
        fd_set fds_connect;
        FD_ZERO (&fds_connect);
        //printf("Client started\n");
        int soc = socket(AF_INET, SOCK_STREAM, 0);
        fcntl(soc, F_SETFL, O_NONBLOCK);
        if(soc<0) {
            close(soc);
            continue;
        }
        serv.sin_family = AF_INET;
        snprintf(buf, 16, "%s.%d", eRuraSubnet, i);
        hp = gethostbyname (buf);
        if (hp == 0) {
            close(soc);
            continue;
        }
        memcpy(&serv.sin_addr, hp->h_addr, hp->h_length);
        serv.sin_port = eRuraPortNumber;
        printf("Address: %s\n",inet_ntoa(serv.sin_addr));
        connect(soc, (struct sockaddr*) &serv, sizeof (serv));
        fd_set fd;
        FD_ZERO(&fd);
        FD_SET(soc,&fd);
        times.tv_usec=10000;
        if (select(FD_SETSIZE, NULL, &fd, NULL, &times) <1)
        {
            printf("Connect failed!\n");
            close(soc);
            continue;
        }
        printf("Successfully connected to: %s\n",hp->h_name);
        sockets.push_back(soc);
        servs.push_back(serv);
    }
    return sockets;
}

void closeSocktes(const std::vector<int> &sockets) {
    for (unsigned i = 0; i < sockets.size(); i++) {
        close(sockets[i]);
    }
}

void checkAllDetectors(MessageHandler &handler, std::vector<int> &sockets, fd_set &readfds) {
    struct message *msg = (struct message*)malloc(sizeof(struct message));
    select(FD_SETSIZE, &readfds,NULL,NULL,NULL);
    for (unsigned i = 0; i < sockets.size(); i++) {
        if(sockets[i] != -1 && FD_ISSET(sockets[i], &readfds)) {
            int rval = recv(sockets[i], msg, sizeof(struct message), 0);
            if (rval < 0 ) {
                perror("reading stream message error");
                FD_CLR(sockets[i],&readfds);
                sockets[i]=-1;
            } else if (rval==0) {
                printf("Ending connection\n");
            } else {
                handler.handle(msg);
            }
        }
    }
}

fd_set setFlags(const std::vector<int> &sockets) {
    fd_set readfds;
    FD_ZERO (&readfds);
    int end = 0;
    for(unsigned i = 0; i < sockets.size(); i++) {
        FD_SET(sockets[i],&readfds);
    }
    return readfds;
}


void detectorListener(MessageHandler *handler, std::vector<int> *sockets) {
    fd_set readfds = setFlags(*sockets);
    sleep(1);

    while(1)
        checkAllDetectors(*handler, *sockets, readfds);

    closeSocktes(*sockets);
}

int main(int argc, char *argv[])
{
    MessageHandler handler;

    /* Create http handler thread */
    std::thread httpHandler(httpHandlerStart, &handler);
    std::vector<int> sockets = findDetectors();

    std::thread loop(detectorListener, &handler, &sockets);

    CommandLineInterface cli(&handler.getHistory());
    cli.mainMenu();

    return 0;
}
