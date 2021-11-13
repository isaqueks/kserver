#ifndef HTTP_HEADER
#define HTTP_HEADER

typedef struct {

    char name[128];
    // I really think some KBs of memory loss
    // is better than a lot of memory leaks.
    char content[2048];

} http_header;

/**
 * @brief ! This shouldn't be used in most cases.
 * Allocates a new http_header struct.
 * 
 * @return http_header* 
 */
http_header* http_header_create();

/**
 * @brief ! Shouldn't be used in most cases.
 * Frees and allocated http_header.
 * 
 * @param header The header to free
 */
void http_header_free(http_header* header);

int http_header_set_name(http_header* header, char* name);
int http_header_set_content(http_header* header, char* content);

int http_header_parse(http_header* header, char* line);

#endif