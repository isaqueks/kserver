#ifndef BODY
#define BODY 1
#include <inttypes.h>

#ifndef MAX_HTTP_BODY_SIZE
#define MAX_HTTP_BODY_SIZE 2044
// 2044 + uint32_t = 2048 bytes
#endif

typedef struct {

    char data[MAX_HTTP_BODY_SIZE];
    uint32_t length;

} http_body_t;

/**
 * @brief ! This souldn't be used in most cases as it uses malloc (heap memory)
 * Creates a new http body message in the heap memory.
 * 
 */
http_body_t* http_body_create();

/**
 * @brief Sets the whole body data.
 * 
 * @param body The body to write to
 * @param data The source buffer
 * @param length The buffer length, in bytes
 * @return int The number of bytes written or < 0 if there was an error
 */
int http_body_set(http_body_t* body, char* data, uint16_t length);

/**
 * @brief Appends data to the body.
 * 
 * @param body The target http_body_t
 * @param data The buffer to append to the body
 * @param length The buffer length
 * @return int The number of bytes appended or < 0 if there was an error
 */
int http_body_append(http_body_t* body, char* data, uint16_t length);

/**
 * @brief Should only be used with heap stored http_body.
 * Frees the allocated http_body_t
 * @param body The body to be destroyed
 */
void http_body_free(http_body_t* body);

#endif