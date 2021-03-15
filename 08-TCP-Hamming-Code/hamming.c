/*
Program to generate the hamming code for the message at the sender end, introduce error manually(via user input) and check it at the receiver
*/

#include <stdio.h>
#include <math.h>
#include <string.h>

#define MAX 100

void hc_gen(int data[], int m)
{
	int hamming[MAX] = {0};
	int r, h;
	int i, j, k;
	int p;
	int parity;

	r = 0;

	/*Calculating the number of parity bits, r needed for the code
	r - number of parity bits*/
	while((m + r + 1)>(pow(2,r)))
        r++;

    printf("\nNumber of redundant bits needed is: %d", r);

	/*Set all the parity locations to -1
	and add the msg bits in the appropriate location*/

	// h - length of the hamming code with redundant bits
	h = r + m;

	// exponent for 2
	j = 0;

	// index for data
	k = 0;

	for(i=1;i<=h;i++)
	{
		p = pow(2,j);

		if(p == i)
		{
			hamming[i] = -1;
			j++;
		}

		else
		{
			hamming[i] = data[k];
			k++;
		}
	}

	/*
		//DEBUGGING
		printf("\nh:%d", h);
		for(i=1;i<=h;i++)
			printf("\ndata%d: %d", i, hamming[i]);
		printf("\n");
	*/

	/*Set the EVEN parity values for the redundant bits*/
	k = 0;
	int x, min, max = 0;

	for (i = 1; i <= h; i = pow (2, k))
	{
		k++;
		parity = 0;
		j = i;
		//x = i;
		min = 1;
		max = i;

		while(j <= h)
		{
			for(x = j; max >= min && x <= h; min++, x++)
			{
				if (hamming[x] == 1)
					parity = parity + 1;
			}

			j = x + i;
			min = 1;
		}

		//checking for even parity
		if (parity % 2 == 0)
			hamming[i] = 0;
		else
			hamming[i] = 1;
	}

	/*Need to display in reverse */
	printf("\nData with redundant bits(Hamming Code): ");
	for(i=h; i>=1; i--)
		printf("%d", hamming[i]);
	printf("\n");
}

void hc_err_detect(int hamming[], int h)
{
	int data[MAX] = {0};
	int r, m;
	int i, j, k;
	int parity;
	int error[MAX];
	int msg[MAX];
	r = 0;

	/*Calculating the number of parity bits, r needed for the code
	r - number of parity bits*/
	while((h + 1)>(pow(2,r)))
        r++;

	/*Set all the parity locations to -1
	and add the msg bits in the appropriate location*/

	// h - length of the data code with redundant bits
	m = h - r;

	/*
		//DEBUGGING
		printf("\nh:%d m:%d r:%d", h, m, r);
	*/

	int index = 1;

	/*Find the EVEN parity values for the redundant bits*/
	k = 0;
	int x, min, max = 0;

	for (i = 1; i <= h; i = pow (2, k))
	{
		k++;
		parity = 0;
		j = i;
		//x = i;
		min = 1;
		max = i;

		while(j <= h)
		{
			for(x = j; max >= min && x <= h; min++, x++)
			{
				if (hamming[x] == 1)
					parity = parity + 1;
			}

			j = x + i;
			min = 1;
		}

		//checking for even parity
		if (parity % 2 == 0)
			data[index++] = 0;
		else
			data[index++] = 1;
	}

	/*Need to reverse*/
	for(i=r, j=1; i>=1; i--, j++)
		error[j] = data[i];


	printf("\nCalulated redundant bits(error pos): ");
	for(i=1; i<=r;i++)
		printf("%d", error[i]);
	printf("\n");

	int c = r-1;
	int err_pos = 0;

	/*Converting the binary value in the error int array into the equivalent decimal index*/

	for(i=1; i<=r; i++)
		err_pos += (error[i] * pow(2, c--));

	/*DEBUG
	printf("\nError position: %d", err_pos);
	*/

	// 0 index means no error since all indexing in this program starts from 1 to match the indexing for hamming code which starts with 1
	if(err_pos != 0)
	{
		hamming[err_pos] = hamming[err_pos] == 0 ?1:0; // changing the 0 to 1 and vice-versa
		//printf("\nCorrected data: ");
	}
	/*
	else
		printf("Data: ");

	for(i=h; i>=1; i--)
		printf("%d", hamming[i]);
	printf("\n");
	*/


	/*Setting the redundant bit positions to -1*/
	int p;

	j = 0;

	for(i=1;i<=h;i++)
	{
		p = pow(2,j);

		if(p == i)
		{
			hamming[i] = -1;
			j++;
		}
	}

	/*DEBUG
	printf("-1 Data: ");
	for(i=h; i>=1; i--)
		printf("%d", hamming[i]);
	printf("\n");
	*/

	/*Retrieving the message from the hamming code without the redundant bits*/
	index = 1;
	for(i=1; i<=h; i++)
	{
		if (hamming[i] != -1)
			msg[index++] = hamming[i];
	}
	printf("Corrected Data: ");
	for(i=m; i>=1; i--)
		printf("%d", msg[i]);
	printf("\n");

}

int main()
{
	char msg[MAX];
	int data[MAX];
	int m;
	int i;
	int j;

	/*Reading the message or data to be sent*/

	printf("Input data: ");
	scanf("%s", msg);

	m = strlen(msg);

	/*Reversing the msg read
	arr indexing: 0 to len - 1
	msg bit read has indexing: len-1 to 0
	msb should be in array index len_of_array - 1 but will be in index 0
	To correct this, store the msg in data in the reversed order*/
	for(i=m-1, j=0; i>=0; i--, j++)
		data[i] = msg[j] - '0';

	/*
	//DEBUGGING
	printf("\nm:%d", m);
	for(i=0;i<m;i++)
		printf("\ndata%d: %d", i+1,data[i]);
	printf("\n");
	*/

	/*Calling the hamming code generator helper function
	to obtain the code sent by the Sender*/
	hc_gen(data, m);

	printf("Introduce error in data: ");
	scanf("%s", msg);

	m = strlen(msg);

	/*Reversing the msg read
	arr indexing: 0 to len - 1
	msg bit read has indexing: len-1 to 0
	msb should be in array index len_of_array - 1 but will be in index 0
	To correct this, store the msg in data in the reversed order*/
	for(i=m, j=0; i>=1; i--, j++)
		data[i] = msg[j] - '0';

	hc_err_detect(data, m);

	return 0;
}