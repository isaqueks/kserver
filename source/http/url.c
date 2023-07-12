#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../include/http/url.h"
#include "../../include/errors/errors.h"

// TODO: Support absolute urls

int url_get_pathname(char* url, char* out, int max_len) {
    int len = strlen(url);
    int i = 0;
    for (; i < len; i++) {
        char ch = url[i];

        if (i >= max_len) {
            return ERROR;
        }

        if (ch == '?') {
            break;
        }

        out[i] = ch;
    }

    out[i] = '\0';

    return i;
}

int url_get_query(char* url, char* out, int max_len) {
    int len = strlen(url);

    bool writing_param = false;
    
    int j = 0;

    for (int i = 0; i < len; i++) {
        char ch = url[i];

        if (ch == '?') {
            if (writing_param) {
                return MALFORMED_STRING_ERROR;
            }
            writing_param = true;
            continue;
        }

        if (writing_param) {
            if (j >= max_len) {
                return ERROR;
            }
            out[j++] = ch;
        }
    }

    out[j] = '\0';

    return j;
}

int url_get_param(char* url, char* param_name, char* out, int max_len) {
   
    char query[QUERY_BUFFER_SIZR];

    int result = 0;

    if ((result = url_get_query(url, query, QUERY_BUFFER_SIZR)) < 0) {
        return result;
    }

    int query_len = strlen(query);
    int param_name_len = strlen(param_name);

    char value_buf[QUERY_BUFFER_SIZR];

    bool reading_param = true;
    bool is_requested_param = true;

    int j = 0;
    int k = 0;

    for (int i = 0; i < query_len; i++) {
        char ch = query[i];

        if (ch == '&') {
            if (reading_param) {
                return MALFORMED_STRING_ERROR;
            }
            if (is_requested_param) {
                break;
            }

            reading_param = true;
            is_requested_param = true;
            continue;
        }
        else if (ch == '=') {
            if (!reading_param) {
                return MALFORMED_STRING_ERROR;
            }
            if (is_requested_param && k != param_name_len) {
                is_requested_param = false;
                k = 0;
            }
            reading_param = false;
            continue;
        }

        if (reading_param) {
            if (is_requested_param && ch != param_name[k++]) {
                is_requested_param = false;
                k = 0;
                continue;
            }
        }
        else {
            if (is_requested_param) {
                if (j >= max_len) {
                    return BUFFER_TOO_LONG_ERROR;
                }
                out[j++] = ch;
            }
        }

    }

    if (reading_param) {
        return INCOMPLETE_STRING_ERROR;
    }

    if (!is_requested_param) {
        return ERROR;
    }

    out[j] = '\0';

    return j;

}