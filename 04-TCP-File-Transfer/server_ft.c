#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX 1024
#define PORT 8080
#define SA struct sockaddr

void file_transfer(int sockfd)
{
	char buff[MAX], line[100], str[MAX];

	FILE *fptr;

	bzero(buff, MAX);

	// read the file name from client and copy it into the buffer
	read(sockfd, buff, sizeof(buff));
	// print buffer which contains the client contents
	printf("\nCLIENT has requested: %s\n",buff);

	fptr = fopen(buff, "r");
	while (fgets(line, sizeof(line), fptr))
		strcat(str, line);

	fclose (fptr);

	bzero(buff, MAX);

	// copy the file contents into the buffer
	//int len = strlen(str);
	//str[len] = '\0';
	//printf("%s",str);
	strcpy(buff, str);

	// and send that buffer to client
	write(sockfd, buff, sizeof(buff));
}

// Driver function
int main()
{
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		printf("[FAILED]Socket creation failed !\n");
		exit(0);
	}
	else
		printf("[SUCCESS]Socket successfully created\n");

	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0)
	{
		printf("[FAILED]Socket bind failed !\n");
		exit(0);
	}
	else
		printf("[SUCCESS]Socket successfully binded\n");

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0)
	{
		printf("[FAILED]Listen failed !\n");
		exit(0);
	}
	else
		printf("[SUCCESS]Server listening\n");

	socklen_t cli_size;
	cli_size = sizeof(struct sockaddr_in);

	// Accept the data packet from client and verification
	connfd = accept(sockfd, (SA*)&cli, &cli_size);
	if (connfd < 0) {
		printf("[FAILED]Server acccept failed !\n");
		exit(0);
	}
	else
		printf("[SUCCESS]Server acccepted the client\n");

	// Function for transferring the file from the server to the client
	file_transfer(connfd);

	// After transfer close the socket
	close(sockfd);
}
