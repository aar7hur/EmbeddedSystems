#include "../inc_t/main.h"
#include "../inc_t/server_t.h"
#include "../inc_t/protocol_t.h"




int main(int argc, char ** argv)
{
	int sock = -1;
	struct sockaddr_in address;
	connection_t * connection;
	pthread_t thread;

	if (validServerUserInput(argc, argv) == False)
	{
		exit(EXIT_FAILURE);
	}

	/* create socket */
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock <= 0)
	{
		fprintf(stderr, "%s: error: cannot create socket\n", argv[0]);
		return -3;
	}

	/* bind socket to port */
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(serverPort);
	if (bind(sock, (struct sockaddr *)&address, sizeof(struct sockaddr_in)) < 0)
	{
		fprintf(stderr, "%s: error: cannot bind socket to port %d\n", argv[0], serverPort);
		return -4;
	}

	/* listen on port */
	if (listen(sock, 5) < 0)
	{
		fprintf(stderr, "%s: error: cannot listen on port\n", argv[0]);
		return -5;
	}

	printf("\n %s Listening on TCP IP: %s\n", argv[0], argv[1]);
	
	while (1)
	{
		/* accept incoming connections */
		connection = (connection_t *)malloc(sizeof(connection_t));
		connection->sock = accept(sock, &connection->address, &connection->addr_len);
		if (connection->sock <= 0)
		{
			free(connection);
		}
		else
		{
			/* start a new thread but do not wait for it */
			pthread_create(&thread, 0, process, (void *)connection);
			pthread_detach(thread);
		}
	}
	
	return 0;
}

boolean validServerUserInput(int argc, char *argv[])
{

    // if number of CLI inputs is different from 3
    if (argc != 2)
    {
        fprintf(stderr, "Arguments are missing to run the application\n\n");
        return False;
    }

    /* obtain port number */
	if (sscanf(argv[1], "%d", &serverPort) <= 0)
	{
		fprintf(stderr, "%s: error: wrong parameter: port\n", argv[0]);
		return False;
	}
	return True;

}

void * process(void * ptr)
{
	char * buffer, *txClient;
	char *oldname, *newName;
	int lenResponse, command;
	connection_t * conn;
	long addr = 0;

	char *bufferT = (char*)calloc(200, sizeof(char));

	if (!ptr) pthread_exit(0); 
	conn = (connection_t *)ptr;

	/* read length of message */
	read(conn->sock, &lenResponse, sizeof(int));
	if (lenResponse > 0)
	{
		addr = (long)((struct sockaddr_in *)&conn->address)->sin_addr.s_addr;
		buffer = (char *)malloc((lenResponse+1)*sizeof(char));
		memset(buffer, 0x0, lenResponse);

		/* read message */
		read(conn->sock, buffer, lenResponse);

		command = atoi(buffer);

		free(buffer);
		
		switch (command)
		{
		case LS:
			// if server receives ls command

			printf("\n[INFO] I received LS command.");
			printf("\n[INFO] Sending response to client.\n");
			
			// sending ls to terminal and get the response
			ls_command(conn);

			break;
		
		case MKDIR:

			read(conn->sock, &lenResponse, sizeof(int));

			memset(bufferT, 0x00, 200);

			read(conn->sock, bufferT, lenResponse);

			printf("\n[INFO] I received MKDIR command.");
			printf("\n[INFO] Creating directory.\n");
			mkdir_command(bufferT, conn);
			break;
		
		case RENAME:
			
			read(conn->sock, &lenResponse, sizeof(int));

			oldname = (char*)malloc(lenResponse * sizeof(char));

			read(conn->sock, oldname, lenResponse);

			read(conn->sock, &lenResponse, sizeof(int));

			newName = (char*)malloc(lenResponse * sizeof(char));

			read(conn->sock, newName, lenResponse);

			printf("\n[INFO] I received Rename command.");
			printf("\n[INFO] Renaming directory\n");

			rename_command(oldname, newName, conn);
			break;

		case CD:
			
			read(conn->sock, &lenResponse, sizeof(int));

			memset(bufferT, 0x00, 200);

			read(conn->sock, bufferT, lenResponse);

			printf("\n[INFO] I received CD command.");
			printf("\n[INFO] Moving to directory.\n");
			cd_command(bufferT, conn);
			break;
		
		case RM:

			read(conn->sock, &lenResponse, sizeof(int));

			memset(bufferT, 0x00, 200);

			read(conn->sock, bufferT, lenResponse);

			printf("\n[INFO] I received Remove command.");
			printf("\n[INFO] Removing directory/file.\n");
			rm_command(bufferT, conn);
			break;
		}

	}
	
	/* close socket and clean up */
	close(conn->sock);
	free(conn);
	pthread_exit(0);
}


void delay(double dly){
    /* save start time */
    const time_t start = time(NULL);

    time_t current;
    do{
        /* get current time */
        time(&current);

        /* break loop when the requested number of seconds have elapsed */
    }while(difftime(current, start) < dly);
}