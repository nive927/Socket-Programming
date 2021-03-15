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
	char buff[MAX], fname[MAX], newpath[MAX];
	int n, valread = 0;

	FILE * fptr;

	bzero(buff, sizeof(buff));
	printf("\nEnter the path of the file: ");

	n = 0;
	while ((buff[n++] = getchar()) != '\n')
		;

	write(sockfd, buff, sizeof(buff-1));// remove the \n
	strncpy(fname, buff, sizeof(buff)-1);// remove the \n
	bzero(buff, sizeof(buff));
	valread = read(sockfd, buff, sizeof(buff));

	if(valread != 0)
		printf("[SUCCESS]File transferred from the server\n");

	printf("\n\nSERVER has sent the file %s\n\nFile Content:\n%s\n", fname, buff);

	//move the file to a new location
	/*
	strcpy(newpath, "newLocation/");
	strcat(newpath, fname);
	fptr = fopen(newpath, "w+");
	fprintf(fptr,"%s\n", buff);
	fclose(fptr);
	*/

	printf("\n\nSave the file in path: ");
	scanf("%s", newpath);
	fptr = fopen(newpath, "w");
	fprintf(fptr,"%s\n", buff);
	fclose(fptr);
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

	// Function for transferring the file from the server to the client
	file_transfer(sockfd);

	// close the socket
	close(sockfd);
}
