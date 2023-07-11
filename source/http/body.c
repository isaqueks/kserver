#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../../include/errors/errors.h"
#include "../../include/http/body.h"

http_body_t* http_body_create() {
    http_body_t* body = (http_body_t*)malloc(sizeof(http_body_t));
    if (body == NULL) {
        return NULL;
    }
    
    http_body_init(body);

    return body;
}

void http_body_init(http_body_t* body) {
    body->length = 0;
}

int http_body_set(http_body_t* body, char* data, uint32_t length) {
    if (length > MAX_HTTP_BODY_SIZE) {
        return BUFFER_TOO_LONG_ERROR;
    }
    memcpy(body->data, data, length);
    return length;
}

int http_body_append(http_body_t* body, char* data, uint32_t length) {
    if (body->length + length > MAX_HTTP_BODY_SIZE) {
        return BUFFER_TOO_LONG_ERROR;
    }
    memcpy(&body->data[body->length], data, length);
    return length;
}

void http_body_free(http_body_t* body) {
    free(body);
}