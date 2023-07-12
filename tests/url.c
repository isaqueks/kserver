#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/http/url.h"
#include "../include/errors/errors.h"

#define BUF_SIZE 2048

int main() {

    char buf[BUF_SIZE];
    memset(buf, 0, BUF_SIZE);

    assert(url_get_pathname("/test.php?a=b", buf, BUF_SIZE) == strlen("/test.php"));
    assert(strcmp(buf, "/test.php") == 0);

    assert(url_get_pathname("/dir/text.html", buf, BUF_SIZE) == strlen("/dir/text.html"));
    assert(strcmp(buf, "/dir/text.html") == 0);

    assert(url_get_query("/test.php?param=value&a=b", buf, BUF_SIZE) == strlen("param=value&a=b"));
    assert(strcmp(buf, "param=value&a=b") == 0);

    assert(url_get_query("/dir/text.html?param=value", buf, BUF_SIZE) == strlen("param=value"));
    assert(strcmp(buf, "param=value") == 0);

    assert(url_get_param("/test.php?param=value&a=b", "param", buf, BUF_SIZE) == strlen("value"));
    assert(strcmp(buf, "value") == 0);

    // Test small buffers
    assert(url_get_param("/test.php?a=b", "a", buf, 2) == strlen("b"));
    assert(strcmp(buf, "b") == 0);

    assert(url_get_param("/dir/text.html?param=value&param2=this%20is%20a%20test", "param2", buf, BUF_SIZE) == strlen("this%20is%20a%20test"));
    assert(strcmp(buf, "this%20is%20a%20test") == 0);

    assert(url_get_param("/dir/text.html?param=some=shit", "param", buf, BUF_SIZE) == MALFORMED_STRING_ERROR);
    assert(url_get_param("/dir/text.html?param=value", "inexistent_param", buf, BUF_SIZE) == ERROR);
    assert(url_get_param("/dir/text.html?par", "inexistent_param", buf, BUF_SIZE) == INCOMPLETE_STRING_ERROR);
    assert(url_get_param("/dir/text.html?param=too_big_value", "param", buf, 4) == BUFFER_TOO_LONG_ERROR);
    // Buffer size is OK for param value (but not for name (which is not a problem))
    assert(url_get_param("/dir/text.html?abigparam=12", "abigparam", buf, 3) == strlen("12"));

}