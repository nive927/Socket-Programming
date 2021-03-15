#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>  

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
	char *ack = "ACK";
	char *noack="NACK";
	
	frame f[5];
	frame f1;
	
	for(int i=0;i<N;i++)
	{
		printf("\nEnter DATAfor Frame%d [8 Bit]: ",(i+1));
		scanf("%s",f[i].data);
		
		if(i%2 == 0)
			f[i].frameno=0;
		
		else if(i%2!=0)
			f[i].frameno=1;
	}
	
	for(int i=0; i<N; i++)
	{
		printf("\nFRAME %d",i+1);
		printf("\nTRANSMISSION OPTION\n[0]WITHOUT error\n[1]WITH error\n\nEnter option: ");
		scanf("%d",&f[i].flag);
		//sendto - server			
			
		//recvfrom - server
					
					 
		//buffer[MAX] = '\0'; 
		//printf("Server : %s\n", buffer); 
	}

	for(int i=0; i<N; i++)
	{
	
		f1 = f[i];
		printf("\n\nFRAME DETAILS");
		printf("\nFrame num:");
		printf("%d",f1.frameno);
		printf("\nData: ");
		printf("%s",f1.data);
		if(f1.flag == 0)
		{
			printf("\n%s",ack);
			//sendto - client - ACK
		}

		else if(f1.flag == 1)
		{
			printf("\n%s",noack);
			//sendto - client -NOACK  
		}
	}
	
	printf("\n");
		 
	return 0;
}


