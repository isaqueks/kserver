
#include <stdlib.h>
#include <string.h>
#include "../../include/errors/errors.h"
#include "../../include/constants/constants.h"
#include "../../include/http/request.h"
#include "../../include/http/http_parser.h"
#include "../../include/net/tcp_socket.h"
#include <stdio.h>

int http_parse(http_request_t* req, http_parsing_status* curr_status, char* buffer, int len, char** allowed_headers, int allowed_headers_len) {
    
    if (*curr_status == HTTP_PARSING_DONE) {
        return 0;
    }
    else if (*curr_status == HTTP_PARSING_LINE) {
        int ret = http_req_start_line_parse(&req->start_line, buffer, len);
        if (ret < 0) {
            return ret;
        }
        *curr_status = HTTP_PARSING_HEADER;
        return ret;
    }
    else if (*curr_status == HTTP_PARSING_HEADER) {
        // identify the end CR LF
        // although http_header_parse already validates the start line,
        // we need to assure that it at least has a CR LF
        int i = 0;
        for (; i < len; i++) {
            if (buffer[i] == CR) {
                if (i + 1 < len && buffer[i + 1] == LF) {
                    break;
                }
                else {
                    printf("nocrlf\n");
                    return MALFORMED_STRING_ERROR;
                }
            }
        }
        
        http_header_t header;
        http_header_init(&header);
        int ret = http_header_parse(&header, buffer);
        if (ret < 0) {
            return ret;
        }

        char* hname = http_header_get_name(&header);
        bool set = false;

        for (int j = 0; j < allowed_headers_len; j++) {
            if (strcmp(hname, allowed_headers[j]) == 0) {
                int set_ret = 0;
                if ((set_ret = http_request_set_header(req,
                    http_header_get_name(&header),
                    http_header_get_content(&header)
                )) < 0) {
                    return set_ret;
                }
                set = true;
                break;
            }
        }

        if (!set) {
            printf("http_parse: header (%s) not set\n", hname);
        }

        // detect if there is a CRLF next,
        // indicating that header is complete

        if (len - ret >= 2 && buffer[ret] == CR && buffer[ret + 1] == LF) {
            // header is complete
            *curr_status = HTTP_PARSING_BODY;
            return ret + 2;
        }
        else {
            // header is not complete
            *curr_status = HTTP_PARSING_HEADER;
        }

        return ret;
    }
    else if (*curr_status == HTTP_PARSING_BODY) {
        return http_request_write_body(req, buffer, len);
    }
}


int http_parse_all(http_request_t* req, tcp_socket_t* sock, http_parsing_status* curr_status, char** allowed_headers, int allowed_headers_len) {
    char buffer[DEFAULT_BUFFER_SIZE];
    int buf_len = 0;
    int total_read = 0;
    int content_length = -1;
    int page_read = 0;

    while (*curr_status != HTTP_PARSING_DONE) {

        char* bufptr = buffer;

        if (total_read == 0 || page_read >= buf_len) {
            page_read = 0;
            memset(buffer, 0, DEFAULT_BUFFER_SIZE);
            if ((buf_len = tcp_socket_read(sock, buffer, DEFAULT_BUFFER_SIZE)) < 0) {
                return ERROR;
            }
        }
        else {
            bufptr = &buffer[page_read];
        }

        int ret = http_parse(req, curr_status, bufptr, buf_len - page_read, allowed_headers, allowed_headers_len);
        if (ret < 0) {
            return ret;
        }


        page_read += ret;
        total_read += ret;

        if (content_length == -1 && *curr_status == HTTP_PARSING_BODY && strcmp(req->start_line.method, "POST") == 0) {
            char* clen_str = http_request_get_header_content(req, "Content-Length");
            if (clen_str == NULL) {
                return ERROR;
            }
            content_length = atoi(clen_str);
        }

        if (*curr_status == HTTP_PARSING_BODY) {
            if (strcmp(req->start_line.method, "POST") != 0 || http_request_get_body(req)->length == content_length) {
                *curr_status = HTTP_PARSING_DONE;
            }
        }
    }

    return total_read;
}