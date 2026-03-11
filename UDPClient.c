
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "app.h"
#include "socket_conf.h"

int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <server_ip>\n", argv[0]);
        exit(1);
    }

    
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    int n;
    const char *server_ip = argv[1];


    // criacao do socket udp(SOCK_DGRAM) do cliente
    client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (client_socket < 0) {
        perror("Socket create error");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    server_addr.sin_port = htons(PORT); 

    // Manda a primeira requisição para conectar ao servidor
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Server connection error");
        close(client_socket);
        exit(1);
    }

    printf("Connection established with the server.\n");

    // Envia e recebe mensagens:
    while (1) {
        printf("Client: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        send(client_socket, buffer, strlen(buffer), 0);

        n = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (n <= 0) {
            printf("Connection closed by server.\n");
            break;
        }
        buffer[n] = '\0';
        printf("Server: %s", buffer);
    }

    close(client_socket);
    return 0;
}
