#include "../inc_t/main.h"
#include "../inc_t/client_t.h"



int main(int argc, char ** argv)
{
	int sock = -1;
	struct sockaddr_in address;
	struct hostent * host;
	int len;
	
	/* checking commandline parameter */
	if (validClientUserInput(argc, argv) == False)
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

	/* connect to server */
	address.sin_family = AF_INET;
	address.sin_port = htons(clientPort);
	host = gethostbyname(argv[1]);
	if (!host)
	{
		fprintf(stderr, "%s: error: unknown host %s\n", argv[0], argv[1]);
		return -4;
	}
	memcpy(&address.sin_addr, host->h_addr_list[0], host->h_length);
	if (connect(sock, (struct sockaddr *)&address, sizeof(address)))
	{
		fprintf(stderr, "%s: error: cannot connect to host %s\n", argv[0], argv[1]);
		return -5;
	}

	commServer(argc, argv, sock);
	
	return 0;
}

boolean validClientUserInput(int argc, char *argv[])
{
	int command = atoi(argv[3]);
   /* checking commandline parameter */
	if (argc < 4)
	{
		fprintf(stderr, "usage: hostname port text\n");
		return False;
	}
	if(command == MKDIR || command == RM || command == CD)
	{
		if(argv[4] == NULL)
		{
			fprintf(stderr, "Missing parameters\n");
			exit(EXIT_FAILURE);
		}
	}
	if(command == RENAME)
	{
		if((argv[4] == NULL || (argv[5] == NULL)))
		{
			fprintf(stderr, "Missing parameters\n");
			exit(EXIT_FAILURE);
		}
	}
	
	/* obtain port number */
	if (sscanf(argv[2], "%d", &clientPort) <= 0)
	{
		fprintf(stderr, "%s: error: wrong parameter: port\n", argv[0]);
		return False;
	}

}

void commServer(int argc, char *argv[], int sock)
{
	/* send text to server */
	int command = atoi(argv[3]);
	int len = 1;
	char *rxBuffer = (char*)calloc(200, sizeof(char));
	char *rxBufferT;

	// sends the data len to socket
	write(sock, &len, sizeof(int));

	// sends data to socket
	write(sock, argv[3], len);

	
	switch (command)
	{
		case MKDIR:
		case CD:
		case RM:
			// if cd, mkdir or rm:
			// only neeeds one parameter to work

			len = strlen(argv[4]);
			
			write(sock, &len, sizeof(int));

			write(sock, argv[4], len);
			break;
			
			
		case RENAME:

			// if rename, needs to send two parameters:
			// the name of the file/directory to rename and 
			// the new name
			
			len = strlen(argv[4]);

			write(sock, &len, sizeof(int));

			write(sock, argv[4], len);

			len = strlen(argv[5]);

			write(sock, &len, sizeof(int));

			write(sock, argv[5], len);
			break;
	
	}

	// LS does not need one conditional because only sends 1 string to the server.
	// It is coverage by the first write call.
	
	read(sock, &len, sizeof(int));
	printf("\n[INFO] I receive %d bytes from TCP PORT: %s.\n", len, argv[2]);

	rxBufferT = (char*)realloc(rxBuffer, len+1);

	read(sock, rxBufferT, len);

	printf("\[INFO] Result of the request to the server: \n\n%s\n", rxBufferT);

	/* close socket */
	close(sock);
}