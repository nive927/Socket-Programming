#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "arp.h"

#define PORT 8888
#define MAX 1024
#define SA struct sockaddr

int main()
{
	int clientSocket;
	struct sockaddr_in serveraddr;

	arp packet, rec_packet;

	printf("\nARP PACKET DETAILS\n");
	printf("IP Address: ");
	scanf("%s", packet.src_ip);
	printf("MAC Address: ");
	scanf("%s", packet.src_mac);

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0)
	{
		printf("[FAILED]Socket creation unsuccessful\n");
		exit(1);
	}

	bzero(&serveraddr, sizeof(serveraddr));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(PORT);
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");


	if(connect(clientSocket, (SA*)&serveraddr, sizeof(serveraddr)) != 0)
	{
		printf("[FAILED]Connection to Server unsuccessful\n");
		exit(1);
	}

	bzero(&rec_packet, sizeof(rec_packet));
	recv(clientSocket, (void*)&rec_packet, sizeof(rec_packet), 0);

	printf("\nReceived REQ ARP\n");
	printARPPacket(rec_packet);

	if(strcmp(packet.src_ip, rec_packet.dest_ip) == 0)
	{
		printf("\n[SUCCESS]IP Address MATCHED\n");
		packet.mode = ACK;
		strcpy(packet.dest_ip, rec_packet.src_ip);
		strcpy(packet.dest_mac, rec_packet.src_mac);

		send(clientSocket, (void*)&packet, sizeof(packet), 0);
		printf("\nSent ARP DATA\n");
		printARPPacket(packet);

		bzero(&rec_packet, sizeof(rec_packet));
		recv(clientSocket, (void*)&rec_packet, sizeof(rec_packet), 0);
		printf("\nReceived this ARP Packet\n");
		printARPPacket(rec_packet);
	}

	else
	{
		printf("\n[FAIL]IP Address DID NOT MATCH\n");
	}

	close(clientSocket);

	return 0;
}