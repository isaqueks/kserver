#ifndef RESPONSE_H
#define RESPONSE_H 1

// Pratically same thing as 'response.h' but for response
// polymorphism would be great now

#include "message.h"
#include "res_start_line.h"
#include "body.h"
#include "header.h"
#include <stdbool.h>

typedef struct {

    http_response_start_line_t start_line;
    http_message_t message;
    bool head_sent;

} http_response_t;

http_response_t* http_response_create();

void http_response_init(http_response_t* response);

int http_response_set_header(http_response_t*, char* name, char* value);

int http_response_write_body(http_response_t*, char* content, int length);

http_response_start_line_t* http_response_get_start_line(http_response_t* res);

http_message_t* http_response_get_message(http_response_t* res);

http_body_t* http_response_get_body(http_response_t* res);

http_header_t* http_response_get_header(http_response_t* res, char* name);

char* http_response_get_header_content(http_response_t* res, char* name);

int http_response_send_head(http_response_t* res, tcp_socket_t* socket);

int http_response_write_head(http_response_t* res, tcp_socket_t* socket, int status, char* status_message);

int http_response_body_flush(http_response_t* res, tcp_socket_t* socket);

int http_response_send_body(http_response_t* res, tcp_socket_t* socket, char* buffer, int len);

#endif