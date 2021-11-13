#ifndef SOCKET_TOOL
#define SOCKET_TOOL

#include <sys/socket.h>
#include <netdb.h>
#include <inttypes.h>

typedef int socketfd;

typedef struct {
    socketfd fd;
    socklen_t addr_len;
    struct sockaddr_in addr;
} tcp_socket_t;

tcp_socket_t* tcp_socket_create();
int tcp_socket_connect(tcp_socket_t* sock, char* ip, uint16_t port);

int tcp_socket_write(tcp_socket_t* sock, char* data, int len);
int tcp_socket_read(tcp_socket_t* sock, char* data, int len);

int tcp_socket_bind_address(tcp_socket_t* sock, in_addr_t addr, uint16_t port);
int tcp_socket_listen(tcp_socket_t* sock, int backlog);

int tcp_socket_accept(tcp_socket_t* sock, tcp_socket_t* out_client);

void tcp_socket_close(tcp_socket_t* sock);
void tcp_socket_free(tcp_socket_t* sock);

#endif