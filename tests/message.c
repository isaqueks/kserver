#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "../include/errors/errors.h"
#include "../include/http/header.h"
#include "../include/http/message.h"
#include "../include/http/body.h"
#include "../include/net/tcp_socket.h"

int main() {

    http_message_t* message = http_message_create();
    assert(message != NULL);


    assert(http_message_set_header(message, "Content-Type", "text/html") == 0);
    assert(strcmp(http_message_get_header_content(message, "Content-Type"), "text/html") == 0);
    assert(http_message_set_header(message, "content-type", "application/json") == 0);
    assert(strcmp(http_message_get_header_content(message, "Content-Type"), "application/json") == 0);

    assert(http_message_set_header(message, "user-agent", "Test UA") == 1);
    assert(strcmp(http_message_get_header_content(message, "User-Agent"), "Test UA") == 0);
    assert(http_message_set_header(message, "User-Agent", "Another test UA") == 1);
    assert(strcmp(http_message_get_header_content(message, "user-agent"), "Another test UA") == 0);

    assert(http_message_set_header(message, "content-length", "123") == 2);
    assert(strcmp(http_message_get_header_content(message, "Content-Length"), "123") == 0);
    assert(http_message_set_header(message, "Content-Length", "456") == 2);
    assert(strcmp(http_message_get_header_content(message, "content-length"), "456") == 0);

    assert(http_message_get_header(message, "Content-Type", true) == &message->header[0]);
    assert(http_message_get_header(message, "content-TYPE", true) == &message->header[0]);

    assert(http_message_get_header(message, "user-agent", true) == &message->header[1]);

    assert(http_message_get_header(message, "content-length", true) == &message->header[2]);

    assert(http_message_get_header(message, "Inexistent-Header", false) == NULL);
    assert(http_message_get_header_content(message, "Inexistent-Header") == NULL);


    

    return 0;

}