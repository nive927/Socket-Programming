/// DNS UDP client

#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX 1024
#define PORT 8080

#define MAX_ADDR 10
#define MAX_DOMAIN 20

typedef char string[MAX];

typedef struct Entry
{
	string domain;
	string address[MAX_ADDR];
}Entry;

// error - wrapper for perror
void error(char *msg)
{
	perror(msg);
	exit(0);
}

int main(int argc, char **argv)
{
    /*------------------------------------------------------
					For the UDP connection
	------------------------------------------------------*/

    int sockfd;
    socklen_t serverlen;
    struct sockaddr_in serveraddr;

    /*------------------------------------------------------
				For the DNS operations
	------------------------------------------------------*/

	Entry query;

    // socket: create the socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        error("[FAIL] Unable to create/open parent socket");

    printf("[SUCCESS] Parent Socket Created\n");

    // build the server's Internet address
    bzero((char *) &serveraddr, sizeof(serveraddr));

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddr.sin_port = htons(PORT);

	int len = sizeof(Entry);
	serverlen = sizeof(serveraddr);

	while(1)
	{
		bzero(&query, sizeof(Entry));
		printf("\nEnter the domain name: ");
		scanf(" %[^\n]", query.domain);

		if (strcmp(query.domain, "END") == 0)
			break;

		sendto(sockfd, query.domain, sizeof(query.domain), MSG_CONFIRM, (struct sockaddr *)&serveraddr, serverlen);

		recvfrom(sockfd, &query, sizeof(Entry), MSG_WAITALL, (struct sockaddr *)&serveraddr, &len);

		if (!query.address[0][0])
			printf("No entry in the DNS Table !\n");
		else
		{
			printf("The IP Address is: \n");
			for (int i = 0; i < MAX_ADDR; i++)
			{
				if (query.address[i][0])
					printf("%s\n", query.address[i]);
			}
			printf("\n");
		}
	}

	close(sockfd);
    return 0;
}