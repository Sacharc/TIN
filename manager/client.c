#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include "../common/messageType.h"
#include "../common/message.h"

#define DET_NMB 3

int main(int argc, char *argv[])
{
	if(argc<3)
	{
		printf("Usage:\n ./MANAGER <DETECTOR_IP> <DETECTOR_PORT>\n");
		return 0;
	}
/* Variables */
	int pressed;

	fd_set readfds;
	FD_ZERO (&readfds);
	int sock[2];
	struct sockaddr_in server;
	struct hostent *hp;
	struct message msg1 = {15,REPORT,35,0};
	struct message *msg;
	msg = malloc(sizeof(struct message));

	struct sockaddr_in servers [2];

/* Create socket*/
	int i=0;
	for(i=0; i<DET_NMB; i++)
	{
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
		hp= gethostbyname (argv[1+i*2]);
		if(hp == 0)
		{
			perror("GetHostName failed");
			close(sock[i]);
			exit(1);
		}
		printf("Hostname received.\n");

		memcpy(&servers[i].sin_addr, hp->h_addr, hp->h_length);
		servers[i].sin_port = atoi(argv[2+i*2]);


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
		int j=0;
		for(j=0; j< DET_NMB; j++)
		{
			FD_SET(sock[j],&readfds);
		}
		i = 0;
		select(FD_SETSIZE, &readfds,NULL,NULL,NULL);
		for (i=0; i<DET_NMB; i++)
		{
			if(FD_ISSET(sock[i],&readfds))
			{
				if((rval=recv(sock[i], msg, sizeof(struct message), 0)) <0)
					perror("reading stream message error");
				else if(rval==0)
				{
					printf("Ending connection\n");
					end=1;
				}
				else
					switch(msg->msg_type)
					{
						case REPORT:
							printf("Detector %ld reported: waterlevel: %d/%d\n", msg->id, msg->currentResistance, msg->typicalResistance);
						break;
						case ALARM:
							printf("Detector %ld ALARMED: waterlevel: %d/%d\n", msg->id, msg->currentResistance, msg->typicalResistance);
						break;
						deafult:
							printf("Unknown message from manager %ld\n", msg->id);
						break;
					}
			}
		}
	}
	close(sock[0]);
	close(sock[1]);
	free(msg);
	return 0;
}