#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
	
struct message
{
	long id;
	unsigned short msg_type;
};
int main(int argc, char *argv[])
{
/* Variables */
	int pressed;
	
	int sock;
	struct sockaddr_in server;
	struct hostent *hp;
	struct message msg1 = {15,35};
/* Create socket*/
	sock=socket(AF_INET, SOCK_STREAM, 0);
	if(sock<0)
	{
		perror("Failer to create socket");
		close(sock);
		exit(1);
	}

	server.sin_family = AF_INET;
	hp= gethostbyname (argv[1]);
	if(hp == 0)
	{
		perror("GetHostName failed");
		close(sock);
		exit(1);
	}


	memcpy(&server.sin_addr, hp->h_addr, hp->h_length);
	server.sin_port=5000;


	if(connect(sock, (struct sockaddr*) &server, sizeof (server)) <0)
	{
		perror("Connect failed");
		close(sock);
		exit(1);
	}
	pressed=getchar();
	while(pressed!=27)
	{
		printf("Press a key.");
		if(send(sock, &msg1, sizeof(msg1), 0) <0)
		{
			perror("send failed");
			close(sock);
			exit(1);
		}

		printf("Sent some data.\n");
		pressed = getchar();
	}
	close(sock);
	return 0;
}
