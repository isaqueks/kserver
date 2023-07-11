#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/http/header.h"
#include "../include/errors/errors.h"

int main() {

    http_header_t* header = http_header_create();
    assert(header != NULL);

    assert(http_header_set_name(header, "Content-Type") == 0);
    assert(strcmp(http_header_get_name(header), "content-type") == 0);
    
    assert(http_header_set_content(header, "text/html") == 0);
    assert(strcmp(http_header_get_content(header), "text/html") == 0);

    assert(http_header_set_name(header, "Content-Type") == 0);
    assert(strcmp(http_header_get_name(header), "content-type") == 0);

    assert(http_header_set_name(header, "Server") == 0);
    assert(strcmp(http_header_get_name(header), "server") == 0);
    assert(strcmp(http_header_get_content(header), "text/html") == 0);

    assert(http_header_set_name(header, "Way-Too-Long-Header-But-Valid") == 0);
    assert(strcmp(http_header_get_name(header), "way-too-long-header-but-valid") == 0);
    assert(strcmp(http_header_get_content(header), "text/html") == 0);

    assert(http_header_set_content(header, "This is a cool header content, totally valid.") == 0);
    assert(strcmp(http_header_get_content(header), "This is a cool header content, totally valid.") == 0);
    assert(strcmp(http_header_get_name(header), "way-too-long-header-but-valid") == 0);


    char too_long_line[MAX_HEADER_SIZE + 1];
    memset(too_long_line, 'a', MAX_HEADER_SIZE);

    assert(http_header_parse(header, "&Invalid-Name: value\r\n") == MALFORMED_STRING_ERROR); // & char
    assert(http_header_parse(header, "Invalid-CRLF: value\r") == INCOMPLETE_STRING_ERROR); // no line feed
    assert(http_header_parse(header, "Invalid-CRLF: value\n") == MALFORMED_STRING_ERROR); // no CR
    assert(http_header_parse(header, "Invalid-CRLF: value\0\r\n") == INCOMPLETE_STRING_ERROR); // null byte
    assert(http_header_parse(header, "Invalid-CRLF: \tvalue\r\n") == MALFORMED_STRING_ERROR); // null byte
    assert(http_header_parse(header, too_long_line) == BUFFER_TOO_LONG_ERROR); // too long

    assert(http_header_parse(header, "User-Agent: test-ua\r\n") == 0);

    assert(strcmp(http_header_get_name(header), "user-agent") == 0);
    assert(strcmp(http_header_get_content(header), "test-ua") == 0);

    assert(http_header_parse(header, "USER-AGENT:          test-ua\r\n") == 0);

    assert(strcmp(http_header_get_name(header), "user-agent") == 0);
    assert(strcmp(http_header_get_content(header), "test-ua") == 0);

    assert(http_header_parse(header, "user-agent:test-ua\r\n") == 0);

    assert(strcmp(http_header_get_name(header), "user-agent") == 0);
    assert(strcmp(http_header_get_content(header), "test-ua") == 0);

    assert(http_header_parse(header, "Corect-Long-Header: Correct and long value\r\n") == 0);

    assert(strcmp(http_header_get_name(header), "corect-long-header") == 0);
    assert(strcmp(http_header_get_content(header), "Correct and long value") == 0);

}