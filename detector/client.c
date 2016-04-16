#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

enum msg_type {REPORT, ALARM};
struct message
{
	long id;
	enum msg_type type;
	int value;
	int value2;
};
int main(int argc, char *argv[])
{
	if(argc<3)
	{
		printf("Usage:\n ./MANAGER <DETECTOR_IP> <DETECTOR_PORT>\n");
		return 0;
	}
/* Variables */
	int pressed;

	int sock;
	struct sockaddr_in server;
	struct hostent *hp;
	struct message msg1 = {15,REPORT,35,0};
	struct message *msg;
	msg = malloc(sizeof(struct message));
/* Create socket*/
	printf("Client started\n");
	sock=socket(AF_INET, SOCK_STREAM, 0);
	if(sock<0)
	{
		perror("Failed to create socket");
		close(sock);
		exit(1);
	}
	printf("Socket created.\n");
	server.sin_family = AF_INET;
	hp= gethostbyname (argv[1]);
	if(hp == 0)
	{
		perror("GetHostName failed");
		close(sock);
		exit(1);
	}
	printf("Hostname received.\n");

	memcpy(&server.sin_addr, hp->h_addr, hp->h_length);
	server.sin_port = atoi(argv[2]);


	if(connect(sock, (struct sockaddr*) &server, sizeof (server)) < 0)
	{
		perror("Connect failed");
		close(sock);
		exit(1);
	}
	printf("Successfully connected to: %s\n",hp->h_name);
	int end = 0;
	int rval;
	while(end != 1)
	{
		if((rval=recv(sock, msg, sizeof(struct message), 0)) <0)
			perror("reading stream message error");
		else if(rval==0)
		{
			printf("Ending connection\n");
			end=1;
		}
		else
			switch(msg->type)
			{
				case REPORT:
					printf("Detector %ld reported: waterlevel: %d/%d\n", msg->id, msg->value, msg->value2);
				break;
				case ALARM:
					printf("Detector %ld ALARMED: waterlevel: %d/%d\n", msg->id, msg->value, msg->value2);
				break;
				deafult:
					printf("Unknown message from detector %ld\n", msg->id);
				break;
			}
	}
	close(sock);
	free(msg);
	return 0;
}
