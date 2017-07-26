#include <stdio.h>
#include <string.h>

int main()
{
	char msg[50];
	int i;
	int ch;

	printf("------CAESAR CIPHER-------\
		\n1. Encrypt\
		\n2. Decrypt\
		\nChoice: ");
	scanf("%d",&ch);

	switch(ch)
	{
		case 1:
			printf("Enter plain text: ");
			scanf("%s",msg);
			for(i=0;i<strlen(msg);i++)
			{
				msg[i]+=3;
				if((msg[i]>90 && msg[i]<101)||msg[i]>122)
					msg[i]-=26;
			}
			printf("Encrypted: %s\n",msg);
			break;
		case 2:
			printf("Enter cipher text: ");
			scanf("%s",msg);
			for(i = 0; i < strlen(msg); i++)
			{
				msg[i]-=3;
				if((msg[i]<65) || (msg[i]>90 && msg[i]<97))
					msg[i]+=26;
			}
			printf("Decrypted: %s\n",msg);
	}

	return 0;
}