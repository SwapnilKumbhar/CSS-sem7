/*
** Header File for fest cipher.
** 
** Each block is 32 bytes 
** each sub block (sblock) is 16 bytes
*/
typedef unsigned int block;
typedef unsigned short sblock;


/*
**	Wrapper function for commandline input for encryption
*/
void encryptFest(char* pt, char* key);

/*
**	Wrapper function for file input for encryption
*/
void encryptFestFile(char* pt, char* key);

/*
**	Wrapper function for commandline input for decryption
*/
void decryptFest(char* ct, char* key);

/*
**	Wrapper function for file input for decryption
*/
void decryptFestFile(char* ct, char* key);

/*
**	Function for concatenating two sub blocks and returning the final 
**	concatenation of the two
*/
block concatSubks(sblock,sblock);

/*
**	Function to write blocks into a file. Last parameter is file name to be given
**	to the resultant file.
*/
void writeBlocks(block*,int,char[]);

/*
**	Function to read blocks from a file.
*/
block* readBlocks(FILE*);

/*
**	Function to get size of input file in blocks. Block size will be rounded off
**	to the next integer.
*/
int getFileBlockSize(FILE*);

/*
**	Base function that handles encryption.
*/
block* encrypt(block*,block,int);

/*
**	Base function that handles decryption.
*/
block* decrypt(block*,block,int);