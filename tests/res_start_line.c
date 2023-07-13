#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "../include/http/res_start_line.h"
#include "../include/errors/errors.h"
#include "../include/constants/http_status_codes.h"


int main() {

    http_response_start_line_t* start_line = http_res_start_line_create();
    assert(start_line != NULL);

    assert(http_res_start_line_set_version(start_line, "SHIT") == MALFORMED_STRING_ERROR);
    assert(http_res_start_line_set_version(start_line, "HTTP&1.1") == MALFORMED_STRING_ERROR);
    assert(http_res_start_line_set_version(start_line, "HTTP/1.1") >= 0);
    assert(http_res_start_line_set_version(start_line, "HTTP/1.0") >= 0);
    assert(strcmp(start_line->version, "HTTP/1.0") == 0);

    assert(http_res_start_line_set_status_code(start_line, HTTP_STATUS_BAD_REQUEST) >= 0);
    assert(start_line->status_code == HTTP_STATUS_BAD_REQUEST);

    assert(http_res_start_line_set_status_message(start_line, "Bad request") == strlen("Bad request")+1);
    assert(strcmp(start_line->status_message, "Bad request") == 0);

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    assert(http_res_start_line_output(start_line, buffer, sizeof(buffer)) >= 0);
    assert(strcmp(buffer, "HTTP/1.0 400 Bad request\r\n") == 0);

    assert(http_res_start_line_output(start_line, buffer, 27) >= 0);

    assert(http_res_start_line_output(start_line, buffer, 26) == BUFFER_TOO_LONG_ERROR);
    assert(http_res_start_line_output(start_line, buffer, 25) == BUFFER_TOO_LONG_ERROR);



    return 0;
}