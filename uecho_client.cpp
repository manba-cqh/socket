/*
    udp回声客户端
*/

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

    int client_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (client_sock == -1) {
        printf("socket error!\n");
        return -1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, sizeof(server_addr), 0);
    server_addr.sin_family=PF_INET;
	server_addr.sin_addr.s_addr=inet_addr(argv[1]);
	server_addr.sin_port=htons(atoi(argv[2]));

    char message[BUFFER_LEN];
    socklen_t server_addr_len = sizeof(server_addr);
    while(1) {
        printf("please input send message:");
        fgets(message, BUFFER_LEN, stdin);
		
		if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))
			break;

        sendto(client_sock, message, strlen(message), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

        int recv_len = recvfrom(client_sock, message, BUFFER_LEN, 0, (struct sockaddr*)&server_addr, &server_addr_len);
		
		message[recv_len]=0;
		printf("Message from server: %s", message);
    }

    close(client_sock);

    return 0;
}