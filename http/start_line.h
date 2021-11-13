#ifndef START_LINE
#define START_LINE 1

#ifndef MAX_HTTP_PATH_SIZE
#define MAX_HTTP_PATH_SIZE 1024
#endif

typedef struct {

    char method[8];
    char version[10];
    char path[MAX_HTTP_PATH_SIZE];

} http_start_line;

/**
 * @brief ! This shouldn't be used in most cases,
 * as it uses malloc. It is expected that
 * you already have a buffer to work with.
 * Only use it when you really need.
 */
http_start_line* http_start_line_create();

/**
 * @brief This shouldn't be used most times.
 * Frees the allocated *http_start_line*
 * 
 * @param start_line 
 */
void http_start_line_free(http_start_line* start_line);

int http_start_line_parse(http_start_line* start_line, char* line);

#endif