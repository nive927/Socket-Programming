#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void chat(int sockfd)
{
	char buff[MAX];
	int n;


	for(;;)
	{
		bzero(buff, MAX);

		// read the message from client and copy it into the buffer
		read(sockfd, buff, sizeof(buff));
		// print buffer which contains the client contents
		printf("\nCLIENT: %sEnter MESSAGE: ",buff);
		bzero(buff, MAX);
		n = 0;
		// copy server message in the buffer
		while ((buff[n++] = getchar()) != '\n')
			;

		// and send that buffer to client
		write(sockfd, buff, sizeof(buff));

		// if msg contains "exit" then the server exits and the chat is ended
		if (strncmp("exit", buff, 4) == 0)
		{
			printf("\nSERVER Exit !\n");
			break;
		}
	}
}

// Driver function
int main()
{
	int sockfd, connfd, len;
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
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0)
	{
		printf("Socket bind failed !\n");
		exit(0);
	}
	else
		printf("Socket successfully binded\n");

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0)
	{
		printf("Listen failed !\n");
		exit(0);
	}
	else
		printf("Server listening\n");
	len = sizeof(cli);

	// Accept the data packet from client and verification
	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		printf("Server acccept failed !\n");
		exit(0);
	}
	else
		printf("Server acccepted the client\n");

	// Function for chatting between client and server
	chat(connfd);

	// After chatting close the socket
	close(sockfd);
}
