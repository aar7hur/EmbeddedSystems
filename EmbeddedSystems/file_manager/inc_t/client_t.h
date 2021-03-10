#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


int clientPort;


void commServer(int argc, char *argv[], int sock);


boolean validClientUserInput(int argc, char *argv[]);


