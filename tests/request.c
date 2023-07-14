#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "../include/http/request.h"
#include "../include/errors/errors.h"


int main() {

    // http_request_t s just a proxy object, so there's not much to test

    http_request_t* request = http_request_create();
    assert(request != NULL);

    assert(http_request_set_header(request, "Host", "localhost") >= 0);
    assert(http_request_get_header(request, "Host") != NULL);
    assert(http_request_get_header(request, "X-Inexistent-Header") == NULL);
    assert(strcmp(http_request_get_header_content(request, "Host"), "localhost") == 0);

    assert(http_request_write_body(request, "Hello, World!", 14) >= 0);
    assert(http_request_get_body(request) == http_message_get_body(http_request_get_message(request)));


    return 0;
}