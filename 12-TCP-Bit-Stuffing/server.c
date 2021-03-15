/*
QUESTION

1. At client side read data from the user.
2. At client side perform bit stuffing and transmit the stuffed data to the server.
3. At the server side, read the stuffed data.
4. At the server side, after reading, unstuff it.
5. At the server side, display the original data to the user. Also display the original data with flags at both ends
*/

/*
BIT UNSTUFF - Remove the extra 0's after five one's from the data to regain the original data

read from client
perform above
show original data with flags at both ends
*/


#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX 1024
#define PORT 8080
#define SA struct sockaddr

// code for bit unstuffing
void bitUnstuffing(char stuffed[], char unstuffed[])
{
	int i, j;
	int n = 0;

	for(i=0, j=0; i<strlen(stuffed); i++,j++)
	{

		unstuffed[j] = stuffed[i];

		if(stuffed[i] == '1')
		{
			n++;
		}

		else
		{
			n = 0;
		}

		if(n == 5)
		{
			i++;
			n = 0;
			continue;
		}


	}

	unstuffed[j]='\0';
}

// code for Question 3, 4, 5
void bitStuff(int sockfd)
{
	char buff[MAX];

	char flag[] = "01111110";
	char unstuffed[MAX];


	bzero(buff, MAX);

	read(sockfd, buff, sizeof(buff));

	printf("\nCLIENT: %s",buff);

	printf("\n\nBEFORE Bit UNstuffing : %s", buff);

	bitUnstuffing(buff, unstuffed);
	printf("\nAFTER Bit UNstuffing : %s", unstuffed);

	printf("\nBEFORE FLAGS : 00000000%s00000000", unstuffed);
	printf("\nAFTER FLAGS  : %s%s%s", flag,unstuffed,flag);

	bzero(buff, MAX);

}

// Driver function
int main()
{
	int sockfd, connfd;
	socklen_t len;
	struct sockaddr_in servaddr, cli;


	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		printf("Socket creation failed !\n");
		exit(0);
	}
	else
		printf("Socket successfully created\n");

	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);


	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0)
	{
		printf("Socket bind failed !\n");
		exit(0);
	}
	else
		printf("Socket successfully binded\n");


	if ((listen(sockfd, 5)) != 0)
	{
		printf("Listen failed !\n");
		exit(0);
	}
	else
		printf("Server listening\n");
	len = sizeof(cli);

	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		printf("Server acccept failed !\n");
		exit(0);
	}
	else
		printf("Server acccepted the client\n");


	bitStuff(connfd);

	printf("\n\nSERVER Exit !\n");

	close(sockfd);
}
