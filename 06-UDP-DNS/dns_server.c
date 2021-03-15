// DNS UDP server that can handle multiple clients simultaneously

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

// function to verify if the IP address already exists in the table
int is_IpTaken(Entry table[MAX_DOMAIN], char* address)
{
	for(int i=0; i<MAX_DOMAIN; i++)
	{
		for (int j = 0; j < MAX_ADDR; j++)
		{
			if(table[i].address[j][0] && strcmp(table[i].address[j],address)==0)
			{
				printf("[ISSUE]IP address has already been taken !\n");
				return 0;
			}
		}
	}

	return 1;
}

// function to check if any character in the IP address is not a number
int validate_number(char* str)
{
   while(*str)
   {
      if(!isdigit(*str))
         return 0;

      str++;
   }

   return 1;
}

// function to check the validity of the IP address
int validate_ip(char *ip)
{
	int num;
	int dots = 0;

	char *ptr;

	if (ip == NULL)
	return 0;

	// when a dot is encountered, slice the string

	//breaks string str into a series of tokens using the dot(.) as the delimiter
	ptr = strtok(ip, ".");

	if (ptr == NULL)
		return 0;

	while(ptr)
	{
		if (!validate_number(ptr))
			return 0;

		num = atoi(ptr); // convert the slice from the IP address into a number

		if (num >= 0 && num <= 255)
		{
			ptr = strtok(NULL, "."); // slice the next token of the string

			if (ptr != NULL)
				dots++; // track the number of dots in the address
		}

		else
			return 0;
	}

	if (dots != 3) // valid IP addresses cannot have more than three dots
		return 0;

	return 1;
}

// function to create a table entry
int createEntry(Entry table[MAX_DOMAIN], char* domain, char* address)
{
    int index = -1;
    int flag = 0;

    for (int i = 0; i < MAX_DOMAIN; i++) // to find the domain entry if it already exists
    {
        if (strcmp(table[i].domain, domain) == 0)
        {
            for (int j = 0; j < MAX_DOMAIN; j++)
            {
                if (!table[i].address[j][0])
                {
                    strcpy(table[i].address[j], address);
                    flag = 1;
                    break;
                }
		   	}

            break;
        }

        if (!table[i].domain[0] && index == -1)
            index = i;
    }

    if (!flag)
    {
        strcpy(table[index].domain, domain);
        strcpy(table[index].address[0], address);
        flag = 1;
    }

    return flag;
}

// function to return the IP address
Entry *getAddress(Entry *table, char *const domain)
{
    // so that the value is retained through the run of the program
    static Entry result;

    bzero(&result, sizeof(Entry));
    strcpy(result.domain, domain);

    for (int i = 0; i < MAX_DOMAIN; i++)
    {
        if (strcmp(table[i].domain, domain) == 0)
        {
            for (int j = 0; j < MAX_ADDR; j++)
            {
                strcpy(result.address[j], table[i].address[j]);
            }
            break;
        }
    }

    return (&result);
}

void print_table(Entry *table)
{
	printf("\n");
	printf("+---------------------+----------------------+\n");
	printf("|     Server Name     |        Address       |\n");
	printf("+---------------------+----------------------+\n");

	for (int i = 0; i < MAX_DOMAIN; i++)
	{
		if (table[i].domain[0])
		{
			printf("| %-19s | %-20s |\n", table[i].domain, table[i].address[0]);

			for (int j = 1; j < MAX_ADDR && table[i].address[j][0]; j++)
				printf("| %-19s | %-20s |\n", "", table[i].address[j]);

			printf("+---------------------+----------------------+\n");
		}
	}

	printf("\n");
}

void update_table(Entry *table)
{

	int flag = 0;
	char temp[MAX];
	string dn,ip;

	printf("\nEnter the domain name: ");
	scanf(" %[^\n]", dn);

	while(flag == 0)
	{
		printf("Enter ip address: ");
		scanf(" %[^\n]", ip);
		strcpy(temp,ip);
		flag = validate_ip(temp);

		if(flag == 0)
			printf("[ISSUE]IP address entered was INVALID !\n");

		else
			flag = is_IpTaken(table,ip);
	}

	createEntry(table,dn,ip);

	printf("\nUPDATED TABLE\n");
	print_table(table);

}

// error - wrapper for perror
void error(char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char **argv)
{
	/*------------------------------------------------------
			For establishing the UDP connection
	------------------------------------------------------*/

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


	/*------------------------------------------------------
			For the DNS operations
	------------------------------------------------------*/

	Entry table[MAX_DOMAIN];
	bzero(table, MAX_DOMAIN * sizeof(Entry));
	char option = 'y';

	// predefined entries from the question
	createEntry(table, "www.yahoo.com", "10.2.45.67");
	createEntry(table, "www.annauniv.edu", "197.34.53.122");
	createEntry(table, "www.google.com", "142.89.78.66");


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

	print_table(table);

	printf("Do you want to update the table?[y/n]: ");
	scanf(" %c",&option);

	option = tolower(option);

	if(option == 'y')
		update_table(table);

	while (1)
	{

		// recvfrom: receive a UDP datagram from a client
		bzero(buf, MAX);
		n = recvfrom(sockfd, buf, MAX, MSG_WAITALL,(struct sockaddr *) &clientaddr, &clientlen);

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

		/*---------------------------
					DNS
		---------------------------*/

		Entry *result = getAddress(table, buf);

		// sendto: echo the input back to the client
		n = sendto(sockfd, result, sizeof(Entry), MSG_CONFIRM, (struct sockaddr *) &clientaddr, clientlen);

		if (n < 0)
		  error("[FAIL] in sendto");
	}
}