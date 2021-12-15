#include <sys/socket.h>
#include <netdb.h>
#include <inttypes.h>
#include <stdlib.h>
#include "../../include/errors/errors.h"
#include "../../include/net/tcp_socket.h"

tcp_socket_t* tcp_socket_create() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        return CREATE_SOCKET_ERROR;
    }
    tcp_socket_t* sock = malloc(sizeof(tcp_socket_t));
    if (sock == NULL) {
        return CREATE_SOCKET_ERROR;
    }

    getsockname(sockfd, (struct sockaddr*)&sock->addr, &sock->addr_len);
    sock->fd = sockfd;

    return sock;
}

int tcp_socket_connect(tcp_socket_t* sock, char* ip, uint16_t port) {
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(ip);
	servaddr.sin_port = htons(port);

	// connect the client socket to server socket
	if (connect(sock->fd, &servaddr, sizeof(servaddr)) != 0) {
		return CONNECT_ERROR;
	}

    return 0;
}

int tcp_socket_write(tcp_socket_t* sock, char* data, int len) {
    return write(sock->fd, data, len);
}

int tcp_socket_read(tcp_socket_t* sock, char* data, int len) {
    return read(sock->fd, data, len);
}


int tcp_socket_bind_address(tcp_socket_t* sock, in_addr_t addr, uint16_t port) {
    sock->addr_len = sizeof(sock->addr);
    sock->addr.sin_family = AF_INET;
    sock->addr.sin_addr.s_addr = addr;
    sock->addr.sin_port = htons(port);

    if (bind(sock->fd, (struct sockaddr*)&sock->addr, sock->addr_len) < 0) {
        return ERROR;
    }

    return 0;
}

int tcp_socket_listen(tcp_socket_t* sock, int backlog) {
    if (listen(sock->fd , backlog) < 0) {
        return ERROR;
    }
    return 0;
}

int tcp_socket_accept(tcp_socket_t* sock, tcp_socket_t* out_client) {
    out_client->addr_len = sizeof(struct sockaddr_in);
    int client_sock = accept(sock->fd, (struct sockaddr *)&out_client->addr, (socklen_t*)&out_client->addr_len);
    if (client_sock < 0) {
        return ERROR;
    }
    out_client->fd = client_sock;
    return 0;
}


void tcp_socket_close(tcp_socket_t* sock) {
    close(sock->fd);
}

void tcp_socket_free(tcp_socket_t* sock) {
    free(sock);
}