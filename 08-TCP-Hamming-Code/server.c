/*TCP Server Program to simulate Hamming Code Generation and Transmission*/

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

#include <math.h>

#define MAX 1024
#define PORT 8080
#define SA struct sockaddr

void hc_gen(int sockfd)
{
	char buff[MAX];

	char msg[MAX];
	int data[MAX];
	int m;
	int hamming[MAX] = {0};
	int r, h;
	int i, j, k;
	int p;
	int parity;

	/*Reading the message or data to be sent*/

	printf("\nInput data: ");
	scanf("%s", msg);

	m = strlen(msg);

	/*Reversing the msg read
	arr indexing: 0 to len - 1
	msg bit read has indexing: len-1 to 0
	msb should be in array index len_of_array - 1 but will be in index 0
	To correct this, store the msg in data in the reversed order*/
	for(i=m-1, j=0; i>=0; i--, j++)
		data[i] = msg[j] - '0';

	/*
	//DEBUGGING
	printf("\nm:%d", m);
	for(i=0;i<m;i++)
		printf("\ndata%d: %d", i+1,data[i]);
	printf("\n");
	*/

	r = 0;

	/*Calculating the number of redundant bits, r needed for the code
	r - number of parity bits*/
	while((m + r + 1)>(pow(2,r)))
		r++;

	printf("Number of redundant bits needed is: %d", r);

	/*Set all the parity locations to -1
	and add the msg bits in the appropriate location*/

	// h - length of the hamming code with redundant bits
	h = r + m;

	// exponent for 2
	j = 0;

	// index for data
	k = 0;

	for(i=1;i<=h;i++)
	{
		p = pow(2,j);

		if(p == i)
		{
			hamming[i] = -1;
			j++;
		}

		else
		{
			hamming[i] = data[k];
			k++;
		}
	}

	/*
		//DEBUGGING
		printf("\nh:%d", h);
		for(i=1;i<=h;i++)
			printf("\ndata%d: %d", i, hamming[i]);
		printf("\n");
	*/

	/*Set the EVEN parity values for the redundant bits*/
	k = 0;
	int x, min, max = 0;

	for (i = 1; i <= h; i = pow (2, k))
	{
		k++;
		parity = 0;
		j = i;
		//x = i;
		min = 1;
		max = i;

		while(j <= h)
		{
			for(x = j; max >= min && x <= h; min++, x++)
			{
				if (hamming[x] == 1)
					parity = parity + 1;
			}

			j = x + i;
			min = 1;
		}

		//checking for even parity
		if (parity % 2 == 0)
			hamming[i] = 0;
		else
			hamming[i] = 1;
	}

	/*Need to display in reverse */
	printf("\nData with redundant bits(Hamming Code): ");
	for(i=h; i>=1; i--)
		printf("%d", hamming[i]);
	printf("\n");

	printf("Introduce error in data: ");
	scanf("%s", msg);

	m = strlen(msg);

	bzero(buff, MAX);

	strcpy(buff, msg);
	//printf("\nbuff: %s", buff);

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

	// Function to obtain the sender message and calculate the hamming code
	hc_gen(connfd);

	// After transfer close the socket
	close(sockfd);
}
