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

void setUp(struct sockaddr_in *server){
	server->sin_addr.s_addr = inet_addr("127.0.0.1");
    server->sin_family = AF_INET;
    server->sin_port = htons(8888);
}

void connectSocket(struct sockaddr_in server, SOCKET *s){
    if(connect(*s,(struct sockaddr *)&server,sizeof(server))<0){
        puts("connect error");
    }
    puts("Connected");
}

void cleanup(SOCKET *s, char *line, char *server_reply){
	send(*s, line, strlen(line),0);
	recv(*s,server_reply,100,0);
	puts(server_reply);
	closesocket(*s);
	WSACleanup();
	free(line);
	free(server_reply);
}

int beginTheGame(SOCKET *s){
	char *server_reply, *line;
    line = (char*)calloc(100,sizeof(char));
    server_reply = (char*)calloc(100,sizeof(char));
	int recv_size;
	
    puts("\nWhat should we send?");
    scanf("%s",line);

	while(strcmp(line,"end") != 0){
		//Send some data
		if( send(*s , line , strlen(line) , 0) < 0)
		{
			puts("Send failed");
			printf("%d",WSAGetLastError());
			return 1;
		}
		puts("Data Sent\n");
		// //Receive a reply from the server
		if((recv_size = recv(*s , server_reply , 100 , 0)) == SOCKET_ERROR)
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
	cleanup(s,line,server_reply);
	return 1;
}


int main(int argc , char *argv[])
{
	SOCKET s;
	struct sockaddr_in server;
    
	//a = (int*)calloc(n, sizeof(int));

	initialiseSocket();
	createSocket(&s);
	setUp(&server);
	connectSocket(server,&s);
	
	// setUp(&server);
	// connectSocket(&server,&s);

	if(beginTheGame(&s) == 1){
		return 0;
	}
}