//
// Created by Paweł Joński
// Modified by Michał Sacharczuk

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <limits>
#include <thread>
#include "../common/messageType.h"
#include "../common/message.h"
#include "../common/Constants.h"

#define CHECK_STATUS_INTERVAL 1
#define IpV6 0
// global variables - detector specific
int detector_id;
int typicalResistance;
int currentResistance;

int detector_send(int sock, int type)
{


	struct message *msg;
	int err;
	msg =(struct message*)  malloc(sizeof(struct message));
	msg->id = detector_id;
	msg->msg_type = type;
	msg->currentResistance = currentResistance;
	msg->typicalResistance = typicalResistance;
	err = send(sock, msg, sizeof(struct message), MSG_NOSIGNAL);
	if( err < 0) {
		perror("Sending raport failed");
		return -1;
	}
	const char* name = type == ALARM ? "Alarm" : "Raport";

	printf("%s Sent: id: %ld, type: %d, current resistance: %d, typical eesistance: %d\n", name,
	msg->id, msg->msg_type, msg->currentResistance, msg->typicalResistance);

    int rval = recv(sock, msg, sizeof(struct message), 0);
    if (rval < 0 ) {
        perror("reading stream message error");
    } else if (rval==0) {
        printf("Ending connection\n");
    }

    if(msg->msg_type == CHANGE_TYPICAL_RESISTANCE) {
        typicalResistance = msg->typicalResistance;
    }

    printf("Received: id: %ld, type: %d, current resistance: %d, typical resistance: %d\n",
           msg->id, msg->msg_type, msg->currentResistance, msg->typicalResistance);


    free(msg);
	return 0;
}

int sock;
int mysock;


int createSockets() {
    struct sockaddr_in server;
    struct sockaddr_in6 server6;

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
        server.sin_port = eRuraPortNumber;
        /* CALL BIND */
        if(bind(sock, (struct sockaddr*) &server, sizeof(server))) {
            perror("bind failed");
            exit(1);
        }
    }
    else {
        server6.sin6_family = AF_INET6;
        server6.sin6_addr = in6addr_any;
        server6.sin6_port = eRuraPortNumber;
        /* CALL BIND  server6 = ipv6 */
        if(bind(sock, (struct sockaddr*) &server6, sizeof(server6))) {
            perror("bind failed");
            exit(1);
        }
    }
}

void mainLoop() {
/* LISTEN */
    listen(sock,5);
/* ACCEPT */
    while(1) {
		int end=0;

		printf("waiting for connection from manager...\n");
		mysock = accept(sock, (struct sockaddr *) 0, 0);
		/* REPORT AND ALARM UNTIL MANAGER DISCONNECTS */
		do {
			if(mysock < 1)
				perror("accept failed");
			else {
                int type = currentResistance < typicalResistance ? ALARM : REPORT;
                if(currentResistance > 999999)
                    type = INFINITY_RESISTANCE;

                if (detector_send(mysock, type) == -1) {
                    end = 1;
                    break;
                }

				sleep(CHECK_STATUS_INTERVAL);
			}
		} while(!end);
	}
}

int main(int argc, char *argv[])
{
	if(argc<2)
	{
		printf("Usage:\n ./DETECTOR <DETECTOR_ID>\n");
		return 0;
	}

	srand(time(NULL));
	detector_id = atoi (argv[1]);

    typicalResistance = defaultTypicalResistance;
    currentResistance = defaultTypicalResistance;

    createSockets();

    std::thread loop(mainLoop);

    int i = 0;

    while(1) {
        std::cin>>i;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        switch (i) {
            case 0:
                currentResistance = typicalResistance;
                break;
            default:
                currentResistance = i;
        }
    }


    close(sock);

	return 0;
}
