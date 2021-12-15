#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "net/tcp_socket.h"

#define MAX 80
#define PORT 80

#define REQ_NEW_LINE "\r\n"
#define HEADER(name, content) name ": " content REQ_NEW_LINE
#define REQ(method, path) method " " path " HTTP/1.1" REQ_NEW_LINE

void func(tcp_socket_t* sock)
{

    char* request = 
    REQ("GET", "/phpforum/")
    HEADER("Connection", "close")
    HEADER("Host", "www.php-forum.com")
    HEADER("Pragma", "no-cache")
    HEADER("Upgrade-Insecure-Requests", "0")
    HEADER("Accept", "text/html")
    HEADER("User-Agent", "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/95.0.4638.69 Safari/537.36")
    REQ_NEW_LINE;

    printf("=== <REQUEST> ===\n%s", request);

    // write(sockfd, http, strlen(http));
    // send(sockfd, request, strlen(request), 0);
    tcp_socket_write(sock, request, strlen(request));

    printf("request sent\n\n");

    char response[1024*1024];
    bzero(response, 1024*1024);
    int valread;
    while(valread = tcp_socket_read(sock, response, 1024*1024) > 0) {
        printf("%s", response);
        bzero(response, 1024*1024);
    }
    printf("\n\n%i", valread);
}

int main()
{
	tcp_socket_t* sock = tcp_socket_create();
    if (sock < 0 ) {
        printf("error creating socket\n");
        return -1;
    }
    if (tcp_socket_connect(sock, "51.15.158.208", 80) < 0) {
        printf("error connecting to server\n");
        return -1;
    }

    func(sock);

    tcp_socket_close(sock);
    tcp_socket_free(sock);

    return 0;

}
