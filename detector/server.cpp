#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fstream>
#include "../common/messageType.h"
#include "../common/message.h"
#define RAPORT_INTERVAL 5
#define CHECK_STATUS_INTERVAL 1
#define IpV6 0
// global variables - detector specific
int detector_id;
int safe_level;
int water_level;

int detector_send(int sock, messageType type)
{
	struct message *msg;
	int err;
	msg =(struct message*)  malloc(sizeof(struct message));
	msg->id = detector_id;
	msg->msg_type = type;
	msg->currentResistance = water_level;
	msg->typicalResistance = safe_level;
	err = send(sock, msg, sizeof(struct message), MSG_NOSIGNAL);
		if( err < 0)
	{
		perror("Sending raport failed");
		return -1;
	}
	const char* name = type == ALARM ? "Alarm" : "Raport";

	printf("%s sent: id: %ld, type: %d, water level: %d, safe_level: %d\n", name,
	msg->id, msg->msg_type, msg->currentResistance, msg->typicalResistance);
	free(msg);
	return 0;
}

void loadTypicalResistance() {
    std::ifstream config("config.conf");
    config>>safe_level;
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
	struct sockaddr_in6 server6;
	int mysock;

	srand(time(NULL));
	detector_id = atoi (argv[1]);
	water_level = 2000 + rand() % 2000;
	safe_level = 3000;

    loadTypicalResistance();

/* Create socket*/
	if(IpV6 == 0)
		sock=socket(AF_INET, SOCK_STREAM, 0);
	else
		sock=socket(AF_INET6, SOCK_STREAM, 0);

	if(sock<0) {
		perror("Failed to create socket");
		exit(1);
	}

	if(IpV6 == 0) {
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = INADDR_ANY;
		server.sin_port = atoi(argv[2]);
        /* CALL BIND */
        if(bind(sock, (struct sockaddr*) &server, sizeof(server))) {
            perror("bind failed");
            exit(1);
        }
	}
	else {
		server6.sin6_family = AF_INET6;
		server6.sin6_addr = in6addr_any;
		server6.sin6_port = atoi(argv[2]);
        /* CALL BIND  server6 = ipv6 */
        if(bind(sock, (struct sockaddr*) &server6, sizeof(server6))) {
            perror("bind failed");
            exit(1);
        }
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
			else {
				water_level = 2000 + rand() % 2000;
				if(iter % RAPORT_INTERVAL == 0)
					if (detector_send(mysock, REPORT) == -1) {
						end = 1;
						break;
					}
				if(water_level >= safe_level)
					if (detector_send(mysock, ALARM) == -1) {
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
