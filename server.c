#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>



int main()
{

struct message
{
	long id;
	unsigned short msg_type;
};
/* Variables */

	int sock;
	struct sockaddr_in server;
	int mysock;
	int rval;
	struct message *msg;
	msg = malloc(sizeof(struct message));
	//memset(msg, 0, sizeof(struct message));
/* Create socket*/
	sock=socket(AF_INET, SOCK_STREAM, 0);
	if(sock<0)
	{
		perror("Failer to create socket");
		exit(1);
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port=5000;

/* CALL BIND */
	if(bind(sock, (struct sockaddr*) &server, sizeof(server)))
	{
		perror("bind failed");
		exit(1);
	}

/* LISTEN */
	listen(sock,5);
/* ACCEPT */
	while(1)
	{
	int end=0;
		mysock = accept(sock, (struct sockaddr *) 0, 0);
	do {
		if(mysock ==-1)
			perror("accept failed");
		else
		{
			if((rval=recv(mysock, msg, sizeof(struct message), 0)) <0)
				perror("reading stream message error");
			else if(rval==0)
			{
				printf("Ending connection\n");
				end=1;
			}
			else
				printf("MSG: %ld , %hd ,\n", msg->id, msg->msg_type);
			printf("Got the message (rval=%d)\n",rval);
		}


	} while(!end);
//	close(sock);
	}
	close(sock);	
	return 0;
}
