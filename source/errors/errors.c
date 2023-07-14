
#include <stdio.h>
#include <signal.h>

int __err(int code, char* file, int line, char* msg) {
    fprintf(stderr, "ERROR (%d): \"%s\" at %s:%d:\n", code, msg, file, line);
    fflush(stderr);
    raise(SIGTRAP);
    return code;
}
