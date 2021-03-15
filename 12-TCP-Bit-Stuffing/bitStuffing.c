#include<stdio.h>
#include<string.h>

#define MAX 1024

/* This is a helper function for performing bit stuffing
data: user
stuffed: answer after performing bit stuffing
*/
void bitStuffing(char data[], char stuffed[])
{
	int i, j;
	int n = 0;
	
	for(i=0, j=0; i<strlen(data); i++,j++)
	{
	
		stuffed[j] = data[i];
		
		if(data[i] == '1')
		{
			n++;
		}
		
		else
		{
			n = 0;
		}
		
		if(n == 5)
		{
			stuffed[++j] = '0';
			n = 0;
		}
	}
	
	stuffed[j] = '\0';
}

/* This is a helper function for performing bit UNstuffing
stuffed: bit stuffed data
unstuffed: answer after performing bit uNstuffing
*/
void bitUnstuffing(char stuffed[], char unstuffed[])
{
	int i, j;
	int n = 0;
	
	for(i=0, j=0; i<strlen(stuffed); i++,j++)
	{
	
		unstuffed[j] = stuffed[i];
			
		if(stuffed[i] == '1')
		{
			n++;
		}
		
		else
		{
			n = 0;
		}
		
		if(n == 5)
		{
			i++;
			n = 0;
			continue;
		}
		
		
	}
	
	unstuffed[j]='\0';
}

// Driver
int main()
{
	char data[MAX];
	char stuffed[MAX];
	char unstuffed[MAX];

	printf("\nEnter the data: ");
	scanf("%s",data);

	printf("\nBEFORE Bit Stuffing: %s",data);
	
	bitStuffing(data, stuffed);
	
	printf("\nAFTER Bit stuffing : %s", stuffed);
	
	bitUnstuffing(stuffed, unstuffed);
	
	printf("\nAFTER Bit UNstuffing : %s", unstuffed);
	
	printf("\n");

	return 0;
}
