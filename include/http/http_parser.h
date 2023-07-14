#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H 1

#include <inttypes.h>
#include "request.h"
#include "../net/tcp_socket.h"

typedef uint8_t http_parsing_status;

#define HTTP_PARSING_LINE 0
#define HTTP_PARSING_HEADER 1
#define HTTP_PARSING_BODY 2
#define HTTP_PARSING_DONE 3

/**
 * @brief Parses partially a HTTP request
 * 
 * @param req The output request to be parsed to
 * @param curr_status The current parsing status, the new status will be written to the address
 * @param buffer The HTTP buffer
 * @param len The buffer size
 * @return int The numbers of bytes read or < 0 if error
 */
int http_parse(http_request_t* req, http_parsing_status* curr_status, char* buffer, int len, char** allowed_headers, int allowed_headers_len);

/**
 * @brief Parses a HTTP request until end
 * 
 * @param req The request to be parsed to
 * @param sock The TCP socket
 * @return int 0 if ok or < 0 if error
 */
int http_parse_all(http_request_t* req, tcp_socket_t* sock, http_parsing_status* curr_status, char** allowed_headers, int allowed_headers_len);

#endif