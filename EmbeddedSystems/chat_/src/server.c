#include "../inc/server_t.h"




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

	// allocate memory for clients array
	clients = (int**)malloc(MAX_CLIENTS*sizeof(int));

	if (!clients)
	{
		perror("[ERROR] Couldnt allocate memory \n");
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
	if (listen(sock, 1) < 0)
	{
		fprintf(stderr, "%s: error: cannot listen on port\n", argv[0]);
		return -5;
	}

	// start array that represetns clients with all positions False.
	for (int indexClient = 0; indexClient < MAX_CLIENTS; indexClient++)
	{
		clients[indexClient] = False;
	}

	ctx_en =  EVP_CIPHER_CTX_new();
    ctx_de = EVP_CIPHER_CTX_new();
    if (ctx_init((unsigned char*)restrict_key, keySize, (unsigned char *)&salt, ctx_en, ctx_de)) {
        printf("[ERROR] Error in initializing AES Cipher\n");
        return -1;
    }

	printf("\n[INFO] Listening on TCP IP: %s\n", argv[1]);
	
	while (1)
	{
		// accept incoming connections 
		int len;
		unsigned char *ciphered;
		connection = (connection_t *)malloc(sizeof(connection_t));
		connection->sock = accept(sock, &connection->address, &connection->addr_len);
		if (connection->sock <= 0)
		{
			free(connection);
		}
		else
		{
			// If altready 5 clients connected
			if (actualClient == (MAX_CLIENTS -1))
			{
				printf("[ERROR] The server accepts only 5 clients at the same time. \n");
				close(connection->sock);
				free(connection);
			}
			else
			{	
				// Function scans the array and saves the current socket connection 
				// in the position that is not yet occuped
				for (int indexClient = 0 ; indexClient < MAX_CLIENTS; indexClient++)
				{
					if (clients[indexClient] == False)
					{
						clients[indexClient] = connection->sock;

						len = strlen(initMessage);

						// vcriptografh firt initial message
						ciphered = ctx_encrypt(ctx_en, (unsigned char *)initMessage, &len);

						len = strlen(ciphered);

						// write the size of start message into socket
						write(clients[indexClient], &len, sizeof(int));

						// now sends the raw data 
						write(clients[indexClient], ciphered, len);

						printf("[INFO] Sending ciphered start message: %u to client \n", ciphered);


						// holds the index of the actual client in an integer variable.
						// This variable will be used later to broadcast all messages.
						actualClient = indexClient;

						memset(ciphered, 0x00, strlen(ciphered));
						break;
					}
				}
				// start a new thread but do not wait for it 
				pthread_create(&thread, 0, process, (void *)connection);
			}
		}
	}

	// close all conections/pointer allocated
	close(connection->sock);
	free(connection);
	EVP_CIPHER_CTX_free(ctx_en);
    EVP_CIPHER_CTX_free(ctx_de);
	
	return EXIT_SUCCESS;
}

boolean validServerUserInput(int argc, char *argv[])
{

    // if number of CLI inputs is different from 3
    if (argc != 2)
    {
        fprintf(stderr, "[ERROR] Arguments are missing to run the application\n\n");
        return False;
    }

    /* obtain port number */
	if (sscanf(argv[1], "%d", &serverPort) <= 0)
	{
		fprintf(stderr, "%s: [ERROR] Wrong parameter: port\n", argv[0]);
		return False;
	}
	return True;

}

void * process(void * ptr)
{
	char *buffer, *txClient;
	int indexClient;
	char *oldname, *newName;
	unsigned char *notCiphered, *ciphered;
	int lenResponse;
	char *message = (char*)calloc(MAX_MESSAGE_SIZE, sizeof(char));
	connection_t * conn;
	long addr = 0;

	buffer = (char*)calloc(MAX_MESSAGE_SIZE, sizeof(char));


	if (!ptr) pthread_exit(0); 
	conn = (connection_t *)ptr;

	/* read length of message */
	read(conn->sock, &lenResponse, sizeof(int));
	if (lenResponse > 0)
	{
		addr = (long)((struct sockaddr_in *)&conn->address)->sin_addr.s_addr;
		if ((lenResponse > MAX_MESSAGE_SIZE))
		{
			buffer = (char*)realloc(buffer, lenResponse + 1);
		}

		memset(buffer, 0x00, strlen(buffer));
		memset(notCiphered, 0x00, strlen(notCiphered));
		memset(ciphered, 0x00, strlen(ciphered));
		memset(message, 0x00, strlen(message));

		/* read message */
		read(conn->sock, buffer, lenResponse);

		// decrypt message from client
		notCiphered = (char *)ctx_decrypt(ctx_de, (unsigned char*)buffer, &lenResponse);

		strcpy(message, buffer);

		lenResponse = strlen(message);
		ciphered = ctx_encrypt(ctx_en, (unsigned char *)message, &lenResponse);

		printf("[INFO] Sending ciphered message: %u to client \n", ciphered);
		broadcastClients(ciphered);
	}
	else
	{
		printf("erro \n\n");
	}
}



void broadcastClients(unsigned char* msg){
	int clientIndex = 0;
	printf("%u", msg);
	int messageLen = strlen((char *)msg);

	// scan clients array and sends all messages to clients
    for (clientIndex; clientIndex < MAX_CLIENTS; clientIndex++){
        if ((clients[clientIndex] != False) && (clientIndex != actualClient)){
			
			// firts send the size of the message
			write(clients[clientIndex], &messageLen, sizeof(int));

			// now sends the actually message
			write(clients[clientIndex], msg, messageLen);

    	}
	}
}


