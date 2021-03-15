#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8888
#define MAX 1024
#define SA struct sockaddr

void chat(int sockfd)
{
	char buff[MAX];
	int n;

	for (;;)
	{
		bzero(buff, sizeof(buff));
		printf("\nMESSAGE: ");

		n = 0;
		while ((buff[n++] = getchar()) != '\n')
			;

		write(sockfd, buff, sizeof(buff));
		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));

		printf("SERVER: %s", buff);
		if ((strncmp(buff, "exit", 4)) == 0)
		{
			printf("\n[EXIT]SERVER has ended the connection\n");
			break;
		}
	}
}

int main()
{

	int clientSocket;
	struct sockaddr_in serveraddr;

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0)
	{
		printf("[FAILED]Socket creation unsuccessful\n");
		exit(1);
	}

	printf("[SUCCESS]Client Socket has been created\n");

	bzero(&serveraddr, sizeof(serveraddr));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(PORT);
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");


	if(connect(clientSocket, (SA*)&serveraddr, sizeof(serveraddr)) != 0)
	{
		printf("[FAILED]Connection to Server unsuccessful\n");
		exit(1);
	}

	printf("[SUCCESS]Client is now Connected to Server\n");

	chat(clientSocket);

	close(clientSocket);

	return 0;
}