#ifndef HTTP_MESSAGE
#define HTTP_MESSAGE 1

#include "start_line.h"
#include "header.h"
#include "body.h"

#define HTTP_MESSAGE_HEADERS 6

typedef struct {

    http_start_line_t start_line;
    http_header_t header[HTTP_MESSAGE_HEADERS];
    http_body_t body;

} http_message_t;


/**
 * @brief Creates a new http message in the heap memory and initializes it.
 * 
 * @return http_message_t* 
 */
http_message_t* http_message_create();


/**
 * @brief Initializes the message
 * 
 * @param message 
 */
void http_message_init(http_message_t* message);


// /**
//  * @brief Sets the start line of the message
//  * 
//  * @param message 
//  * @param start_line 
//  * @return int 0 if success, < 0 if error
//  */
// int http_message_set_start_line(http_message_t* message, http_start_line_t* start_line);


/**
 * @brief 
 * 
 * @param message The message to set the header
 * @param name The header name
 * @param value The header value
 * @return int The header index if ok, < 0 if error
 */
int http_message_set_header(http_message_t* message, char* name, char* value);

/**
 * @brief Get the header object with the specified name
 * 
 * @param message The message to set the header
 * @param name The header name to search for
 * @param return_free_header If true, returns the first free header if no header with the specified name is found
 * @return http_header_t* The header matching the name or NULL
 */
http_header_t* http_message_get_header(http_message_t* message, char* name, bool return_free_header);

/**
 * @brief Get the header content with the specified name
 * 
 * @param message The message to set the header
 * @param name The header name to search for
 * @return char* The header's content matching the name or NULL
 */
char* http_message_get_header_content(http_message_t* message, char* name);


/**
 * @brief Get the body object
 * 
 * @param message The message containing the body
 * @return http_body_t* 
 */
http_body_t* http_message_get_body(http_message_t* message);

#endif