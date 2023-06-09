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

    int server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_sock == -1) {
        printf("socket error!\n");
        return -1;
    }
    
    struct sockaddr_in server_addr;
    memset(&server_addr, sizeof(server_addr), 0);
    server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	server_addr.sin_port=htons(atoi(argv[1]));

    if (bind(server_sock, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        printf("bind error!\n");
        return -1;
    }

    if (listen(server_sock, 5) == -1) {
        printf("listen error!\n");
        return -1;
    }

    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    printf("wait connect,listening port: %s\n", argv[1]);
    int client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_addr_len);

    char buffer[BUFFER_LEN];
    int recv_len = 0;
    while((recv_len = read(client_sock, buffer, BUFFER_LEN)) != 0) {
        buffer[recv_len] = 0;
        printf("recv message: %s\n", buffer);
        write(client_sock, buffer, recv_len);
    }

    close(client_sock);
    close(server_sock);

    return 0;
}
