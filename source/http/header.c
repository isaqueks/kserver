#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../../include/http/header.h"
#include "../../include/constants/constants.h"
#include "../../include/errors/errors.h"

http_header_t* http_header_create() {
    http_header_t* header = malloc(sizeof(http_header_t));
    if (header == NULL) {
        return NULL;
    }
    http_header_init(header);
    return header;
}

void http_header_init(http_header_t* header) {
    memset(header->raw, 0, MAX_HEADER_SIZE);
}

void http_header_free(http_header_t* header) { 
    free(header);
}

char* http_header_get_name(http_header_t* header) {
    return header->raw;
}

char* http_header_get_content(http_header_t* header) {
    int name_len = strlen(http_header_get_name(header));
    return &header->raw[name_len + 1];
}

int http_header_set_name(http_header_t* header, char* name) {

    int name_len = strlen(name);

    if (name_len + 1 /*name's \0*/ + 1 /*content's \0*/ > MAX_HEADER_SIZE) {
        return BUFFER_TOO_LONG_ERROR;
    }

    char* existing_name_ptr = http_header_get_name(header);
    char* existing_content_ptr = http_header_get_content(header);

    int existing_name_len = strlen(existing_name_ptr);
    int existing_content_len = strlen(existing_content_ptr);

    if (name_len != existing_name_len) {
        if (name_len + 1 + existing_content_len + 1 >= MAX_HEADER_SIZE) {
            return BUFFER_TOO_LONG_ERROR;
        }
        existing_content_ptr = memmove((void*)(existing_name_ptr + name_len + 1), existing_content_ptr, existing_content_len + 1);
    }

    // strcpy(existing_name_ptr, name); // lowercase
    for (int i = 0; i < name_len; i++) {
        existing_name_ptr[i] = tolower(name[i]);
    }

    existing_name_ptr[name_len] = '\0';

    return 0;
}

int http_header_set_content(http_header_t* header, char* content) {
    
    char* existing_name_ptr = http_header_get_name(header);
    char* existing_content_ptr = http_header_get_content(header);

    int existing_name_len = strlen(existing_name_ptr);
    
    if (existing_name_len + 1 + strlen(content) + 1 > MAX_HEADER_SIZE) {
        return BUFFER_TOO_LONG_ERROR;
    }
    strcpy(existing_content_ptr, content);
    return 0;
}

int http_header_parse(http_header_t* header, char* line) {

    bool parsing_name = true;

    int name_len = 0;
    int content_start = 0;
    int content_len = 0;

    for (int i = 0; true; i++) {
        char ch = line[i];

        if (ch == '\0') {
            return INCOMPLETE_STRING_ERROR;
        }

        if (ch == CR) {
            if (line[i + 1] == LF) {
                if (parsing_name) {
                    return INCOMPLETE_STRING_ERROR;
                } 
                break;
            } 
            return INCOMPLETE_STRING_ERROR;
        }
        if (!isprint(ch)) {
            return MALFORMED_STRING_ERROR;
        }

        if (parsing_name) {
            if (ch == ':') {
                // name_len++;
                parsing_name = false;
            } 
            else {
                if (name_len + 1 + 1 >= MAX_HEADER_SIZE) {
                    return BUFFER_TOO_LONG_ERROR;
                }

                if (ch != '-') {
                    if (!isalpha(ch)) {
                        return MALFORMED_STRING_ERROR;
                    }
                    ch = tolower(ch);
                }
                name_len++;
            }
        }
        else {
            if (ch == ' ' && content_len == 0) {
                continue;
            }
            if (name_len + 1 + content_len + 1 >= MAX_HEADER_SIZE) {
                return BUFFER_TOO_LONG_ERROR;
            }
            if (content_len == 0) {
                content_start = i;
            }
            content_len++;
            // header->content[content_len++] = ch;
        }
      
    }

    char tmp[MAX_HEADER_SIZE];
    strncpy(tmp, line, name_len);
    tmp[name_len] = '\0';

    int err = 0;
    if ((err = http_header_set_name(header, tmp)) != 0) {
        return err;
    }

    strncpy(tmp, line + content_start, content_len);
    tmp[content_len] = '\0';

    if ((err = http_header_set_content(header, tmp)) != 0) {
        return err;
    }

    return 0;

}

int http_header_output(http_header_t* header, char* out, int buf_size) {
    char* name = http_header_get_name(header);
    char* content = http_header_get_content(header);

    int name_len = strlen(name);
    int content_len = strlen(content);

    if (name_len + 2 /* ": " */ + content_len + 3 /* "\r\n\0 " */ > buf_size) {
        return BUFFER_TOO_LONG_ERROR;
    }

    return sprintf(out, "%s: %s\r\n", name, content);
}