/*Maximum Limit of memory allocation for string variables*/

#define LIMIT 50

/*ARP Operation Codes*/

#define REQ 1
#define ACK 2
#define DATA 3

/*Structure for the ARP Packets*/

struct ARP_PACKET
{
	int mode;
	char src_ip[LIMIT];
	char dest_ip[LIMIT];
	char src_mac[LIMIT];
	char dest_mac[LIMIT];
	char data[LIMIT];
};

typedef struct ARP_PACKET arp;

/*Helper function for CREATION of ARP Packets*/

arp createARPPacket(int mode)
{
	arp packet;
	bzero(&packet, sizeof(packet));

	packet.mode = mode;
	printf("\nARP PACKET DETAILS\n");
	printf("Source IP: ");
	scanf("%s", packet.src_ip);
	printf("Source MAC:  ");
	scanf("%s", packet.src_mac);
	printf("Destination IP: ");
	scanf("%s", packet.dest_ip);
	printf("Data (16-bit): ");
	scanf("%s", packet.data);

	return packet;
}

/*Helper function for PRINTING the Details of ARP Packets*/

void printARPPacket(arp packet)
{
	switch(packet.mode)
	{
		case REQ: 	printf("%d|%s|%s|%s|%s\n", packet.mode, packet.src_mac, packet.src_ip, "00:00:00:00:00:00", packet.dest_ip);
					break;
		case ACK:	printf("%d|%s|%s|%s|%s\n", packet.mode, packet.src_mac, packet.src_ip, packet.dest_ip, packet.dest_mac);
					break;
		case DATA:	printf("%d|%s|%s|%s|%s|%s\n", packet.mode, packet.src_mac, packet.src_ip, packet.dest_ip, packet.dest_mac, packet.data);
					break;
	}
}
