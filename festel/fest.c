/*
**	********* Fest cipher ********** 
**	Basic Feistel network with key XOR'd after every iteration.
**
**
**	TODO:
**		- Add Documentation.
**		- Add README with diagramatic explaination.
**		- Decide a better name. Or not.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "fest.h"

int main(int argc, char* argv[])
{
	if(argc != 4)
	{
		printf("Usage: <Option> <Text> <Key>\n");
		if(argc > 1)
			if(!strcmp(argv[1],"h") || !strcmp(argv[1],"help") || !strcmp(argv[1],"-h") || !strcmp(argv[1],"--help"))
				printf("------HELP FOR FEST ENCRYPTION------\
				\n 1. For encryption with normal plain text as arguement\
				\n 2. For encryption with file as input (Recommended)\
				\n 3. For decryption with normal cipher text as arguement\
				\n 4. For decryption with file as input (Recommended)\
				\n h. For help.\
				\n Usage: <Option> <Text> <Key>\
				\n");		
		exit(0);
	}

	if(!strcmp(argv[1],"h") || !strcmp(argv[1],"help"))
	{
		printf("------HELP FOR FEST ENCRYPTION------\
				\n 1. For encryption with normal plain text as arguement\
				\n 2. For encryption with file as input (Recommended)\
				\n 3. For decryption with normal cipher text as arguement\
				\n 4. For decryption with file as input (Recommended)\
				\n h. For help.\
				\n Usage: <Option> <Text> <Key>\
				\n");
	}
	else if(!strcmp(argv[1],"1"))
	{
		encryptFest(argv[2],argv[3]);
	}
	else if(!strcmp(argv[1],"2"))
	{
		encryptFestFile(argv[2],argv[3]);
	}
	else if(!strcmp(argv[1],"3"))
	{
		decryptFest(argv[2],argv[3]);
	}
	else if(!strcmp(argv[1],"4"))
	{
		decryptFestFile(argv[2],argv[3]);
	}
	else
	{
		printf("Usage: <Option> <Text> <Key>\n");
	}

	return 0;
}

void encryptFest(char* pt, char* key)
{
	block* bks;
	block keybk = 0;
	int NO_BLOCKS = (int)ceil((float)strlen(pt)/4);
	int i;

	bks = (block*)malloc(NO_BLOCKS*sizeof(block));

	for(i=0;i<strlen(pt);i++)
	{
		bks[i/4] += pt[i];
		if((i+1)%4 != 0)
			bks[i/4] <<= 8;
	}

	while((i+1)%4 != 0)
	{
		bks[i/4] <<= 8;
		i++;
	}

	for(i=strlen(key);i>-1;i--)
		keybk += key[i] << (i * 8);

	printf("KEY: %04x\n",keybk);

	block* ctbks = encrypt(bks,keybk,NO_BLOCKS);

	printf("CipherText (HEX): ");

	for(i=0;i<NO_BLOCKS;i++)
		printf("%04x",ctbks[i]);			
	printf("\n");

	writeBlocks(ctbks, NO_BLOCKS, "ct.dat");

}

void encryptFestFile(char* f,char* key)
{
	FILE *fp = fopen(f,"rb");
	block* pt = readBlocks(fp);
	block* ct;
	block keybk = 0;
	int i;
	int NO_BLOCKS = getFileBlockSize(fp);

	for(i=strlen(key);i>-1;i--)
		keybk += key[i] << (i*8);

	ct = encrypt(pt,keybk,NO_BLOCKS);

	for(i=0;i<NO_BLOCKS;i++)
		printf("%04x",ct[i]);
	printf("\n");

	writeBlocks(ct, NO_BLOCKS, "ct.dat");
}

void decryptFest(char* ct, char* key)
{
	block* bks;
	block keybk = 0;
	int NO_BLOCKS = (int)ceil((float)strlen(ct)/4);
	int i;

	bks = (block*)malloc(NO_BLOCKS*sizeof(block));

	for(i=0;i<strlen(ct);i++)
	{
		bks[i/4] += ct[i];
		if((i+1)%4 != 0)
			bks[i/4] <<= 8;
	}

	while((i+1)%4 != 0)
	{
		bks[i/4] <<= 8;
		i++;
	}

	for(i=strlen(key);i>-1;i--)
		keybk += key[i] << (i * 8);

	printf("KEY: %04x\n",keybk);

	block* ctbks = decrypt(bks,keybk,NO_BLOCKS);

	printf("Plain Text (HEX): ");

	for(i=0;i<NO_BLOCKS;i++)
		printf("%04x",ctbks[i]);			// Cipher Text
	printf("\n");

	writeBlocks(ctbks, NO_BLOCKS, "pt.dat");
}

void decryptFestFile(char* f, char* key)
{
	FILE *fp = fopen(f,"rb");
	block* ct = readBlocks(fp);
	block keybk = 0;
	int i;
	int NO_BLOCKS = getFileBlockSize(fp);

	for(i=strlen(key);i>-1;i--)
		keybk += key[i] << (i*8);

	block* pt = decrypt(ct,keybk,NO_BLOCKS);

	/*
	for(i=0;i<NO_BLOCKS;i++)
		printf("%04x", pt[i]);
	printf("\n");
	*/

	writeBlocks(pt, NO_BLOCKS, "pt.dat");
}

void writeBlocks(block* text, int NO_BLOCKS, char f[])
{
	FILE *ct;
	int i;

	ct = fopen(f,"wb+");
	for(i=0;i<NO_BLOCKS;i++)
	{
		fwrite((const void*)&text[i], sizeof(block), 1, ct);
	}

	fclose(ct);
}

block* readBlocks(FILE* fp)
{
	int NO_BLOCKS = getFileBlockSize(fp);

	int* data = (int*)malloc(NO_BLOCKS*sizeof(int));

	int i;
	for(i=0;i<NO_BLOCKS;i++)
	{
		fread(&data[i],sizeof(int),1,fp);
	}

	return data;
}

int getFileBlockSize(FILE* fp)
{
	if(!fp)
		return 0;
	fseek(fp,0L,SEEK_END);
	int fsize = ftell(fp);
	rewind(fp);

	return (int)ceil((float)fsize/4);
}

block* encrypt(block* pt, block key, int NO_BLOCKS)
{
	sblock sb;
	int i;
	block* ctbks = (block*)malloc(NO_BLOCKS*sizeof(block));

	for(i=0;i<NO_BLOCKS;i++)
	{
		sblock left = 0, right = 0;
		block temp = 0;

		sb = pt[i] >> 16;
		
		left = sb ^ pt[i];
		right = sb;

		temp = concatSubks(left,right);
		temp ^= key;

		ctbks[i] = temp;
	}

	return ctbks;
}

block* decrypt(block* ct, block key, int NO_BLOCKS)
{
	sblock sb;
	block* ptbks = (block*)malloc(NO_BLOCKS*sizeof(block));
	int i;

	for(i=0;i<NO_BLOCKS;i++)
	{
		sblock left = 0, right = 0;
		block temp = ct[i] ^ key;
	
		left = temp;
	
		sb = temp >> 16;
		right = sb ^ left;
	
		temp = concatSubks(left,right);

		ptbks[i] = temp;
	}

	return ptbks;
}

block inline concatSubks(sblock s1, sblock s2)
{
	block res = 0;
	res += s1;
	res <<= 16;
	res += s2;
	return res; 
}