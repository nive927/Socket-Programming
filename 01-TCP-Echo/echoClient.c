#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void echo(int sockfd)
{
	char buff[MAX];
	int n;

	for (;;)
	{
		bzero(buff, sizeof(buff));
		printf("\nCLIENT Message: ");

		n = 0;
		// Append the message to the buffer till a * is encountered
		while ((buff[n++] = getchar()) != '*')
			;

		// Removing the * character from the message
		buff[strlen(buff)-1] = '\0';

		write(sockfd, buff, sizeof(buff));
		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));

		printf("ECHOED message from SERVER:\n%s", buff);
		printf("\nCLIENT Exit !\n");
		break;
	}
}

int main()
{
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		printf("Socket creation failed !\n");
		exit(0);
	}
	else
		printf("Socket successfully created\n");

	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // loopback IP address
	servaddr.sin_port = htons(PORT); // communication with the same system

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("Connection to the server failed !\n");
		exit(0);
	}
	else
		printf("Connected to the server\n");

	// function for echo message
	echo(sockfd);

	// close the socket
	close(sockfd);
}
