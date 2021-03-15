#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

 /*FD_SET, FD_ISSET, FD_ZERO macros*/

#include <sys/time.h>

 /*ARP Library*/

#include "arp.h"

/*Useful Substitutions*/

#define TRUE 1
#define FALSE 0
#define PORT 8888
#define MAX 1024

/*Driver*/

int main(int argc , char *argv[])
{
	/*INIT for Socket Connection*/
	int opt = TRUE;
	int master_socket;
	int addrlen;
	int new_socket;
	int client_socket[30];
	int max_clients = 30;
	int activity;
	int i;
	int sd;
	int max_sd;

	struct sockaddr_in address;

	// -------------------------------------------------------------------------
		/*Creating ARP REQ packet*/
		arp packet;
		arp rec_packet;

		packet = createARPPacket(REQ);
		printf("\nDeveloping ARP REQ Packet\n");
		printARPPacket(packet);
		printf("\nARP Packet BROADCAST\n");
		printf("[ARP WAITING]......\n");
	// -------------------------------------------------------------------------

	// -------------------------------------------------------------------------
	/*Establishing Socket Connection*/

	/*Set of socket descriptors*/
	fd_set readfds;

	/*Initialise all client_socket[] to 0 so not checked*/
	for (i = 0; i < max_clients; i++)
	{
		client_socket[i] = 0;
	}

	/*create a master socket*/
	if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
	{
		perror("[FAILED]Server Socket creation unsuccessful\n");
		exit(EXIT_FAILURE);
	}

	/*Set master socket to allow multiple connections ,
	this is just a good habit, it will work without this*/
	if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
	{
		perror("[FAILED](setsockopt)Multiple connection allowance unsuccessful\n");
		exit(EXIT_FAILURE);
	}

	/*Type of socket created*/
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );

	/*bind the socket to localhost port 8888*/
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
	{
		perror("[FAILED]Error when binding\n");
		exit(EXIT_FAILURE);
	}

	/*Specify maximum of 3 pending connections for the master socket*/
	if (listen(master_socket, 3) < 0)
	{
		perror("[FAILED]Unable to listen\n");
		exit(EXIT_FAILURE);
	}

	/*accept the incoming connection*/
	addrlen = sizeof(address);


	while(TRUE)
	{
		/*Clear the socket set*/
		FD_ZERO(&readfds);

		/*Add master socket to set*/
		FD_SET(master_socket, &readfds);
		max_sd = master_socket;

		/*add child sockets to set*/
		for ( i = 0 ; i < max_clients ; i++)
		{
			/*socket descriptor*/
			sd = client_socket[i];

			/*if valid socket descriptor then add to read list*/
			if(sd > 0)
				FD_SET( sd , &readfds);

			/*highest file descriptor number, need it for the select function*/
			if(sd > max_sd)
				max_sd = sd;
		}

		/*wait for an activity on one of the sockets , timeout is NULL ,
		so wait indefinitely*/
		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);

		if ((activity < 0) && (errno!=EINTR))
		{
			printf("[FAILED]Socket selection error\n");
		}

		/*If something happened on the master socket ,
		then its an incoming connection*/
		if (FD_ISSET(master_socket, &readfds))
		{
			if ((new_socket = accept(master_socket,(struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
			{
				perror("[FAILED]Server didn't accept Client socket\n");
				exit(EXIT_FAILURE);
			}

			/*inform user of socket number - used in send and receive commands*/

			// FOR DEBUGING: printf("\n[SUCCESS]New connection with socket fd: %d  ip: %s port: %d\n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
			send(new_socket,(void*)&packet,sizeof(packet),0);

			/*add new socket to array of sockets*/
			for (i = 0; i < max_clients; i++)
			{
				/*if position is empty*/
				if( client_socket[i] == 0 )
				{
					client_socket[i] = new_socket;
					//printf("[SUCCESS]Adding to list of sockets as %d\n" , i);
					break;
				}
			}
		}

		/*Broadcast the arp packet*/
		for (i = 0; i < max_clients; i++)
		{
			sd = client_socket[i];

			bzero((void*)&rec_packet, sizeof(rec_packet));

			if (FD_ISSET( sd , &readfds))
			{
				/*Check for ARP Response*/

				recv(sd,(void*)&rec_packet, sizeof(rec_packet), 0);

				if(rec_packet.mode == ACK)
				{
					printf("\nARP ACK REPLY\n");
					printARPPacket(rec_packet);

					strcpy(packet.dest_mac,rec_packet.src_mac);
					packet.mode = DATA;

					printf("Sending the ARP DATA Packet to %s\n", packet.dest_mac);

					send(new_socket, (void*)&packet, sizeof(packet),0);

					printf("\nSent this ARP DATA Packet\n");
					printARPPacket(packet);
					exit(EXIT_SUCCESS);

				}
			}
		}
	}

	return 0;
}