#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "../../include/errors/errors.h"
#include "../../include/http/start_line.h"
#include "../../include/http/header.h"
#include "../../include/http/body.h"
#include "../../include/http/message.h"
#include "../../include/net/tcp_socket.h"

http_message_t* http_message_create() {
    http_message_t* message = (http_message_t*)malloc(sizeof(http_message_t));
    if (message == NULL) {
        return NULL;
    }
    
    http_message_init(message);

    return message;
}


void http_message_init(http_message_t* message) {
    http_start_line_init(&message->start_line);
    for (int i = 0; i < HTTP_MESSAGE_HEADERS; i++) {
        http_header_init(&message->header[i]);
    }
    http_body_init(&message->body);
}



int http_message_set_header(http_message_t* message, char* name, char* value) {
    http_header_t* header = http_message_get_header(message, name, true);
    if (header == NULL) {
        return ERROR;
    }

    int err = 0;

    if ((err = http_header_set_name(header, name)) < 0) {
        return err;
    }
    if ((err = http_header_set_content(header, value)) < 0) {
        return err;
    }

    int index = ((int)((int)header - (int)&message->header[0]) / (int)sizeof(http_header_t));

    return index;
}


http_header_t* http_message_get_header(http_message_t* message, char* name, bool return_free_header) {
    int free_header_index = -1;
    int name_len = strlen(name);
    for (int i = 0; i < HTTP_MESSAGE_HEADERS; i++) {
        char* hname = http_header_get_name(&message->header[i]);
        int hname_len = strlen(hname);

        if (hname_len == 0 && free_header_index == -1) {
            free_header_index = i;
            continue;
        }
        else if (hname_len == name_len) {
            for (int j = 0; j <= name_len; j++) {
                char ch = hname[j];

                if (hname[j] != tolower(name[j])) {
                    break;
                }

                else if (j == name_len && ch == '\0') {
                    return &message->header[i];
                }
            }
        }
    }

    if (return_free_header && free_header_index >= 0) {
        return &message->header[free_header_index];
    }

    return NULL;
}

char* http_message_get_header_content(http_message_t* message, char* name) {
    http_header_t* header = http_message_get_header(message, name, false);
    if (header == NULL) {
        return NULL;
    }

    return http_header_get_content(header);
}


http_body_t* http_message_get_body(http_message_t* message) {
    return &message->body;
}