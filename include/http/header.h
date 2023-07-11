#ifndef HTTP_HEADER
#define HTTP_HEADER

#define MAX_HEADER_SIZE 2048

typedef struct {

    // Two strings - name\0content\0
    char raw[MAX_HEADER_SIZE];

} http_header_t;

/**
 * @brief ! This shouldn't be used in most cases.
 * Allocates a new http_header_t object and initializes it.
 * 
 * @return http_header_t* 
 */
http_header_t* http_header_create();

/**
 * @brief Initialize the header
 * 
 * @param header 
 */
void http_header_init(http_header_t* header);

/**
 * @brief ! Shouldn't be used in most cases.
 * Frees an allocated http_header_t.
 * 
 * @param header The header to free
 */
void http_header_free(http_header_t* header);

int http_header_set_name(http_header_t* header, char* name);
int http_header_set_content(http_header_t* header, char* content);

char* http_header_get_name(http_header_t* header);
char* http_header_get_content(http_header_t* header);

int http_header_parse(http_header_t* header, char* line);

#endif