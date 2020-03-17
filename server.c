/*
	Bind socket to port 8888 on localhost
*/

#include<stdio.h>
#include<winsock2.h>
#include<string.h>
#include<time.h>

void initSocket(){
	WSADATA wsa;

	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	{
		printf("Failed. Error Code : %d",WSAGetLastError());
	}
	
	printf("Initialised.\n");
}

void createSocket(SOCKET *s){
	if((*s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d" , WSAGetLastError());
	}

	printf("Socket created.\n");
}

void setUp(struct sockaddr_in *server){
	server->sin_family = AF_INET;
	server->sin_addr.s_addr = INADDR_ANY;
	server->sin_port = htons(8888);
}

void bindSocket(SOCKET *s, struct sockaddr_in server){
	if( bind(*s ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d" , WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	
	puts("Bind done");
}

void waitForConnection(SOCKET *incomingSocket, SOCKET *s, struct sockaddr_in client){
	puts("Waiting for incoming connections...");
	int c;
	c = sizeof(struct sockaddr_in);
	*incomingSocket = accept(*s , (struct sockaddr *)&client, &c);

	if (*incomingSocket == INVALID_SOCKET)
	{
		printf("accept failed with error code : %d" , WSAGetLastError());
	}

	puts("Connection accepted\n");
}

void getANumber(int *c){
	srand ( time(NULL) );
	*c = rand() % 150 + 1;
	printf("%d",*c);
}

void cleanUp(SOCKET *incoming_socket, SOCKET *s, char *message, char *socket_message){
	message = "Goodbye";
	send(*incoming_socket , message, strlen(message) , 0);
	closesocket(*s);
	closesocket(*incoming_socket);
	WSACleanup();
	free(message);
	free(socket_message);
}

int startTheGame(SOCKET *incoming_socket, SOCKET *s, /*char *bufferedInt*/ int *createdInt){
	char *socket_message, *message;
	message = (char*)calloc(13,sizeof(char));
	socket_message = (char*)calloc(100,sizeof(char));
	int recv_size, number;
	while((recv_size = recv(*incoming_socket , socket_message , 100 , 0)) != SOCKET_ERROR){
		if(recv_size == SOCKET_ERROR)
		{
			printf("recv failed with %d", WSAGetLastError());
			return 1;
		}
		if(strncmp(socket_message,"end",3) == 0){
			cleanUp(incoming_socket,s,message,socket_message);
			return 0;
		}
		number = atoi(socket_message);
		if(number == *createdInt){
			message = "Correct";
			send(*incoming_socket , message, strlen(message) , 0);
		} else if (number < *createdInt){
			message = "Higher";
			send(*incoming_socket , message, strlen(message) , 0);
		
		} else {
			message = "Lower";
			send(*incoming_socket , message, strlen(message) , 0);
		}

		puts(socket_message);
		// if(strcmp())
		free(socket_message);
		free(message);
		socket_message = (char*)calloc(100,sizeof(char));
		message = (char*)calloc(13,sizeof(char));
	}
}


int main(int argc , char *argv[])
{
	
	SOCKET s, incoming_socket;
	struct sockaddr_in server, client;
	char *bufferedInt;
	int c;

	bufferedInt = (char*)calloc(4,sizeof(char));

	initSocket();
	createSocket(&s);
	setUp(&server);
	bindSocket(&s, server);
	

	listen(s , 3);

	waitForConnection(&incoming_socket,&s,client);
	// createANumber(bufferedInt);
	getANumber(&c);

	if(startTheGame(&incoming_socket,&s,&c) == 0){
		return 0;
	} else {
		printf("%d", WSAGetLastError());
		return 1;
	}
	
}