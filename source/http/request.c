
#include <stdlib.h>
#include "../../include/http/request.h"

http_request_t* http_request_create() {
    http_request_t* request = (http_request_t*)malloc(sizeof(http_request_t));
    if (request == NULL) {
        return NULL;
    }
    
    http_request_init(request);

    return request;
}

void http_request_init(http_request_t* request) {
    http_request_start_line_init(&request->start_line);
    http_message_init(&request->message);
}

int http_request_set_header(http_request_t* req, char* name, char* value) {
    return http_message_set_header(http_request_get_message(req) , name, value);
}

int http_request_write_body(http_request_t* req, char* content, int length) {
    return http_body_append(http_request_get_body(req), content, length);
}

http_request_start_line_t* http_request_get_start_line(http_request_t* req) {
    return &req->start_line;
}

http_message_t* http_request_get_message(http_request_t* req) {
    return &req->message;
}

http_body_t* http_request_get_body(http_request_t* req) {
    return http_message_get_body(&req->message);
}

http_header_t* http_request_get_header(http_request_t* req, char* name) {
    return http_message_get_header(&req->message, name, false);
}

char* http_request_get_header_content(http_request_t* req, char* name) {
    return http_message_get_header_content(http_request_get_message(req), name);
}