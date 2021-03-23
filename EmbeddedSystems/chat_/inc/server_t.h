#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"
#include "crypto.h"


#define MAX_CLIENTS 5

/**
*
*   Variables used in client file.
*   
**/

int** clients;
unsigned char *restrict_key = "atividadeQuatroArthur";      	// restrict key that client and server knows
int actualClient;                                               // integer that represents the actual client.
int keySize = 22;                             					// len of the restrict_key
EVP_CIPHER_CTX* ctx_en;                                         // CTX struct used to encrypt strings
EVP_CIPHER_CTX* ctx_de;                                         // CTX struct used to decrypt strings
unsigned int salt[] = {12345, 54321};
int serverPort;
const char* initMessage = "\n\n________Welcome to chatroom________";


typedef struct
{
	int sock;
	struct sockaddr address;
	int addr_len;
} connection_t;


void * process(void * ptr);


boolean validServerUserInput(int argc, char *argv[]);

void requestFromClient(connection_t * conn);

void delay(double dly);

void broadcastClients(unsigned char* msg);
