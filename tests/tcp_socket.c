#include <assert.h>
#include <stdlib.h>
#include "../include/net/tcp_socket.h"
#include "../include/errors/errors.h"

int main() {

    tcp_socket_t *sock = tcp_socket_create();
    assert(sock != NULL);

    int status = tcp_socket_connect(sock, "*.*.*.*", 80);
    assert(status == CONNECT_ERROR);

    assert(tcp_socket_bind_address(sock, INADDR_ANY, 3000) == 0);
    assert(sock->addr.sin_addr.s_addr == INADDR_ANY);
    assert(sock->addr.sin_port == htons(3000));


}