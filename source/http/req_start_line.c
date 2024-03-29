#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../../include/errors/errors.h"
#include "../../include/http/req_start_line.h"

http_request_start_line_t* http_req_start_line_create() {

    http_request_start_line_t* start_line = (http_request_start_line_t*)malloc(sizeof(http_request_start_line_t));
    if (start_line == NULL) {
        return NULL;
    }

    http_req_start_line_init(start_line);

    return start_line;
}

void http_req_start_line_init(http_request_start_line_t* start_line) {
    memset(start_line->method, 0, MAX_HTTP_METHOD_SIZE);
    memset(start_line->url, 0, MAX_HTTP_PATH_SIZE);
    memset(start_line->version, 0, MAX_HTTP_VERSION_SIZE);
}

void http_req_start_line_free(http_request_start_line_t* start_line) {
    free(start_line);
}

int http_req_start_line_set_method(http_request_start_line_t* start_line, char* method) {
    if (strlen(method) >= MAX_HTTP_METHOD_SIZE) {
        return ERROR;
    }
    strcpy(start_line->method, method);
    return 0;
}

int http_req_start_line_set_version(http_request_start_line_t* start_line, char* version) {
    if (strlen(version) >= MAX_HTTP_VERSION_SIZE) {
        return ERROR;
    }
    strcpy(start_line->version, version);
    return 0;
}

int http_req_start_line_set_path(http_request_start_line_t* start_line, char* url) {
    if (strlen(url) >= MAX_HTTP_PATH_SIZE) {
        return ERROR;
    }
    strcpy(start_line->url, url);
    return 0;
}

int http_req_start_line_parse(http_request_start_line_t* start_line, char* buffer, int buffer_size) {
    
    // GET / HTTP/1.1
    // A * HTTP/X.Y\r\n -> 14 min length
    if (buffer_size < 14) {
        return INCOMPLETE_STRING_ERROR;
    }

    http_req_start_line_init(start_line);

    bool reading_method = true;
    bool reading_path = false;
    bool reading_version = false;
    int target_buffer_index = 0;

    for (int i = 0; i < buffer_size; i++) {
        char ch = buffer[i];
        if (ch == '\0') {
            return MALFORMED_STRING_ERROR;
        }
        if (ch == ' ') {
            if (reading_method) {
                if (strlen(start_line->method) == 0) {
                    return MALFORMED_STRING_ERROR;
                }
                reading_method = false;
                reading_path = true;
            } else if (reading_path) {
                if (strlen(start_line->url) == 0) {
                    return MALFORMED_STRING_ERROR;
                }
                reading_path = false;
                reading_version = true;
            }
            else if (reading_version) {
                // version should end with CRLF
                return MALFORMED_STRING_ERROR;
            }
            target_buffer_index = 0;
            continue;
        }

        if (reading_method) {
            if (ch < 'A' || ch > 'z') {
                return MALFORMED_STRING_ERROR;
            }
            if (target_buffer_index >= MAX_HTTP_METHOD_SIZE - 1) {
                return BUFFER_TOO_LONG_ERROR;
            }
            start_line->method[target_buffer_index++] = ch;
        }
        else if (reading_path) {
            if (ch < '!' || ch > '~') {
                return MALFORMED_STRING_ERROR;
            }
            if (target_buffer_index >= MAX_HTTP_PATH_SIZE - 1) {
                return BUFFER_TOO_LONG_ERROR;
            }
            start_line->url[target_buffer_index++] = ch;
        }
        else if (reading_version) {
            if (ch == '\r') {
                if (i + 1 >= buffer_size) {
                    return INCOMPLETE_STRING_ERROR;
                }
                if (buffer[i + 1] != '\n') {
                    return MALFORMED_STRING_ERROR;
                }
                // This is the only case where the start_line
                // can be ended
                return i+2;
            }
            if (ch < '!' || ch > 'z') {
                return MALFORMED_STRING_ERROR;
            }
            if (target_buffer_index >= MAX_HTTP_VERSION_SIZE - 1) {
                return BUFFER_TOO_LONG_ERROR;
            }
            start_line->version[target_buffer_index++] = ch;
        }
    }

    return INCOMPLETE_STRING_ERROR;

}