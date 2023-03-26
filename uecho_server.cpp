/*
    udp回声服务端
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_LEN 1024

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("please input port!\n");
        return 0;
    }

    int server_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (server_sock == -1) {
        printf("socket error!\n");
        return -1;
    }
    
    struct sockaddr_in server_addr;
    memset(&server_addr, sizeof(server_addr), 0);
    server_addr.sin_family=PF_INET;
	server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	server_addr.sin_port=htons(atoi(argv[1]));

    if (bind(server_sock, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        printf("bind error!\n");
        return -1;
    }

    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_LEN] = {0};
    while(1) {
        int recv_len = recvfrom(server_sock, buffer, BUFFER_LEN, 0, (struct sockaddr*)&client_addr, &client_addr_len);
        if (recv_len >= 0) {
            buffer[recv_len] = 0;
            printf("recv from client: %s\n", buffer);
            sendto(server_sock, buffer, recv_len, 0, (struct sockaddr*)&client_addr, client_addr_len);
        }
    }

    close(server_sock);

    return 0;
}
