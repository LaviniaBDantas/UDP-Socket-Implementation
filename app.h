#ifndef APP_H
#define APP_H

#include <arpa/inet.h>

// Server functions
int create_server_socket(int port);
int accept_client(int server_socket, struct sockaddr_in *client_addr);

// Client functions
int create_client_socket(const char *ip, int port);

//Send and Receive functions
void send_message(int socket, const char *message);
int receive_message(int socket, char *buffer, int size);

#endif