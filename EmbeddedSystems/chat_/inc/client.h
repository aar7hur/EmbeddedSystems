#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"
#include "crypto.h"


/**
*
*   Variables used in client file.
*   
**/

int clientPort;                                              // used to hold client port
unsigned char *restrict_key = "atividadeQuatroArthur";      // restrict key that client and server knows
int keySize = 22;                                           // len of the restrict_key
EVP_CIPHER_CTX* ctx_en;                                         // CTX struct used to encrypt strings
EVP_CIPHER_CTX* ctx_de;                                         // CTX struct used to decrypt strings
unsigned int salt[] = {12345, 54321};


/**
 * 
 * 
 *  Function used to communicate with server.
 *  @param argc: Number of user input in CLI
 *  @param argv: An array of string that the user
 *              used in CLI
 * 
 **/

int commServer(int argc, char *argv[], int sock);

/**
 * 
 * 
 * Funcion used to check user input.
 * The user input format follow the following structure:
 *  <IP-ADDRESS> <PORT>.
 * @param argc: Number of user input in CLI
 * @param argv: An array of string that the user
 *              used in CLI
 *
 **/

boolean validClientUserInput(int argc, char *argv[]);


void *allMessages (void *sock);