#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "../include/http/start_line.h"
#include "../include/errors/errors.h"

char FAILED_STR[2048];

int main() {

    memset(FAILED_STR, 'A', 2047);
    FAILED_STR[2047] = '\0';

    http_start_line_t* start_line = http_start_line_create();

    assert(start_line != NULL);

    assert(strlen(start_line->method) == 0);
    assert(strlen(start_line->version) == 0);
    assert(strlen(start_line->path) == 0);

    assert(http_start_line_set_method(start_line, FAILED_STR) == ERROR);
    assert(strlen(start_line->method) == 0);
    assert(http_start_line_set_method(start_line, "GET") == 0);
    assert(strcmp(start_line->method, "GET") == 0);

    assert(http_start_line_set_version(start_line, FAILED_STR) == ERROR);
    assert(strlen(start_line->version) == 0);
    assert(http_start_line_set_version(start_line, "HTTP/1.1") == 0);
    assert(strcmp(start_line->version, "HTTP/1.1") == 0);

    assert(http_start_line_set_path(start_line, FAILED_STR) == ERROR);
    assert(strlen(start_line->path) == 0);
    assert(http_start_line_set_path(start_line, "/") == 0);
    assert(strcmp(start_line->path, "/") == 0);

    char *working_start_line_1 = "GET / HTTP/1.1\r\n";
    int result = http_start_line_parse(start_line, working_start_line_1, strlen(working_start_line_1));
    assert(result == strlen(working_start_line_1));

    assert(strcmp(start_line->method, "GET") == 0);
    assert(strcmp(start_line->path, "/") == 0);
    assert(strcmp(start_line->version, "HTTP/1.1") == 0);

    char *working_start_line_2 = "POST /url/to/path HTTP/1.0\r\n";

    result = http_start_line_parse(start_line, working_start_line_2, strlen(working_start_line_2));
    assert(result == strlen(working_start_line_2));

    assert(strcmp(start_line->method, "POST") == 0);
    assert(strcmp(start_line->path, "/url/to/path") == 0);
    assert(strcmp(start_line->version, "HTTP/1.0") == 0);

    char* not_working_start_line_1 = "GET / HTTP/1.1"; // no CRLF
    assert(http_start_line_parse(start_line, not_working_start_line_1, 
    strlen(not_working_start_line_1)) == INCOMPLETE_STRING_ERROR);

    char* not_working_start_line_2 = "THISISATOOLONGMETHOD / HTTP/1.1\r\n";
    assert(http_start_line_parse(start_line, not_working_start_line_2, 
    strlen(not_working_start_line_2)) == BUFFER_TOO_LONG_ERROR);

    char* not_working_start_line_3 = "GET  / HTTP/1.1\r\n"; // extra space
    assert(http_start_line_parse(start_line, not_working_start_line_3,
    strlen(not_working_start_line_3)) == MALFORMED_STRING_ERROR);

    char* not_working_start_line_4 = "GET / HTTP/1.1\ra"; // "a" instead of LF
    assert(http_start_line_parse(start_line, not_working_start_line_4,
    strlen(not_working_start_line_4)) == MALFORMED_STRING_ERROR);

    char* not_working_start_line_5 = " GET / HTTP/1.1\r\n"; // extra space
    assert(http_start_line_parse(start_line, not_working_start_line_5,
    strlen(not_working_start_line_5)) == MALFORMED_STRING_ERROR);

    char* working_with_extra_content = "HEAD /index.html HTTP/1.1\r\nHost: header...";
    assert(http_start_line_parse(start_line, working_with_extra_content,
    strlen(working_with_extra_content)) == 27);

    assert(strcmp(start_line->method, "HEAD") == 0);
    assert(strcmp(start_line->path, "/index.html") == 0);
    assert(strcmp(start_line->version, "HTTP/1.1") == 0);

    return 0;
}