#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "app.h"
#include "socket_conf.h"

int main() {

    int server_socket; 
    struct sockaddr_in server_addr, client_addr; 
    char buffer[BUFFER_SIZE];
    socklen_t addr_len; 
    int n;

    // Criando o socket com o protocolo UDP (SOCK_DGRAM)
    server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (server_socket < 0) {
        perror("Socket create error");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    server_addr.sin_port = htons(PORT); 

    // Faz o bind(anexando o endereço e a porta do servidor no socket):
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind error");
        close(server_socket);
        exit(1);
    }

    //Ja fica na escuta, porque não é orientado a conexão, então já está pronto para receber requisicoes dos clientes
    printf("Server waiting for connections...\n");

    //Troca de mensagens:
    while (1) {
        addr_len = sizeof(client_addr);
        n = recvfrom(server_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len); //tem que ser recvfrom para identificar o endereço do cliente, já que podem haver vários no mesmo socket.
        if (n <= 0) {
            printf("Receive error.\n");
            close(server_socket);
            exit(1);
        }
        buffer[n] = '\0';
        printf("Received from client: %s\n", buffer);

        printf("Server: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        sendto(server_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&client_addr, addr_len); //envia pelo sendto() para destinar ao cliente específico
    }

    close(server_socket);
    return 0;
}
