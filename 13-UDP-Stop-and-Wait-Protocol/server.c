
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define PORT 8080 
#define MAXLINE 1024 


#define N 5
#define WS 10 //Because 0f '\0' it isn't 8

typedef struct node
{
	int frameno;
	char data[WS];
	int flag;
}frame;

int main()
{ 
	int sockfd;
	struct sockaddr_in servaddr, cliaddr; 
	frame f1;
	
	char *ack = "ACK";
	char *noack="NACK";
	
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
	perror("socket creation failed"); 
	exit(EXIT_FAILURE); 
	} 

	memset(&servaddr, 0, sizeof(servaddr)); 
	memset(&cliaddr, 0, sizeof(cliaddr)); 

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY; 
	servaddr.sin_port = htons(PORT); 

	if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 )

	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 

	socklen_t len;
	int n; 

	len = sizeof(cliaddr); 
	for(int i=0;i<N;i++)
	{
		n = recvfrom(sockfd, &f1, sizeof(frame),MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
		
		printf("\n\nFRAME DETAILS");
		printf("\nFrame num:");
		printf("%d",f1.frameno);
		printf("\nData: ");
		printf("%s",f1.data);
		if(f1.flag == 0)
		{
			printf("\n%s",ack);
			sendto(sockfd, (const char *)ack, strlen(ack), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
			//sendto - client - ACK
		}

		else if(f1.flag == 1)
		{
			printf("\n%s",noack);
			sendto(sockfd, (const char *)noack, strlen(noack), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);  
			//sendto - client -NOACK  
		}
	}
	
	printf("\n");
	
	return 0; 
} 
