/*TCP Client Program to simulate Hamming Code 1-bit Error Detection and Correction*/

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

void hc_err_detect(int sockfd)
{
	char buff[MAX];
	int valread = 0;

	char message[MAX];
	int h, hamming[MAX];
	int data[MAX] = {0};
	int r, m;
	int i, j, k;
	int parity;
	int error[MAX];
	int msg[MAX];

	r = 0;

	bzero(buff, sizeof(buff));
	valread = read(sockfd, buff, sizeof(buff));

	if(valread != 0)
	{
		strcpy(message, buff);

		h = strlen(message);

		printf("\nData received: %s",message);

		/*Reversing the msg read
		arr indexing: 0 to len - 1
		msg bit read has indexing: len-1 to 0
		msb should be in array index len_of_array - 1 but will be in index 0
		To correct this, store the msg in data in the reversed order*/
		for(i=h, j=0; i>=1; i--, j++)
			hamming[i] = message[j] - '0';

		/*Calculating the number of parity bits, r needed for the code
		r - number of parity bits*/
		while((h + 1)>(pow(2,r)))
			r++;

		/*Set all the parity locations to -1
		and add the msg bits in the appropriate location*/

		// h - length of the data code with redundant bits
		m = h - r;

		/*
			//DEBUGGING
			printf("\nh:%d m:%d r:%d", h, m, r);
		*/

		int index = 1;

		/*Find the EVEN parity values for the redundant bits*/
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
				data[index++] = 0;
			else
				data[index++] = 1;
		}

		/*Need to reverse*/
		for(i=r, j=1; i>=1; i--, j++)
			error[j] = data[i];


		printf("\nCalulated redundant bits(error pos): ");
		for(i=1; i<=r;i++)
			printf("%d", error[i]);
		printf("\n");

		int c = r-1;
		int err_pos = 0;

		/*Converting the binary value in the error int array into the equivalent decimal index*/

		for(i=1; i<=r; i++)
			err_pos += (error[i] * pow(2, c--));

		/*DEBUG
		printf("\nError position: %d", err_pos);
		*/

		// 0 index means no error since all indexing in this program starts from 1 to match the indexing for hamming code which starts with 1
		if(err_pos != 0)
		{
			hamming[err_pos] = hamming[err_pos] == 0 ?1:0; // changing the 0 to 1 and vice-versa
			//printf("\nCorrected data: ");
		}
		/*
		else
			printf("Data: ");

		for(i=h; i>=1; i--)
			printf("%d", hamming[i]);
		printf("\n");
		*/


		/*Setting the redundant bit positions to -1*/
		int p;

		j = 0;

		for(i=1;i<=h;i++)
		{
			p = pow(2,j);

			if(p == i)
			{
				hamming[i] = -1;
				j++;
			}
		}

		/*DEBUG
		printf("-1 Data: ");
		for(i=h; i>=1; i--)
			printf("%d", hamming[i]);
		printf("\n");
		*/

		/*Retrieving the message from the hamming code without the redundant bits*/
		index = 1;
		for(i=1; i<=h; i++)
		{
			if (hamming[i] != -1)
				msg[index++] = hamming[i];
		}
		printf("Corrected Data: ");
		for(i=m; i>=1; i--)
			printf("%d", msg[i]);
		printf("\n");

	}
}

int main()
{
	int sockfd;
	struct sockaddr_in servaddr;

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
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // loopback IP address
	servaddr.sin_port = htons(PORT); // communication with the same system

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("[FAILED]Connection to the server failed !\n");
		exit(0);
	}
	else
		printf("[SUCCESS]Connected to the server\n");

	// Function for error detection on the hamming code received
	hc_err_detect(sockfd);

	// close the socket
	close(sockfd);
}
