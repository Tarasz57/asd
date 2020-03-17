#include<stdio.h>
#include<winsock2.h>

//gcc prog.c -o prog.exe [...other .c and .o files...] -lws2_32 to run

void initialiseSocket(){

	WSADATA wsa;

	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	{
		printf("Failed. Error Code : %d",WSAGetLastError());
		//return 1;
	}
	
	printf("Initialised.");
}

void createSocket(SOCKET *s){
	if((*s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d" , WSAGetLastError());
	}

	printf("Socket created.\n");
}


int main(int argc , char *argv[])
{
	SOCKET s;
	struct sockaddr_in server;
    char *server_reply, *line;
	int recv_size;
    line = (char*)calloc(100,sizeof(char));
    server_reply = (char*)calloc(100,sizeof(char));
	//a = (int*)calloc(n, sizeof(int));

	initialiseSocket();
	createSocket(&s);
	

	server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);

    puts("\nWhat should we send?");
    scanf("%s",line);

    if(connect(s,(struct sockaddr *)&server,sizeof(server))<0){
        puts("connect error");
        return 1;
    }
    puts("Connected");
	
	while(strcmp(line,"end") != 0){
		//Send some data
		if( send(s , line , strlen(line) , 0) < 0)
		{
			puts("Send failed");
			return 1;
		}
		puts("Data Sent\n");
		// //Receive a reply from the server
		if((recv_size = recv(s , server_reply , 100 , 0)) == SOCKET_ERROR)
		{
			puts("recv failed");
		}
		puts("Reply received\n");
		puts(server_reply);
		free(line);
		free(server_reply);
		line = (char*)calloc(100,sizeof(char));
		server_reply = (char*)calloc(100,sizeof(char));
		puts("\nWhat should we send?");
		scanf("%s",line);
	}

	send(s, line, strlen(line),0);
	recv(s,server_reply,100,0);
	puts(server_reply);
	
	// //Add a NULL terminating character to make it a proper string before printing
	// server_reply[recv_size] = '\0';
	// puts(server_reply);
    closesocket(s);
	WSACleanup();
	free(line);
	free(server_reply);

	return 0;
}