
#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros

#define TRUE 1
#define FALSE 0
#define PORT 8888
#define MAX 1024

void chat(int sockfd, char buff[MAX], int i)
{
	int n;

	// print buffer which contains the client contents
	printf("\nCLIENT%d: %sEnter MESSAGE: ", i, buff);
	bzero(buff, MAX);
	n = 0;
	// copy server message in the buffer
	while ((buff[n++] = getchar()) != '\n')
		;

	// and send that buffer to client
	write(sockfd, buff, strlen(buff));
	//send(sockfd , buff , strlen(buff) , 0 );
}

int main(int argc , char *argv[])
{
	int opt = TRUE;
	int master_socket , addrlen , new_socket , client_socket[30], max_clients = 30 , activity, i , valread , sd;
	int max_sd;
	struct sockaddr_in address;

	char buffer[MAX]; //data buffer of 1K

	//set of socket descriptors
	fd_set readfds;

	//initialise all client_socket[] to 0 so not checked
	for (i = 0; i < max_clients; i++)
	{
		client_socket[i] = 0;
	}

	//create a master socket
	if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
	{
		perror("[FAILED]Server Socket creation unsuccessful\n");
		exit(EXIT_FAILURE);
	}

	//set master socket to allow multiple connections ,
	//this is just a good habit, it will work without this
	if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
	{
		perror("[FAILED](setsockopt)Multiple connection allowance unsuccessful\n");
		exit(EXIT_FAILURE);
	}

	printf("[SUCCESS]Server Socket has been created\n");

	//type of socket created
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );

	//bind the socket to localhost port 8888
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
	{
		perror("[FAILED]Error when binding\n");
		exit(EXIT_FAILURE);
	}

	printf("[SUCCESS]Listener on port %d \n", PORT);

	//try to specify maximum of 3 pending connections for the master socket
	if (listen(master_socket, 3) < 0)
	{
		perror("[FAILED]Unable to listen\n");
		exit(EXIT_FAILURE);
	}

	printf("[SUCCESS]Listening....\n");

	//accept the incoming connection
	addrlen = sizeof(address);
	puts("[WAIT]Waiting for connections ...");

	while(TRUE)
	{
		//clear the socket set
		FD_ZERO(&readfds);

		//add master socket to set
		FD_SET(master_socket, &readfds);
		max_sd = master_socket;

		//add child sockets to set
		for ( i = 0 ; i < max_clients ; i++)
		{
			//socket descriptor
			sd = client_socket[i];

			//if valid socket descriptor then add to read list
			if(sd > 0)
				FD_SET( sd , &readfds);

			//highest file descriptor number, need it for the select function
			if(sd > max_sd)
				max_sd = sd;
		}

		//wait for an activity on one of the sockets , timeout is NULL ,
		//so wait indefinitely
		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);

		if ((activity < 0) && (errno!=EINTR))
		{
			printf("[FAILED]Socket selection error\n");
		}

		//If something happened on the master socket ,
		//then its an incoming connection
		if (FD_ISSET(master_socket, &readfds))
		{
			if ((new_socket = accept(master_socket,(struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
			{
				perror("[FAILED]Server didn't accept Client socket\n");
				exit(EXIT_FAILURE);
			}

			//inform user of socket number - used in send and receive commands
			printf("\n[SUCCESS]New connection with socket fd: %d  ip: %s port: %d\n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

			//add new socket to array of sockets
			for (i = 0; i < max_clients; i++)
			{
				//if position is empty
				if( client_socket[i] == 0 )
				{
					client_socket[i] = new_socket;
					printf("[SUCCESS]Adding to list of sockets as %d\n" , i);
					break;
				}
			}
		}

		//else its some IO operation on some other socket
		for (i = 0; i < max_clients; i++)
		{
			sd = client_socket[i];

			if (FD_ISSET( sd , &readfds))
			{
				//Check if it was for closing , and also read the
				//incoming message
				if ((valread = read( sd , buffer, 1024)) == 0)
				{
					//Somebody disconnected , get his details and print
					getpeername(sd , (struct sockaddr*)&address ,(socklen_t*)&addrlen);
					printf("Host disconnecting with ip: %s port: %d \n" ,inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

					//Close the socket and mark as 0 in list for reuse
					close( sd );
					client_socket[i] = 0;
				}

				//Echo back the message that came in
				else
				{
					//set the string terminating NULL byte on the end
					//of the data read
					buffer[valread] = '\0';
					//send(sd , buffer , strlen(buffer) , 0 );

					chat(sd, buffer, i+1);
				}
			}
		}
	}

	return 0;
}
