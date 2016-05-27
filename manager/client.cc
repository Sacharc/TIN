#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <thread>
#include <asm/ioctls.h>
#include "../common/messageType.h"
#include "../common/message.h"
#include "MessageHandler.h"
#include "../http_module/HttpHandler.h"


const char* eRuraSubnet = "192.168.1";
const int eRuraPort = 64999;

std::vector<std::string> scan () {
    std::vector<std::string> addrVec;
    for (int i = 1; i < 25; ++i) {
        char addr[16];
        snprintf(addr, 16, "%s.%d", eRuraSubnet, i);
        printf("Scanning: %s\n",addr);
        struct sockaddr_in address;  /* the libc network address data structure */
        short int sock = -1;         /* file descriptor for the network socket */
        fd_set readfds;
        struct timeval tv;

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = inet_addr(addr); /* assign the address */
        address.sin_port = htons(eRuraPort);            /* translate int2port num */

        sock = socket(AF_INET, SOCK_STREAM, 0);
        fcntl(sock, F_SETFL, O_NONBLOCK);

        connect(sock, (struct sockaddr *) &address, sizeof(address));

        FD_ZERO(&readfds);
        FD_SET(sock, &readfds);
        tv.tv_sec = 0;             /* 10 second timeout */
        tv.tv_usec = 50000;

        if (select(sock + 1, NULL, &readfds, NULL, &tv) == 1) {
            int so_error;
            socklen_t len = sizeof so_error;
            getsockopt(sock, SOL_SOCKET, SO_ERROR, &so_error, &len);
            if (so_error == 0) {
                std::string str(addr);
                addrVec.push_back(str);
                printf("Successfully scanned: %s\n",addr);
            } else {
                printf("Cant scann: %s\n", strerror(errno));
            }
        }
        close(sock);
    }
    return addrVec;
}



int main(int argc, char *argv[])
{
    std::string eRuraSubnet = "192.168.1";
/* Variables */
    fd_set readfds;
    FD_ZERO (&readfds);
    int sock[256];
    struct hostent *hp;
    struct message *msg;
    msg = (struct message*)malloc(sizeof(struct message));

    MessageHandler handler;

//    struct sockaddr_in servers [256];
/* Create http handler thread */
	std::thread httpHandler(httpHandlerStart, &handler);

/* Create socket*/
    int servcount = 0;

    auto addrVec = scan();
    struct sockaddr_in servers [255];
    /* Create socket*/
    for(unsigned i=0; i < addrVec.size(); i++) {

        printf("Client started\n");
        sock[i]=socket(AF_INET, SOCK_STREAM, 0);
        if(sock[i]<0)
        {
            perror("Failed to create socket");
            close(sock[i]);
            exit(1);
        }
        printf("Socket created.\n");
        servers[i].sin_family = AF_INET;
        hp= gethostbyname (addrVec[i].c_str());
        if(hp == 0) {
            perror("GetHostName failed");
            close(sock[i]);
            exit(1);
        }
        printf("Hostname received.\n");

        memcpy(&servers[i].sin_addr, hp->h_addr, hp->h_length);
        servers[i].sin_port = eRuraPort;


        if(connect(sock[i], (struct sockaddr*) &servers[i], sizeof (servers[i])) < 0)
        {
            perror("Connect failed");
            close(sock[i]);
            exit(1);
        }
        printf("Successfully connected to: %s\n",hp->h_name);
    }
    int end = 0;
    int rval;
    while(end != 1)
    {
        for(int i=0; i < servcount; i++)
        {
            FD_SET(sock[i], &readfds);
        }
        select(FD_SETSIZE, &readfds,NULL,NULL,NULL);
        for (int i=0; i<servcount; i++) {
            if(FD_ISSET(sock[i],&readfds))
            {
                if((rval=recv(sock[i], msg, sizeof(struct message), 0)) <0)
                    perror("reading stream message error");
                else if(rval==0)
                {
                    printf("Ending connection\n");
                    end=1;
                }
                else {
                    handler.handle(msg);
                }
            }
        }
    }
    for (int i=0; i < servcount; i++)
    {
        close(sock[i]);
    }
    free(msg);
    return 0;
}