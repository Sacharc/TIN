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


#define DET_NMB 3
#define MAX_SOCK 20
int main(int argc, char *argv[])
{
    if(argc<1)
    {
        printf("Usage:\n ./MANAGER <DETECTOR_PORT>\n");
        return 0;
    }
/* Variables */
    fd_set readfds;
    FD_ZERO (&readfds);
    int sock[MAX_SOCK];
    struct hostent *hp;
    struct message *msg;
    struct timeval times;
    times.tv_sec=0;
    times.tv_usec=10000;
    msg = (struct message*)malloc(sizeof(struct message));

    MessageHandler handler;

    struct sockaddr_in servers [20];
  	int ip[] = {1,0,168,192};
  	char buf[16];

/* Create http handler thread */
    std::thread httpHandler(httpHandlerStart, &handler);

    /* Create socket*/
    int servcount = 0;
    	int i=0;
    	for(i=1; i<255; i++)
    	{
    		fd_set fds_connect;
    		FD_ZERO (&fds_connect);
    		ip[0] = i;
    		//printf("Client started\n");
    		sock[servcount]=socket(AF_INET, SOCK_STREAM, 0);
    		fcntl(sock[servcount], F_SETFL, O_NONBLOCK);
    		if(sock[servcount]<0)
    		{
    			//perror("Failed to create socket.\n");
    			close(sock[servcount]);
    			continue;
    		}
    		//printf("Socket created.\n");
    		servers[servcount].sin_family = AF_INET;
    		snprintf(buf, 16, "%d.%d.%d.%d", ip[3],ip[2],ip[1],ip[0]);
    		hp= gethostbyname (buf);
    		if(hp == 0)
    		{
    			//perror("GetHostName failed. \n");
    			close(sock[servcount]);
    			continue;
    		}
    	//	printf("Hostname received.\n");

    		memcpy(&servers[servcount].sin_addr, hp->h_addr, hp->h_length);
    		servers[servcount].sin_port = 2137;
    		printf("Address: %s\n",inet_ntoa(servers[servcount].sin_addr));
    		int ret = connect(sock[servcount], (struct sockaddr*) &servers[servcount], sizeof (servers[servcount]));
    		fd_set fd;
    		FD_ZERO(&fd);
    		FD_SET(sock[servcount],&fd);
    		ret = select(FD_SETSIZE, NULL, &fd, NULL, &times);
    		times.tv_usec=10000;
    		if(ret <1)
    		{
    			printf("Connect failed!\n");
    			close(sock[servcount]);
    			continue;
    		}
    		printf("Successfully connected to: %s\n",hp->h_name);
    		servcount++;
    	}
    	int end = 0;
    	int rval;
    	int j=0;
    	for(j=0; j< servcount; j++)
    	{
    		FD_SET(sock[j],&readfds);
    	}
    	sleep(1);
    	while(end != 1)
    	{

    		i = 0;
    		select(FD_SETSIZE, &readfds,NULL,NULL,NULL);
    		for (i=0; i<servcount; i++)
    		{
    			if(sock[i]!=-1 && FD_ISSET(sock[i],&readfds))
    			{
    				if((rval=recv(sock[i], msg, sizeof(struct message), 0)) <0) {
    					perror("reading stream message error");
    					FD_CLR(sock[i],&readfds);
    					sock[i]=-1;
    				}
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
    	for (i=0;i<servcount;i++)
    	{
    		close(sock[i]);
    	}
    	free(msg);
    	return 0;
    }
