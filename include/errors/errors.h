#ifndef ERROR_H
#define ERROR_H 1

#include <stdio.h>

int __err(int code, char* file, int line, char* msg);

#ifdef DEBUG
#define GETERR(X, Y) __err(X, __FILE__, __LINE__, Y)
#else
#define GETERR(X, Y) X
#endif

#define ERROR GETERR(-1, "ERROR")
#define CREATE_SOCKET_ERROR GETERR(-2, "CREATE_SOCKET_ERROR")
#define CONNECT_ERROR GETERR(-3, "CONNECT_ERROR")

#define INCOMPLETE_STRING_ERROR GETERR(-4, "INCOMPLETE_STRING_ERROR")
#define BUFFER_TOO_LONG_ERROR GETERR(-5, "BUFFER_TOO_LONG_ERROR")
#define MALFORMED_STRING_ERROR GETERR(-6, "MALFORMED_STRING_ERROR")

#define HEAD_ALREADY_SENT_ERROR GETERR(-7, "HEAD_ALREADY_SENT_ERROR")

#endif