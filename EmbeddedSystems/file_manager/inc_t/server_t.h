#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <time.h>



int serverPort;


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