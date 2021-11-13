/*
    C socket server example
*/

#include <arpa/inet.h>  //inet_addr
#include <stdio.h>
#include <string.h>  //strlen
#include <sys/socket.h>
#include <unistd.h>  //write
#include "net/tcp_socket.h";

int main(int argc, char *argv[]) {
    
    tcp_socket_t* server = tcp_socket_create();
    if (server < 0) {
        printf("Error creating socket\n");
        return 1;
    }

    if (tcp_socket_bind_address(server, INADDR_ANY, 8888) < 0) {
        printf("Error binding socket\n");
        return 1;
    }

    if (tcp_socket_listen(server, 5) < 0) {
        printf("Error listening\n");
        return 1;
    }

    char http_head[2048];
    while(1) {
        tcp_socket_t client;
        if (tcp_socket_accept(server, &client) < 0) {
            printf("Error handling client\n");
            perror("");
            continue;
        }
        memset(http_head, 0, 2048);

        char* html = "<h1>Welcome to the ANSI-C Web server!</h1><hr>";
        sprintf(http_head, 
        "HTTP/1.0 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: %d\r\n",
        strlen(html));

        tcp_socket_write(&client, http_head, strlen(http_head));
        tcp_socket_write(&client, "\r\n", 2);
        tcp_socket_write(&client, html, strlen(html));
        tcp_socket_close(&client);
    }

    return 0;
}