/// UDP client

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define MAX 1024
#define PORT 8080
#define HOSTNAME "localhost"

// error - wrapper for perror
void error(char *msg)
{
	perror(msg);
	exit(0);
}

int main(int argc, char **argv)
{
    int sockfd, portno, n;
    socklen_t serverlen;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char *hostname;
    char buf[MAX];

    // check command line arguments
    if (argc > 3)
    {
       fprintf(stderr,"[FAIL]usage: %s <hostname> <port> or blank\n", argv[0]);
       exit(0);
    }

    else if(argc == 3)
    {
		hostname = argv[1];
    	portno = atoi(argv[2]);
	}

	else
	{
		portno = PORT;
		hostname = HOSTNAME;
	}

    // socket: create the socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        error("[FAIL] Unable to create/open parent socket");

    printf("[SUCCESS] Parent Socket Created\n");

    // gethostbyname: get the server's DNS entry
    server = gethostbyname(hostname);
    if (server == NULL)
    {
        fprintf(stderr,"[FAIL] No host with name %s\n", hostname);
        exit(0);
    }

    // build the server's Internet address
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(portno);

    // get a message from the user
    bzero(buf, MAX);
    printf("\nPlease enter msg: ");
    fgets(buf, MAX, stdin);

    // send the message to the server
    serverlen = sizeof(serveraddr);
    n = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&serveraddr, serverlen);
    if (n < 0)
      error("[FAIL] in sendto");

    // print the server's reply
    n = recvfrom(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&serveraddr, &serverlen);
    if (n < 0)
      error("[FAIL] in recvfrom");

    printf("\nEcho from server: %s", buf);
    return 0;
}