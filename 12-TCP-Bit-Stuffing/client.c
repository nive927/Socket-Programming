/*
QUESTION

1. At client side read data from the user.
2. At client side perform bit stuffing and transmit the stuffed data to the server.
3. At the server side, read the stuffed data.
4. At the server side, after reading, unstuff it.
5. At the server side, display the original data to the user. Also display the original data with flags at both ends
*/

/*
BIT STUFFING - add a 0 after 5 consecutive one's to distinguish data from the FLAG: 01111110

Read data from user
perform above
send to server
*/

#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX 1024
#define PORT 8080
#define SA struct sockaddr

// code for bit stuffing
void bitStuffing(char data[], char stuffed[])
{
	int i, j;
	int n = 0;

	for(i=0, j=0; i<strlen(data); i++,j++)
	{

		stuffed[j] = data[i];

		if(data[i] == '1')
		{
			n++;
		}

		else
		{
			n = 0;
		}

		if(n == 5)
		{
			stuffed[++j] = '0';
			n = 0;
		}
	}

	stuffed[j] = '\0';
}

// code for Question 1, 2
void bitStuff(int sockfd)
{
	char buff[MAX];

	char data[MAX];
	char stuffed[MAX];

	bzero(buff, sizeof(buff));
	printf("\nEnter DATA: ");
	scanf("%s",data);

	printf("\n\nBEFORE Bit Stuffing: %s",data);

	bitStuffing(data, stuffed);
	printf("\nAFTER Bit stuffing : %s", stuffed);

	strcpy(buff, stuffed);

	write(sockfd, buff, sizeof(buff));
	bzero(buff, sizeof(buff));

}

int main()
{
	int sockfd;
	struct sockaddr_in servaddr;

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
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("Connection to the server failed !\n");
		exit(0);
	}
	else
		printf("Connected to the server\n");

	//  function
	bitStuff(sockfd);

	printf("\n\nCLIENT Exit !\n");

	close(sockfd);
}
