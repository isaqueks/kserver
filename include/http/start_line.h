#ifndef START_LINE
#define START_LINE 1

#ifndef MAX_HTTP_PATH_SIZE
#define MAX_HTTP_PATH_SIZE 512
#endif

#ifndef MAX_HTTP_VERSION_SIZE
#define MAX_HTTP_VERSION_SIZE 10
#endif

#ifndef MAX_HTTP_METHOD_SIZE
#define MAX_HTTP_METHOD_SIZE 8
#endif

/*

    Remember:
    ** I ** prefrer to use the stack memory with some KBs of
    memory wastage than to use the heap memory with a lot of
    leaks, fragmentation and slowness. But this is ** MY **
    implementation, you can change it if you want and know how
    to manage the heap memory properly.

*/

typedef struct {

    char method[MAX_HTTP_METHOD_SIZE];
    char version[MAX_HTTP_VERSION_SIZE];
    char url[MAX_HTTP_PATH_SIZE];

} http_start_line_t;

/**
 * @brief ! This shouldn't be used in most cases,
 * as it uses malloc. It is expected that
 * you already have a buffer to work with.
 * Only use it when you really need.
 */
http_start_line_t* http_start_line_create();

/**
 * @brief Initializes the start line.
 * 
 * @param start_line 
 */
void http_start_line_init(http_start_line_t* start_line);

/**
 * @brief This shouldn't be used most times.
 * Frees the allocated *http_start_line_t*
 * 
 * @param start_line 
 */
void http_start_line_free(http_start_line_t* start_line);

int http_start_line_set_method(http_start_line_t* start_line, char* method);
int http_start_line_set_version(http_start_line_t* start_line, char* version);
int http_start_line_set_path(http_start_line_t* start_line, char* url);

/**
 * @brief Parses the start line of an HTTP request from buffer.
 * Buffer should be at least the whole line including CRLF.
 * 
 * @param start_line The start line to parse to
 * @param buffer The stat line string
 * @return int The number of bytes read or < 0 if there was an error
 */
int http_start_line_parse(http_start_line_t* start_line, char* buffer, int buffer_size);

#endif