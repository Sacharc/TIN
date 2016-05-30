//
// Created by Pawel Jonski.
// Modified by Michal Sacharczuk, Jakub Dudziak.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <fcntl.h>
#include <arpa/inet.h>
#include <mutex>
#include "../common/message.h"
#include "../common/Constants.h"
#include "MessageHandler.h"
#include "../http_module/HttpHandler.h"
#include <sstream>
#include <iterator>
#include <iomanip>
#define IpV6 0
// sockets
std::vector<int> sockets;

std::string changeFormatIPv6(int hexIP) {
    std::stringstream stream;
    stream << std::hex << hexIP;
    std::string stringIPv6 = stream.str();

    for(int i=stringIPv6.length(); i<4;i++)	{
        stringIPv6 = "0" + stringIPv6;
    }

    return  stringIPv6;
};

std::vector<int> findDetectors() {
    struct hostent *hp;
    struct timeval times;
    times.tv_sec=0;
    times.tv_usec=10000;

    std::vector<int> socks;

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

        long arg = fcntl(soc, F_GETFL, NULL);
        arg &= (~O_NONBLOCK);
        fcntl(soc, F_SETFL, arg);

        printf("Successfully connected to: %s\n",hp->h_name);
        socks.push_back(soc);
    }
    return socks;
}

std::vector<int> findDetectorsIPv6() {
    struct hostent *hp;
    struct timeval times;
    times.tv_sec=0;
    times.tv_usec=10000;

    std::vector<int> sockets;

    for (int i = 1; i < 65536; i++) {
        char buf[32];
        struct sockaddr_in6 serv;
        fd_set fds_connect;
        FD_ZERO (&fds_connect);
        //printf("Client started\n");
        int soc = socket(AF_INET6, SOCK_STREAM, 0);
        fcntl(soc, F_SETFL, O_NONBLOCK);
        if(soc<0) {
            close(soc);
            continue;
        }
        serv.sin6_family = AF_INET6;
        snprintf(buf, 39, "%s.%s", eRuraSubnetIpv6, changeFormatIPv6(i).c_str());
        hp = gethostbyname2 (buf, AF_INET6);
        if (hp == 0) {
            close(soc);
            continue;
        }
        memcpy(&serv.sin6_addr, hp->h_addr, hp->h_length);
        serv.sin6_port = eRuraPortNumber;
        printf("Address: %s\n",buf);
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
    }
    return sockets;
}

void closeSocktes() {
    for (unsigned i = 0; i < sockets.size(); i++) {
        close(sockets[i]);
    }
}

void checkAllDetectors(MessageHandler &handler, fd_set &readfds) {
    struct message *msg = (struct message*)malloc(sizeof(struct message));
    select(FD_SETSIZE, &readfds,NULL,NULL,NULL);
    for (unsigned i = 0; i < sockets.size(); i++) {
        if(sockets[i] != -1 && FD_ISSET(sockets[i], &readfds)) {
            msg =(struct message*)  malloc(sizeof(struct message));
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
    free(msg);
}

fd_set setFlags() {
    fd_set readfds;
    FD_ZERO (&readfds);
    for(unsigned i = 0; i < sockets.size(); i++) {
        FD_SET(sockets[i],&readfds);
    }
    return readfds;
}

void detectorListener(MessageHandler *handler, std::mutex* m, CommandLineInterface *cli) {
    fd_set readfds = setFlags();
    sleep(1);

    while(!cli->isEnd()) {
        m->lock();
        checkAllDetectors(*handler, readfds);
        m->unlock();
        sleep(1);
    }

    closeSocktes();
}

int main(int argc, char *argv[])
{
    MessageHandler handler;
    std::mutex m;
    CommandLineInterface cli(&handler.getHistory(), &m);

    /* Create http handler thread */
    std::thread httpHandler(httpHandlerStart, &handler, &cli);
    if(IpV6 == 0)
        sockets = findDetectors();
    else
        sockets = findDetectorsIPv6();

    std::thread loop(detectorListener, &handler, &m, &cli);

    cli.mainMenu();

    httpHandler.join();
    loop.join();

    return 0;
}
