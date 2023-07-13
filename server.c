/*
    C socket server example
*/

#include <arpa/inet.h>  //inet_addr
#include <stdio.h>
#include <string.h>  //strlen
#include <sys/socket.h>
#include <unistd.h>  //write

#include "include/http/start_line.h";
#include "include/net/tcp_socket.h";

int validate_path(char* url) {
    if (strstr(url, "..") != NULL) {
        return 0;
    }
    if (strstr(url, "~") != NULL) {
        return 0;
    }
    return 1;
}

void sendfile(tcp_socket_t* client, char* filename) {

    printf("Sending %s\n", filename);

    #define BUFF_SIZE 1024*1024

    char response_head[2048];
    char response_body[BUFF_SIZE];

    if (!validate_path(filename)) {
        printf("Invalid url [%s]\n", filename);

        char* html = "<html><body><h1>400 Bad request</h1></body></html>";

        sprintf(response_head,
            "HTTP/1.0 400 Bad request\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %d\r\n\r\n",
        strlen(html));

        tcp_socket_write(client, response_head, strlen(response_head));
        tcp_socket_write(client, html, strlen(html));
    }
    else {
        
        char filepath[128];
        strcpy(filepath, "./static");
        strcat(filepath, filename);
        FILE* fp = fopen(filepath, "rb");

        if (!fp) {

            printf("Not found [%s]\n", filepath);

            char* html = "<html><body><h1>404 Not Found</h1></body></html>";

            sprintf(response_head,
                "HTTP/1.0 404 Not found\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: %d\r\n\r\n",
            strlen(html));

            tcp_socket_write(client, response_head, strlen(response_head));
            tcp_socket_write(client, html, strlen(html));

        }
        else {
            fseek(fp, 0L, SEEK_END);
            long sz = ftell(fp);
            fseek(fp, 0L, SEEK_SET);

            char ftype[128];
            if (strstr(filename, ".html")) {
                strcpy(ftype, "text/html");
            }
            else if (strstr(filename, ".css")) {
                strcpy(ftype, "text/css");
            }
            else if (strstr(filename, ".js")) {
                strcpy(ftype, "application/javascript");
            }
            else if (strstr(filename, ".png")) {
                strcpy(ftype, "image/png");
            }
            else if (strstr(filename, ".jpg")) {
                strcpy(ftype, "image/jpeg");
            }
            else if (strstr(filename, ".gif")) {
                strcpy(ftype, "image/gif");
            }
            else if (strstr(filename, ".svg")) {
                strcpy(ftype, "image/svg+xml");
            }
            else {
                strcpy(ftype, "text/plain");
            }

            sprintf(response_head,
                "HTTP/1.0 200 OK\r\n"
                "Content-Type: %s\r\n"
                "Content-Length: %lu\r\n\r\n",
            ftype, sz);

            tcp_socket_write(client, response_head, strlen(response_head));
            int read = 0;
            while ((read = fread(response_body, 1, BUFF_SIZE, fp)) > 0) {
                tcp_socket_write(client, response_body, read);
            }
            if (read < 0) {
                perror("fread");
                fclose(fp);
                tcp_socket_close(client);
            }

            fclose(fp);
        }
    // tcp_socket_close(client);
    }
}

int main(int argc, char* argv[]) {
    tcp_socket_t* server = tcp_socket_create();
    if (server < 0) {
        printf("Error creating socket\n");
        return 1;
    }

    if (tcp_socket_bind_address(server, INADDR_ANY, 8888) < 0) {
        printf("Error binding socket\n");
        return 1;
    }

    if (tcp_socket_listen(server, 100) < 0) {
        printf("Error listening\n");
        return 1;
    }

    char client_buffer[2048];
    http_request_start_line_t start_line;

    while (1) {
        tcp_socket_t client;
        if (tcp_socket_accept(server, &client) < 0) {
            printf("Error handling client\n");
            perror("");
            continue;
        }
        // memset(http_head, 0, 256);

        int bufsize = 2048;
        if (bufsize = tcp_socket_read(&client, client_buffer, bufsize) < 0) {
            printf("Error reading from client\n");
            continue;
        }
        if (bufsize == 0) {
            bufsize = 2048;
        }


        int s = 0;
        if ((s = http_req_start_line_parse(&start_line, client_buffer, bufsize)) < 0) {
            printf("Error parsing start line (%d)\n", s);
            continue;
        }

        for (int i = 0; i < s; i++) {
            printf("%c", client_buffer[i]);
        }
        printf("\n");

        if (strcmp(start_line.url, "/end") == 0) {
            tcp_socket_close(&client);
            tcp_socket_close(&server);
            return 0;
        }

        sendfile(&client, start_line.url);

    }

    return 0;
}