// UDP server that can handle multiple clients simultaneously

#include <stdio.h>
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

// error - wrapper for perror
void error(char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char **argv)
{
	int sockfd; /* socket */
	int portno; /* port to listen on */
	socklen_t clientlen; /* byte size of client's address */
	struct sockaddr_in serveraddr; /* server's addr */
	struct sockaddr_in clientaddr; /* client addr */
	struct hostent *hostp; /* client host info */
	char buf[MAX]; /* message buf */
	char *hostaddrp; /* dotted decimal host addr string */
	int optval; /* flag value for setsockopt */
	int n; /* message byte size */

	// check command line arguments
	if (argc > 2)
	{
		fprintf(stderr, "[FAIL]Usage: %s <port> | blank[8080]\n", argv[0]);
		exit(1);
	}

	else if(argc == 2)
		portno = atoi(argv[1]);

	else
		portno = PORT;

	// socket: create the parent socket

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	if (sockfd < 0)
		error("[FAIL] Unable to create/open parent socket");

	printf("[SUCCESS] Parent Socket Created\n");

	/* setsockopt: Handy debugging trick that lets
	* us rerun the server immediately after we kill it;
	* otherwise we have to wait about 20 secs.
	* Eliminates "ERROR on binding: Address already in use" error.
	*/
	optval = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,(const void *)&optval , sizeof(int));


	// build the server's Internet address
	bzero((char *) &serveraddr, sizeof(serveraddr));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons((unsigned short)portno);


	// bind: associate the parent socket with a port
	if (bind(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
		error("[FAIL] Unable to bind parent socket to the port");

	printf("[SUCCESS] Binding of the parent socket to port %d complete\n",portno);

	// main loop: wait for a datagram, then echo it
	clientlen = sizeof(clientaddr);

	while (1)
	{

		// recvfrom: receive a UDP datagram from a client
		bzero(buf, MAX);
		n = recvfrom(sockfd, buf, MAX, 0,(struct sockaddr *) &clientaddr, &clientlen);

		if (n < 0)
		  error("[FAIL] in recvfrom");

		 // gethostbyaddr: determine who sent the datagram
		hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, sizeof(clientaddr.sin_addr.s_addr), AF_INET);

		if (hostp == NULL)
		  error("[FAIL] on gethostbyaddr when trying to determine the client");

		hostaddrp = inet_ntoa(clientaddr.sin_addr);

		if (hostaddrp == NULL)
		  error("[FAIL] on inet_ntoa\n");

		printf("[SUCCESS] UDP datagram received from client %s (%s)\n", hostp->h_name, hostaddrp);

		printf("\nServer received %ld/%d bytes: %s\n", strlen(buf), n, buf);

		// sendto: echo the input back to the client
		n = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *) &clientaddr, clientlen);

		if (n < 0)
		  error("[FAIL] in sendto");
	}
}