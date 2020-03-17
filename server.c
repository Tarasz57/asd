/*
	Bind socket to port 8888 on localhost
*/

#include<stdio.h>
#include<winsock2.h>
#include<string.h>

int main(int argc , char *argv[])
{
	WSADATA wsa;
	SOCKET s, incoming_socket;
	struct sockaddr_in server, client;
	int c, recv_size;
	char *message, *socket_message;
	message = (char*)calloc(13,sizeof(char));
	message = "I got it now";
	socket_message = (char*)calloc(100,sizeof(char));

	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	{
		printf("Failed. Error Code : %d",WSAGetLastError());
		return 1;
	}
	
	printf("Initialised.\n");
	
	//Create a socket
	if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d" , WSAGetLastError());
	}

	printf("Socket created.\n");
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8888);
	
	//Bind
	if( bind(s ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d" , WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	
	puts("Bind done");

	listen(s , 3);

	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	
	c = sizeof(struct sockaddr_in);


	incoming_socket = accept(s , (struct sockaddr *)&client, &c);

	if (incoming_socket == INVALID_SOCKET)
	{
		printf("accept failed with error code : %d" , WSAGetLastError());
		return 1;
	}

	puts("Connection accepted");

	while((recv_size = recv(incoming_socket , socket_message , 100 , 0)) != SOCKET_ERROR){
		if(recv_size == SOCKET_ERROR)
		{
			printf("recv failed with %d", WSAGetLastError());
			return 1;
		}
		if(strcmp(socket_message,"end") == 0){
			message = "Goodbye";
			send(incoming_socket , message, strlen(message) , 0);
			closesocket(s);
			WSACleanup();
			free(message);
			free(socket_message);
			return 0;
		}
		puts(socket_message);
		free(socket_message);
		socket_message = (char*)calloc(100,sizeof(char));

		send(incoming_socket , message , strlen(message) , 0);
	}
}