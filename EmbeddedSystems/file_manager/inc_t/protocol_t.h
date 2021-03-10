#include <stdio.h>
#include <string.h>    
#include <stdlib.h>    
#include <sys/socket.h>
#include <unistd.h>    
#include <sys/stat.h>


#define MAX_LEN_POPEN 1035

void ls_command(connection_t *conn);

void mkdir_command(char *directory, connection_t * conn);

void rename_command(char* oldname, char* newname, connection_t * conn);

void cd_command(char *directoryToEntry, connection_t *conn);


void rm_command(char *directory, connection_t * conn);