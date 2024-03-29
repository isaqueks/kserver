#ifndef BODY
#define BODY 1
#include <inttypes.h>

#include "../net/tcp_socket.h"

#ifndef MAX_HTTP_BODY_SIZE
#define MAX_HTTP_BODY_SIZE 2048 - sizeof(uint32_t)

#endif

typedef struct {

    char data[MAX_HTTP_BODY_SIZE];
    uint32_t length;

} http_body_t;

/**
 * @brief ! This souldn't be used in most cases as it uses malloc (heap memory)
 * Creates a new http body message in the heap memory and initializes it.
 * 
 */
http_body_t* http_body_create();


/**
 * @brief Initializes the body
 * 
 * @param body The body to be initialized
 */
void http_body_init(http_body_t* body);

/**
 * @brief Sets the whole body data.
 * 
 * @param body The body to write to
 * @param data The source buffer
 * @param length The buffer length, in bytes
 * @return int The number of bytes written or < 0 if there was an error
 */
int http_body_set(http_body_t* body, char* data, uint32_t length);

/**
 * @brief Appends data to the body.
 * 
 * @param body The target http_body_t
 * @param data The buffer to append to the body
 * @param length The buffer length
 * @return int The number of bytes appended or < 0 if there was an error
 */
int http_body_append(http_body_t* body, char* data, uint32_t length);


/**
 * @brief Writes the body to the socket.
 * 
 * @param body 
 * @param socket 
 * @return int The number of bytes written or < 0 if there was an error
 */
int http_body_flush(http_body_t* body, tcp_socket_t* socket);

/**
 * @brief Should only be used with heap stored http_body.
 * Frees the allocated http_body_t
 * @param body The body to be destroyed
 */
void http_body_free(http_body_t* body);

#endif