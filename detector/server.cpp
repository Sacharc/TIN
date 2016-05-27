#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "../common/messageType.h"
#include "../common/message.h"
#define RAPORT_INTERVAL 5
#define CHECK_STATUS_INTERVAL 1

// global variables - detector specific
int detector_id;
int safe_level;
int water_level;

int detector_report(int sock)
{
	struct message *msg;
	int err;
	msg =(struct message*)  malloc(sizeof(struct message));
	msg->id = detector_id;
	msg->msg_type = REPORT;
	msg->currentResistance = water_level;
	msg->typicalResistance = safe_level;
	err = send(sock, msg, sizeof(struct message), MSG_NOSIGNAL);
		if( err < 0)
	{
		perror("Sending raport failed");
		return -1;
	}
	printf("Raport sent: id: %ld, type: %d, water level: %d, safe_level: %d\n",
	msg->id, msg->msg_type, msg->currentResistance, msg->typicalResistance);
	free(msg);
	return 0;
}
int detector_alarm(int sock)
{
	struct message *msg;
	int err;
	msg = (struct message*) malloc(sizeof(struct message));
	msg->id = detector_id;
	msg->msg_type = ALARM;
	msg->currentResistance = water_level;
	msg->typicalResistance = safe_level;
	err = send(sock, msg, sizeof(struct message), MSG_NOSIGNAL);
		if( err < 0)
	{
		perror("Sending raport failed");
		return -1;
	}
	printf("Alarm sent: id: %ld, type: %d, water level: %d, safe_level: %d\n",
	msg->id, msg->msg_type, msg->currentResistance, msg->typicalResistance);
	free(msg);
	return 0;
}

int main(int argc, char *argv[])
{
	if(argc<3)
	{
		printf("Usage:\n ./DETECTOR <DETECTOR_ID> <LISTEN_PORT>\n");
		return 0;
	}
/* Variables */
	int sock;
	struct sockaddr_in server;
	int mysock;

	srand(time(NULL));
	detector_id = atoi (argv[1]);
	water_level = 2000 + rand() % 2000;
	safe_level = 3000;

/* Create socket*/
	sock=socket(AF_INET, SOCK_STREAM, 0);
	if(sock<0)
	{
		perror("Failed to create socket");
		exit(1);
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = atoi(argv[2]);

/* CALL BIND */
	if(bind(sock, (struct sockaddr*) &server, sizeof(server)))
	{
		perror("bind failed");
		exit(1);
	}

/* LISTEN */
	listen(sock,5);
/* ACCEPT */
	while(1) {
		int end=0;
		int iter =0;
		printf("waiting for connection from manager...\n");
		mysock = accept(sock, (struct sockaddr *) 0, 0);
		/* REPORT AND ALARM UNTIL MANAGER DISCONNECTS */
		do {
			if(mysock < 1)
				perror("accept failed");
			else
			{
				water_level = 2000 + rand() % 2000;
				if(iter % RAPORT_INTERVAL == 0)
					if (detector_report(mysock) == -1)
					{
						end = 1;
						break;
					}
				if(water_level >= safe_level)
					if (detector_alarm(mysock) == -1)
					{
						end = 1;
						break;
					}
				sleep(CHECK_STATUS_INTERVAL);
			}
			iter++;
		} while(!end);
	}
	close(sock);

	return 0;
}
