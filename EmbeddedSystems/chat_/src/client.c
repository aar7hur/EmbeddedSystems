#include "../inc/client.h"




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
		fprintf(stderr, "[ERROR] %s: Unknown host %s\n", argv[0], argv[1]);
		return -4;
	}
	memcpy(&address.sin_addr, host->h_addr_list[0], host->h_length);
	if (connect(sock, (struct sockaddr *)&address, sizeof(address)))
	{
		fprintf(stderr, "[ERROR] %s : cannot connect to host %s\n", argv[0], argv[1]);
		return -5;
	}

	fprintf(stdout, "\n[INFO] Client in online. Sending/listening on port %d\n", clientPort);

	commServer(argc, argv, sock);

	return EXIT_SUCCESS;
}

boolean validClientUserInput(int argc, char *argv[])
{
	
   /* checking commandline parameter */
	if (argc != 3)
	{
		fprintf(stderr, "[ERROR] Usage: hostname port\n");
		return False;
	}
	
	/* obtain port number */
	if (sscanf(argv[2], "%d", &clientPort) <= 0)
	{
		fprintf(stderr, "[ERROR] %s: Wrong parameter: port\n", argv[0]);
		return False;
	}

}

int commServer(int argc, char *argv[], int sock)
{
	/* send text to server */
	int len, cryptoStatus, cipherLen;
	char *rxBuffer = (char*)calloc(MAX_MESSAGE_SIZE, sizeof(char));
	char *txBuffer = (char*)calloc(MAX_MESSAGE_SIZE, sizeof(char));
	unsigned char *cipher;
	char *decryptText, *name;
	pthread_t thread;
	int nameWasSet = False;

	ctx_en =  EVP_CIPHER_CTX_new();
    ctx_de = EVP_CIPHER_CTX_new();

	cryptoStatus = ctx_init(restrict_key, keySize, (unsigned char *)&salt, ctx_en, ctx_de);
	if (cryptoStatus == 1)
	{
		printf("[ERROR] Error in initializing AES Cipher \n");
		exit(EXIT_FAILURE);
	}
	
	// read how many bytes server will send to client
	read(sock, &len, sizeof(int));

	// if at least one bytes received from server
	if (len > 0)
	{
		// If receives more bytes than MAX_MESSAGE_SIZE
		// allocates more bytes to holds the message
		if (len > MAX_MESSAGE_SIZE)
		{
			rxBuffer = (char*)realloc(rxBuffer, len + 1);
		}
		// actually read the data from the server
		read(sock, rxBuffer, len);

		// decrypt the data from server and holds into decryptText 
		decryptText = (char*)ctx_decrypt(ctx_de, (unsigned char*)rxBuffer, &len);
		
		fprintf(stdout, "[INFO] Server sent a first command: %s \n", decryptText);	
	}

	while (True)
	{
		
		// erase all allocated memory
		memset(rxBuffer, 0x00, strlen(rxBuffer));
		memset(txBuffer, 0x00, strlen(txBuffer));

		if (nameWasSet == False)
		{
			printf("[INFO] Please enter your name: \n");
			nameWasSet = True;
			fgets(name, MAX_MESSAGE_SIZE, stdin);
			continue;
		}

		// gets user input
		fgets(rxBuffer, MAX_MESSAGE_SIZE, stdin);

		sprintf(txBuffer, "%s -> %s", name, rxBuffer);

		len = strlen(txBuffer);
		// cypher user message
		cipher = ctx_encrypt(ctx_en, (unsigned char*)txBuffer, &len);

		// gets the cypher len to send to server
		cipherLen = strlen(cipher);

		// send the amount of data in bytes that client will send
		// to the server
		write(sock, &cipherLen, sizeof(int));

		// actually send data to server
		write(sock, cipher, cipherLen);

		if (nameWasSet == True)
		{
			pthread_create(&thread, NULL, allMessages,  (void *)sock);
		}

	
		memset(cipher, 0x00, strlen(cipher));
		memset(txBuffer, 0x00, strlen(txBuffer));
		memset(rxBuffer, 0x00, strlen(rxBuffer));
	
	}
	return EXIT_SUCCESS;

}

void *allMessages (void *sock)
{
	int* socket = (int*)sock;
	int len;
	char *rxBuff = (char*)calloc(MAX_MESSAGE_SIZE, sizeof(char));
	char * decryptText;
	if (read(socket, &len, sizeof(int)) > 0)
	{
		// actually read the data from the server
		read(socket, rxBuff, len);

		// decrypt the data from server and holds into decryptText 
		decryptText = (char*)ctx_decrypt(ctx_de, (unsigned char*)rxBuff, &len);
			
		fprintf(stdout, "[INFO] %s \n", decryptText);	
	}
	return NULL;
}
