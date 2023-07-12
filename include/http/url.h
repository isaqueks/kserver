#ifndef URL_H
#define URL_H

#include <string.h>
#include <stdlib.h>

#define QUERY_BUFFER_SIZR 1024

/**
 * @brief Gets the pathname of the url
 * 
 * @param url The url
 * @param out The output buffer
 * @param max_len The output buffer size
 * @return int The length of the pathname or < 0 if an error occured
 */
int url_get_pathname(char* url, char* out, int max_len);

/**
 * @brief Gets the query of the url
 * 
 * @param url 
 * @param out 
 * @param max_len 
 * @return int The length of the query or < 0 if an error occured
 */
int url_get_query(char* url, char* out, int max_len);

/**
 * @brief Gets the value of a param in the url
 * 
 * @param url 
 * @param param_name 
 * @param out 
 * @param max_len 
 * @return int The length of the param value or < 0 if an error occured
 */
int url_get_param(char* url, char* param_name, char* out, int max_len);

#endif