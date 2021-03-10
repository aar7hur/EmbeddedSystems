#include "../inc_t/main.h"
#include "../inc_t/server_t.h"
#include "../inc_t/protocol_t.h"




void ls_command(connection_t *conn){

	FILE *fp;
    int len;
  	char *resultLs = (char*)malloc(200 *sizeof(char));
    char *auxString = (char*)malloc(200 *sizeof(char));

  	/* Open the command for reading. */
  	fp = popen("/bin/ls", "r");
  	if (fp == NULL) {
    	printf("Failed to run command\n" );
    	exit(1);
  	}

  	/* Read the output a line at a time - output it. */
  	while (fgets(auxString, sizeof(auxString), fp) != NULL) {
  	    strcat(resultLs, auxString);
  	}

    pclose(fp);


  	/* close */
    len = strlen(resultLs);

    write(conn->sock, &len, sizeof(int));

    write(conn->sock, resultLs, len); 	

    free(auxString);
    free(resultLs);
}

void cd_command(char *directoryToEntry, connection_t *conn){
	
    char *pathHere =  (char*)calloc(200, sizeof(char));
    char *pathToGo =  (char*)calloc(200, sizeof(char));
    char result[200];

    int len, status;

    getcwd(pathHere, 200);

	printf("[INFO] Current working directory: %s\n", pathHere);
	
    sprintf(pathToGo, "%s/%s", pathHere, directoryToEntry);

    printf("[INFO] Going to %s\n", pathToGo);

	status = chdir(pathToGo);
    if (status < 0 )
    {
        printf("[ERROR] Cannot move to directory. Does the directory already exist?\n");
        sprintf(result, "[ERROR] Cannot mnove to directory. Does the directory already exist?\n");
    }
    else if (status >= 0)
    {
        printf("[INFO] After command: %s\n", getcwd(pathHere, 200));
        sprintf(result, "[INFO] I'm in %s.\n", pathHere);
    }
	
	len = strlen(result);

    write(conn->sock, &len, sizeof(int));

    write(conn->sock, pathHere, len);

}


void mkdir_command(char *directory, connection_t * conn)
{
	char *resultMkdir = (char*)calloc(200, sizeof(char));
    int status, len;
	
	status = mkdir(directory, 0777);

    if (status < 0 )
    {
        printf("[ERROR] Cannot create directory. Does the file already exist?\n");
        strcpy(resultMkdir, "[ERROR] Cannot create directory. Does the file already exist?\n");
    }
    else if (status >= 0)
    {
        printf("[INFO] Directory create successfully.\n");
        strcpy(resultMkdir, "[INFO] Directory create successfully.\n");
    }

	len = strlen(resultMkdir);


    write(conn->sock, &len, sizeof(int));

    write(conn->sock, resultMkdir, len);
	
	
	free(directory);
    memset(resultMkdir, 0x00, 200);
    free(resultMkdir);
	
}


void rename_command(char* oldname, char* newname, connection_t * conn){
	
	int status, len;
	char *response =  (char*)calloc(200, sizeof(char));

	status = rename(oldname, newname);
	
	if(status==0){
		printf("[INFO] File/Directory renamed successfully.\n");
        strcpy(response, "[INFO] File/Directory renamed successfully.\n");
	}
	else{
		printf("[ERROR] Was not possible to rename file.\n");
        strcpy(response,"[ERROR] Was not possible to rename file.\n");
	}

	len = strlen(response);

    write(conn->sock, &len, sizeof(int));

    write(conn->sock, response, len);

    free(oldname);
    free(newname);
    free(response);
	
	
}

void rm_command(char *directory, connection_t * conn){
	
	int status, len;
	char *response =  (char*)calloc(200, sizeof(char));;
	printf("[INFO] Directory to delete: %s\n", directory);

    status = rmdir(directory);

    if(status==0){
		printf("[INFO] File/Directory deleted successfully.\n");
        strcpy(response, "[INFO] File/Directory deleted successfully.\n");
	}
	else{
		printf("[ERROR] Was not possible to delete file.\n");
        strcpy(response,"[ERROR] Was not possible to delete file.\n");
	}
	
	len = strlen(response);

    write(conn->sock, &len, sizeof(int));

    write(conn->sock, response, len);

    free(response);
    free(directory);
	
}
