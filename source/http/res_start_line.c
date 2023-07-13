#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "../../include/http/res_start_line.h"
#include "../../include/errors/errors.h"
#include "../../include/constants/http_status_codes.h"

http_response_start_line_t* http_res_start_line_create() {
    http_response_start_line_t* start_line = (http_response_start_line_t*)malloc(sizeof(http_response_start_line_t));
    if (start_line == NULL) {
        return NULL;
    }
    http_res_start_line_init(start_line);
    return start_line;
}

void http_res_start_line_init(http_response_start_line_t* start_line) {
    strcpy(start_line->version,         "HTTP/1.1");
    start_line->status_code =           HTTP_STATUS_OK;
    strcpy(start_line->status_message,  "OK");
}

int http_res_start_line_set_version(http_response_start_line_t* start_line, char* version) {
    int len = strlen(version);
    if (len >= MAX_HTTP_VERSION_SIZE) {
        return BUFFER_TOO_LONG_ERROR;
    }
    if (len < 5 || strncmp(version, "HTTP/", 5) != 0) {
        return MALFORMED_STRING_ERROR;
    }
    strcpy(start_line->version, version);
    return len+1;
}

int http_res_start_line_set_status_code(http_response_start_line_t* start_line, uint32_t status_code) {
    // Todo: validate the status code
    start_line->status_code = status_code;
    return status_code;
}

int http_res_start_line_set_status_message(http_response_start_line_t* start_line, char* status_message) {
    int len = strlen(status_message);
    if (len >= MAX_HTTP_STATUS_MESSAGE_SIZE) {
        return BUFFER_TOO_LONG_ERROR;
    }
    strcpy(start_line->status_message, status_message);
    return len+1;
}

int http_res_start_line_output(http_response_start_line_t* start_line, char* buffer, int buffer_size) {
    int http_version_len = strlen(start_line->version);
    int status_message_len = strlen(start_line->status_message);

    // [VER] [STATUS_CODE] [STATUS_MESSAGE]
    // http_ver_len WS 3 WS status_message_len CR LF \0
    int total_len = http_version_len + 1 + 3 + 1 + status_message_len + 3;
    if (total_len > buffer_size) {
        return BUFFER_TOO_LONG_ERROR;
    }

    // sprintf always add a null byte at the end

    return sprintf(buffer, "%s %d %s\r\n", start_line->version, start_line->status_code, start_line->status_message);
}