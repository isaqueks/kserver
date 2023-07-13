#ifndef REQUEST_H
#define REQUEST_H

#include "message.h"
#include "req_start_line.h"
#include "body.h"
#include "header.h"

typedef struct {

    http_request_start_line_t start_line;
    http_message_t message;

} http_request_t;

http_request_t* http_request_create();

void http_request_init(http_request_t* request);

int http_request_set_header(http_request_t*, char* name, char* value);

int http_request_write_body(http_request_t*, char* content, int length);

http_request_start_line_t* http_request_get_start_line(http_request_t* req);

http_message_t* http_request_get_message(http_request_t* req);

http_body_t* http_request_get_body(http_request_t* req);

http_header_t* http_request_get_header(http_request_t* req, char* name);

char* http_request_get_header_content(http_request_t* req, char* name);

#endif