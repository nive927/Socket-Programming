/*NOTE:
Removed code from echo server style and try without error checks
With error checks - not working

*/
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

	char buffer[MAXLINE]; 
	struct sockaddr_in servaddr; 

	frame f[5];
	frame f1;

	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 

	memset(&servaddr, 0, sizeof(servaddr)); 

	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(PORT); 
	servaddr.sin_addr.s_addr = INADDR_ANY; 
	
	int n, len;
	
	for(int i=0;i<N;i++)
	{
		printf("\nEnter DATAfor Frame%d [8 Bit]: ",(i+1));
		scanf("%s",f[i].data);
		
		if(i%2 == 0)
			f[i].frameno=0;
		
		else if(i%2!=0)
			f[i].frameno=1;
	}
	
	for(int i=0;i<N;i++)
	{
	
	
		printf("\nFRAME %d",i+1);
		printf("\nTRANSMISSION OPTION\n[0]WITHOUT error\n[1]WITH error\n\nEnter option: ");
		scanf("%d",&f[i].flag);
		
		sendto(sockfd, &f[i], sizeof(frame),MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
			
			
		n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
					
					 
		buffer[n] = '\0'; 
		printf("Server : %s\n", buffer);
	}	
	
	close(sockfd); 
	return 0; 
} 

