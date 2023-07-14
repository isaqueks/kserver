
#include <stdlib.h>
#include "../../include/errors/errors.h"
#include "../../include/net/tcp_socket.h"
#include "../../include/http/response.h"
#include <stdio.h>

#define RESPONSE_BUF_SIZE 1024

http_response_t* http_response_create() {
    http_response_t* response = (http_response_t*)malloc(sizeof(http_response_t));
    if (response == NULL) {
        return NULL;
    }
    
    http_response_init(response);

    return response;
}

void http_response_init(http_response_t* response) {
    response->head_sent = false;
    http_res_start_line_init(&response->start_line);
    http_message_init(&response->message);
}

int http_response_set_header(http_response_t* res, char* name, char* value) {
    return http_message_set_header(http_response_get_message(res) , name, value);
}

int http_response_write_body(http_response_t* res, char* content, int length) {
    return http_body_append(http_response_get_body(res), content, length);
}

http_response_start_line_t* http_response_get_start_line(http_response_t* res) {
    return &res->start_line;
}

http_message_t* http_response_get_message(http_response_t* res) {
    return &res->message;
}

http_body_t* http_response_get_body(http_response_t* res) {
    return http_message_get_body(http_response_get_message(res));
}

http_header_t* http_response_get_header(http_response_t* res, char* name) {
    return http_message_get_header(http_response_get_message(res), name, false);
}

char* http_response_get_header_content(http_response_t* res, char* name) {
    return http_message_get_header_content(http_response_get_message(res), name);
}

int http_response_send_head(http_response_t* res, tcp_socket_t* socket) {
    if (res->head_sent) {
        return HEAD_ALREADY_SENT_ERROR;
    }

    char buffer[RESPONSE_BUF_SIZE];

    int flag = 0;

    http_response_start_line_t* line = http_response_get_start_line(res);
    if ((flag = http_res_start_line_output(line, buffer, RESPONSE_BUF_SIZE)) < 0) {
        return flag;
    }

    if ((flag = tcp_socket_write(socket, buffer, flag - 1)) < 0) {
        return flag;
    }

    for (int i = 0; i < HTTP_MESSAGE_HEADERS; i++) {
        http_header_t* header = &res->message.header[i];
        if (strlen(http_header_get_name(header)) == 0) {
            continue;
        }
        if ((flag = http_header_output(header, buffer, RESPONSE_BUF_SIZE)) < 0) {
            return flag;
        }
        if ((flag = tcp_socket_write(socket, buffer, flag)) < 0) {
            return flag;
        }
    }

    if ((flag = tcp_socket_write(socket, "\r\n", 2)) < 0) {
        return flag;
    }

    res->head_sent = true;

    return flag;
}

int http_response_write_head(http_response_t* res, tcp_socket_t* socket, int status, char* status_message) {
    if (res->head_sent) {
        return HEAD_ALREADY_SENT_ERROR;
    }

    http_response_start_line_t* line = http_response_get_start_line(res);
    http_res_start_line_set_status_code(line, status);
    http_res_start_line_set_status_message(line, status_message);

    return http_response_send_head(res, socket);
}

int http_response_body_flush(http_response_t* res, tcp_socket_t* socket) {
    return http_body_flush(http_response_get_body(res), socket);
}

int http_response_send_body(http_response_t* res, tcp_socket_t* socket, char* buffer, int len) {
    int ptr = 0;

    while (true) {
        int ret = http_body_append(http_response_get_body(res), &buffer[ptr], len > MAX_HTTP_BODY_SIZE ? MAX_HTTP_BODY_SIZE : len);
        if (ret < 0) {
            return ret;
        }
        ptr += ret;
        len -= ret;

        ret = http_response_body_flush(res, socket);
        if (ret < 0) {
            return ret;
        }

        if (len == 0) {
            return ptr;
        }
    }
}