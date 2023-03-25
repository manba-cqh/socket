#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_LEN 1024

int main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("please input ip and port!\n");
        return 0;
    }

    int client_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_sock == -1) {
        printf("socket error!\n");
        return -1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, sizeof(server_addr), 0);
    server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=inet_addr(argv[1]);
	server_addr.sin_port=htons(atoi(argv[2]));

    socklen_t server_addr_len = sizeof(server_addr);
    if (connect(client_sock, (struct sockaddr*)&server_addr, server_addr_len) == -1) {
        printf("connect error!\n");
        return -1;
    }

    char message[BUFFER_LEN];
    int str_len = 0;
    int recv_len=0;
    while(1) {
        printf("please input send message:");
        fgets(message, BUFFER_LEN, stdin);
		
		if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))
			break;

		str_len=write(client_sock, message, strlen(message));
        if (str_len == -1) {
            printf("write error!\n");
            return -1;
        }
		
		while(recv_len<str_len)
		{
			int recv_cnt=read(client_sock, &message[recv_len], BUFFER_LEN-1);
			if(recv_cnt==-1) {
                printf("read error!\n");
                return -1;
            }
			recv_len+=recv_cnt;
		}
		
		message[recv_len]=0;
		printf("Message from server: %s", message);
    }

    close(client_sock);

    return 0;
}