#include "include/errors/errors.h"

#include <arpa/inet.h>  //inet_addr
#include <stdio.h>
#include <string.h>  //strlen
#include <sys/socket.h>
#include <unistd.h>  //write

#include "include/http/http_parser.h"
#include "include/http/response.h"

char* valid_headers[] = {
    "content-length",
    "content-type",
    "user-agent"
};

char* error_page = "<html><body><h1>Internal Server Error</h1></body></html>";

int main(int argc, char* argv[]) {
    tcp_socket_t* server = tcp_socket_create();
    if (server < 0) {
        printf("Error creating socket\n");
        return 1;
    }

    int port = 8880;

    while (port < 9000 && tcp_socket_bind_address(server, INADDR_ANY, port) < 0) {
        printf("Error binding socket\n");
        port++;
    }

    if (tcp_socket_listen(server, 100) < 0) {
        printf("Error listening\n");
        return 1;
    }

    printf("Listening on port %d\n", port);

    while (1) {
        tcp_socket_t client;
        if (tcp_socket_accept(server, &client) < 0) {
            printf("Error handling client\n");
            perror("");
            continue;
        }
        
        http_request_t request;
        http_request_init(&request);

        http_response_t response;
        http_response_init(&response);

        http_parsing_status status = HTTP_PARSING_LINE;

        int ret = 0;

        if ((ret = http_parse_all(&request, &client, &status, valid_headers, 3)) < 0) {
            printf("Error parsing request (%d) (%d)\n", ret, status);

            http_response_write_head(&response, &client, 500, "Internal Server Error");
            http_response_write_body(&response, error_page, strlen(error_page));
            http_response_body_flush(&response, &client);

            tcp_socket_close(&client);
        }
        else {
            printf("%s %s %s\n", request.start_line.method, request.start_line.url, request.start_line.version);
            printf("Content-Length: %s\n", http_request_get_header_content(&request, "content-length"));

            int ret = 0;

            http_response_set_header(&response, "Content-Type", "text/html");
            http_response_set_header(&response, "Connection", "close");

            http_response_write_head(&response, &client, 200, "OK");

            #define PAGE_SIZE 4096*10

            char page[PAGE_SIZE];

            sprintf(page, 
                "<html>"
                    "<head>"
                        "<title>Response Echo</title>"
                    "</head>"
                    "<body>"
                        "<h2>%s %s %s</h2>"
                        "<hr />"
                        "<h3>Headers</h3>"
                        "<hr/>"
                        "<table>"
                        "    <thead>"
                        "        <tr>"
                        "        <th>Header</th>"
                        "        <th>Value</th>"
                        "        </tr>"
                        "    </thead>"
                        "    <tbody>"
                        "        <tr>"
                        "           <td>User-Agent</td>"
                        "           <td>%s</td>"
                        "        </tr>"
                        "    </tbody>"
                        "</table>"
                        "<h3>Body</h3>"
                        "<hr />"
                        "<pre>%s</pre>"
                        "<h3>Lorem Ipsum</h3>"
                        "<pre>"
                            "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce commodo ante eu purus cursus, vel consectetur nisl tristique. Phasellus id facilisis mauris. Nulla convallis quam a eleifend congue. Vivamus ac orci eu massa semper feugiat nec et lorem. Mauris convallis, ex vitae consectetur scelerisque, est metus bibendum elit, nec pharetra turpis ligula a urna. <br />"
                            "Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae; Nam rhoncus ipsum vel lobortis euismod. Aliquam et condimentum sapien. Suspendisse non mauris nec magna scelerisque interdum eu a leo. In dapibus felis ac lectus dignissim, et posuere turpis viverra. Proin viverra lacinia nibh, eu aliquam leo dignissim at. Phasellus blandit lacinia velit eu sagittis. <br />"
                            "Praesent dapibus, velit a auctor condimentum, purus turpis ultricies massa, non bibendum urna turpis a nulla. Donec at condimentum orci. Nunc tristique sem vitae augue cursus, ut fringilla metus elementum. Fusce in quam non dolor rutrum iaculis. Nunc eu metus vel nisl rutrum efficitur. Phasellus ac ipsum malesuada, consequat odio a, pharetra velit. Ut tristique sapien ac nunc volutpat, ac commodo arcu consequat. <br />"
                            "Sed in consectetur diam. Suspendisse iaculis rutrum tellus nec aliquet. Curabitur nec sagittis justo. Morbi a turpis nec sapien sodales pretium. Quisque fermentum gravida nunc, nec scelerisque dolor consectetur sit amet. Mauris vestibulum purus ac velit euismod gravida. Donec consequat semper dui, non eleifend risus pellentesque in. <br />"
                            "Vivamus fringilla neque nec sagittis pellentesque. In ut tellus dui. Ut vulputate malesuada mi, ut vehicula ipsum pellentesque nec. Nunc maximus mi ex, eget dignissim quam rutrum ut. Duis aliquam mollis neque id elementum. Etiam non purus erat. Nam ac leo vitae dolor ultricies luctus. Morbi quis nulla et massa feugiat consectetur. <br />"
                            "Maecenas mollis faucibus dolor, sed ultricies arcu aliquet et. Vestibulum varius nisl quis feugiat consequat. Nulla facilisi. Proin lacinia auctor tincidunt. Quisque laoreet dolor sit amet urna mollis, id ultricies nisi luctus. Nunc commodo nisi at elit suscipit ultrices. Nulla finibus velit sit amet est consequat vestibulum. <br />"
                            "Integer faucibus tellus id nunc consectetur, vel laoreet ipsum posuere. In vel neque ut eros ullamcorper finibus. Mauris tempor tristique purus a tincidunt. Curabitur sodales metus ut neque bibendum, id vestibulum nisl dignissim. Sed eget orci at dui blandit vehicula. Phasellus at semper ligula. Morbi dictum metus ac metus pellentesque, eget fermentum metus posuere. <br />"
                            "Vivamus eu aliquet metus, in tempor nisl. Vestibulum congue aliquam metus, id pellentesque metus pellentesque ac. Nulla ultrices lectus a augue efficitur ultrices. Morbi ullamcorper mauris tellus, in congue leo lobortis id. Vestibulum viverra posuere vestibulum. Etiam faucibus pharetra turpis, sit amet scelerisque lorem facilisis eget. Sed lacinia massa non fringilla rutrum.  <br />"
                            "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce commodo ante eu purus cursus, vel consectetur nisl tristique. Phasellus id facilisis mauris. Nulla convallis quam a eleifend congue. Vivamus ac orci eu massa semper feugiat nec et lorem. Mauris convallis, ex vitae consectetur scelerisque, est metus bibendum elit, nec pharetra turpis ligula a urna. <br />"
                            "Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae; Nam rhoncus ipsum vel lobortis euismod. Aliquam et condimentum sapien. Suspendisse non mauris nec magna scelerisque interdum eu a leo. In dapibus felis ac lectus dignissim, et posuere turpis viverra. Proin viverra lacinia nibh, eu aliquam leo dignissim at. Phasellus blandit lacinia velit eu sagittis. <br />"
                            "Praesent dapibus, velit a auctor condimentum, purus turpis ultricies massa, non bibendum urna turpis a nulla. Donec at condimentum orci. Nunc tristique sem vitae augue cursus, ut fringilla metus elementum. Fusce in quam non dolor rutrum iaculis. Nunc eu metus vel nisl rutrum efficitur. Phasellus ac ipsum malesuada, consequat odio a, pharetra velit. Ut tristique sapien ac nunc volutpat, ac commodo arcu consequat. <br />"
                            "Sed in consectetur diam. Suspendisse iaculis rutrum tellus nec aliquet. Curabitur nec sagittis justo. Morbi a turpis nec sapien sodales pretium. Quisque fermentum gravida nunc, nec scelerisque dolor consectetur sit amet. Mauris vestibulum purus ac velit euismod gravida. Donec consequat semper dui, non eleifend risus pellentesque in. <br />"
                            "Vivamus fringilla neque nec sagittis pellentesque. In ut tellus dui. Ut vulputate malesuada mi, ut vehicula ipsum pellentesque nec. Nunc maximus mi ex, eget dignissim quam rutrum ut. Duis aliquam mollis neque id elementum. Etiam non purus erat. Nam ac leo vitae dolor ultricies luctus. Morbi quis nulla et massa feugiat consectetur. <br />"
                            "Maecenas mollis faucibus dolor, sed ultricies arcu aliquet et. Vestibulum varius nisl quis feugiat consequat. Nulla facilisi. Proin lacinia auctor tincidunt. Quisque laoreet dolor sit amet urna mollis, id ultricies nisi luctus. Nunc commodo nisi at elit suscipit ultrices. Nulla finibus velit sit amet est consequat vestibulum. <br />"
                            "Integer faucibus tellus id nunc consectetur, vel laoreet ipsum posuere. In vel neque ut eros ullamcorper finibus. Mauris tempor tristique purus a tincidunt. Curabitur sodales metus ut neque bibendum, id vestibulum nisl dignissim. Sed eget orci at dui blandit vehicula. Phasellus at semper ligula. Morbi dictum metus ac metus pellentesque, eget fermentum metus posuere. <br />"
                            "Vivamus eu aliquet metus, in tempor nisl. Vestibulum congue aliquam metus, id pellentesque metus pellentesque ac. Nulla ultrices lectus a augue efficitur ultrices. Morbi ullamcorper mauris tellus, in congue leo lobortis id. Vestibulum viverra posuere vestibulum. Etiam faucibus pharetra turpis, sit amet scelerisque lorem facilisis eget. Sed lacinia massa non fringilla rutrum.  <br />"
                            "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce commodo ante eu purus cursus, vel consectetur nisl tristique. Phasellus id facilisis mauris. Nulla convallis quam a eleifend congue. Vivamus ac orci eu massa semper feugiat nec et lorem. Mauris convallis, ex vitae consectetur scelerisque, est metus bibendum elit, nec pharetra turpis ligula a urna. <br />"
                            "Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae; Nam rhoncus ipsum vel lobortis euismod. Aliquam et condimentum sapien. Suspendisse non mauris nec magna scelerisque interdum eu a leo. In dapibus felis ac lectus dignissim, et posuere turpis viverra. Proin viverra lacinia nibh, eu aliquam leo dignissim at. Phasellus blandit lacinia velit eu sagittis. <br />"
                            "Praesent dapibus, velit a auctor condimentum, purus turpis ultricies massa, non bibendum urna turpis a nulla. Donec at condimentum orci. Nunc tristique sem vitae augue cursus, ut fringilla metus elementum. Fusce in quam non dolor rutrum iaculis. Nunc eu metus vel nisl rutrum efficitur. Phasellus ac ipsum malesuada, consequat odio a, pharetra velit. Ut tristique sapien ac nunc volutpat, ac commodo arcu consequat. <br />"
                            "Sed in consectetur diam. Suspendisse iaculis rutrum tellus nec aliquet. Curabitur nec sagittis justo. Morbi a turpis nec sapien sodales pretium. Quisque fermentum gravida nunc, nec scelerisque dolor consectetur sit amet. Mauris vestibulum purus ac velit euismod gravida. Donec consequat semper dui, non eleifend risus pellentesque in. <br />"
                            "Vivamus fringilla neque nec sagittis pellentesque. In ut tellus dui. Ut vulputate malesuada mi, ut vehicula ipsum pellentesque nec. Nunc maximus mi ex, eget dignissim quam rutrum ut. Duis aliquam mollis neque id elementum. Etiam non purus erat. Nam ac leo vitae dolor ultricies luctus. Morbi quis nulla et massa feugiat consectetur. <br />"
                            "Maecenas mollis faucibus dolor, sed ultricies arcu aliquet et. Vestibulum varius nisl quis feugiat consequat. Nulla facilisi. Proin lacinia auctor tincidunt. Quisque laoreet dolor sit amet urna mollis, id ultricies nisi luctus. Nunc commodo nisi at elit suscipit ultrices. Nulla finibus velit sit amet est consequat vestibulum. <br />"
                            "Integer faucibus tellus id nunc consectetur, vel laoreet ipsum posuere. In vel neque ut eros ullamcorper finibus. Mauris tempor tristique purus a tincidunt. Curabitur sodales metus ut neque bibendum, id vestibulum nisl dignissim. Sed eget orci at dui blandit vehicula. Phasellus at semper ligula. Morbi dictum metus ac metus pellentesque, eget fermentum metus posuere. <br />"
                            "Vivamus eu aliquet metus, in tempor nisl. Vestibulum congue aliquam metus, id pellentesque metus pellentesque ac. Nulla ultrices lectus a augue efficitur ultrices. Morbi ullamcorper mauris tellus, in congue leo lobortis id. Vestibulum viverra posuere vestibulum. Etiam faucibus pharetra turpis, sit amet scelerisque lorem facilisis eget. Sed lacinia massa non fringilla rutrum.  <br />"
                            "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce commodo ante eu purus cursus, vel consectetur nisl tristique. Phasellus id facilisis mauris. Nulla convallis quam a eleifend congue. Vivamus ac orci eu massa semper feugiat nec et lorem. Mauris convallis, ex vitae consectetur scelerisque, est metus bibendum elit, nec pharetra turpis ligula a urna. <br />"
                            "Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae; Nam rhoncus ipsum vel lobortis euismod. Aliquam et condimentum sapien. Suspendisse non mauris nec magna scelerisque interdum eu a leo. In dapibus felis ac lectus dignissim, et posuere turpis viverra. Proin viverra lacinia nibh, eu aliquam leo dignissim at. Phasellus blandit lacinia velit eu sagittis. <br />"
                            "Praesent dapibus, velit a auctor condimentum, purus turpis ultricies massa, non bibendum urna turpis a nulla. Donec at condimentum orci. Nunc tristique sem vitae augue cursus, ut fringilla metus elementum. Fusce in quam non dolor rutrum iaculis. Nunc eu metus vel nisl rutrum efficitur. Phasellus ac ipsum malesuada, consequat odio a, pharetra velit. Ut tristique sapien ac nunc volutpat, ac commodo arcu consequat. <br />"
                            "Sed in consectetur diam. Suspendisse iaculis rutrum tellus nec aliquet. Curabitur nec sagittis justo. Morbi a turpis nec sapien sodales pretium. Quisque fermentum gravida nunc, nec scelerisque dolor consectetur sit amet. Mauris vestibulum purus ac velit euismod gravida. Donec consequat semper dui, non eleifend risus pellentesque in. <br />"
                            "Vivamus fringilla neque nec sagittis pellentesque. In ut tellus dui. Ut vulputate malesuada mi, ut vehicula ipsum pellentesque nec. Nunc maximus mi ex, eget dignissim quam rutrum ut. Duis aliquam mollis neque id elementum. Etiam non purus erat. Nam ac leo vitae dolor ultricies luctus. Morbi quis nulla et massa feugiat consectetur. <br />"
                            "Maecenas mollis faucibus dolor, sed ultricies arcu aliquet et. Vestibulum varius nisl quis feugiat consequat. Nulla facilisi. Proin lacinia auctor tincidunt. Quisque laoreet dolor sit amet urna mollis, id ultricies nisi luctus. Nunc commodo nisi at elit suscipit ultrices. Nulla finibus velit sit amet est consequat vestibulum. <br />"
                            "Integer faucibus tellus id nunc consectetur, vel laoreet ipsum posuere. In vel neque ut eros ullamcorper finibus. Mauris tempor tristique purus a tincidunt. Curabitur sodales metus ut neque bibendum, id vestibulum nisl dignissim. Sed eget orci at dui blandit vehicula. Phasellus at semper ligula. Morbi dictum metus ac metus pellentesque, eget fermentum metus posuere. <br />"
                            "Vivamus eu aliquet metus, in tempor nisl. Vestibulum congue aliquam metus, id pellentesque metus pellentesque ac. Nulla ultrices lectus a augue efficitur ultrices. Morbi ullamcorper mauris tellus, in congue leo lobortis id. Vestibulum viverra posuere vestibulum. Etiam faucibus pharetra turpis, sit amet scelerisque lorem facilisis eget. Sed lacinia massa non fringilla rutrum.  <br />"
                            "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce commodo ante eu purus cursus, vel consectetur nisl tristique. Phasellus id facilisis mauris. Nulla convallis quam a eleifend congue. Vivamus ac orci eu massa semper feugiat nec et lorem. Mauris convallis, ex vitae consectetur scelerisque, est metus bibendum elit, nec pharetra turpis ligula a urna. <br />"
                            "Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae; Nam rhoncus ipsum vel lobortis euismod. Aliquam et condimentum sapien. Suspendisse non mauris nec magna scelerisque interdum eu a leo. In dapibus felis ac lectus dignissim, et posuere turpis viverra. Proin viverra lacinia nibh, eu aliquam leo dignissim at. Phasellus blandit lacinia velit eu sagittis. <br />"
                            "Praesent dapibus, velit a auctor condimentum, purus turpis ultricies massa, non bibendum urna turpis a nulla. Donec at condimentum orci. Nunc tristique sem vitae augue cursus, ut fringilla metus elementum. Fusce in quam non dolor rutrum iaculis. Nunc eu metus vel nisl rutrum efficitur. Phasellus ac ipsum malesuada, consequat odio a, pharetra velit. Ut tristique sapien ac nunc volutpat, ac commodo arcu consequat. <br />"
                            "Sed in consectetur diam. Suspendisse iaculis rutrum tellus nec aliquet. Curabitur nec sagittis justo. Morbi a turpis nec sapien sodales pretium. Quisque fermentum gravida nunc, nec scelerisque dolor consectetur sit amet. Mauris vestibulum purus ac velit euismod gravida. Donec consequat semper dui, non eleifend risus pellentesque in. <br />"
                            "Vivamus fringilla neque nec sagittis pellentesque. In ut tellus dui. Ut vulputate malesuada mi, ut vehicula ipsum pellentesque nec. Nunc maximus mi ex, eget dignissim quam rutrum ut. Duis aliquam mollis neque id elementum. Etiam non purus erat. Nam ac leo vitae dolor ultricies luctus. Morbi quis nulla et massa feugiat consectetur. <br />"
                            "Maecenas mollis faucibus dolor, sed ultricies arcu aliquet et. Vestibulum varius nisl quis feugiat consequat. Nulla facilisi. Proin lacinia auctor tincidunt. Quisque laoreet dolor sit amet urna mollis, id ultricies nisi luctus. Nunc commodo nisi at elit suscipit ultrices. Nulla finibus velit sit amet est consequat vestibulum. <br />"
                            "Integer faucibus tellus id nunc consectetur, vel laoreet ipsum posuere. In vel neque ut eros ullamcorper finibus. Mauris tempor tristique purus a tincidunt. Curabitur sodales metus ut neque bibendum, id vestibulum nisl dignissim. Sed eget orci at dui blandit vehicula. Phasellus at semper ligula. Morbi dictum metus ac metus pellentesque, eget fermentum metus posuere. <br />"
                            "Vivamus eu aliquet metus, in tempor nisl. Vestibulum congue aliquam metus, id pellentesque metus pellentesque ac. Nulla ultrices lectus a augue efficitur ultrices. Morbi ullamcorper mauris tellus, in congue leo lobortis id. Vestibulum viverra posuere vestibulum. Etiam faucibus pharetra turpis, sit amet scelerisque lorem facilisis eget. Sed lacinia massa non fringilla rutrum.  <br />"
                            "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce commodo ante eu purus cursus, vel consectetur nisl tristique. Phasellus id facilisis mauris. Nulla convallis quam a eleifend congue. Vivamus ac orci eu massa semper feugiat nec et lorem. Mauris convallis, ex vitae consectetur scelerisque, est metus bibendum elit, nec pharetra turpis ligula a urna. <br />"
                            "Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae; Nam rhoncus ipsum vel lobortis euismod. Aliquam et condimentum sapien. Suspendisse non mauris nec magna scelerisque interdum eu a leo. In dapibus felis ac lectus dignissim, et posuere turpis viverra. Proin viverra lacinia nibh, eu aliquam leo dignissim at. Phasellus blandit lacinia velit eu sagittis. <br />"
                            "Praesent dapibus, velit a auctor condimentum, purus turpis ultricies massa, non bibendum urna turpis a nulla. Donec at condimentum orci. Nunc tristique sem vitae augue cursus, ut fringilla metus elementum. Fusce in quam non dolor rutrum iaculis. Nunc eu metus vel nisl rutrum efficitur. Phasellus ac ipsum malesuada, consequat odio a, pharetra velit. Ut tristique sapien ac nunc volutpat, ac commodo arcu consequat. <br />"
                            "Sed in consectetur diam. Suspendisse iaculis rutrum tellus nec aliquet. Curabitur nec sagittis justo. Morbi a turpis nec sapien sodales pretium. Quisque fermentum gravida nunc, nec scelerisque dolor consectetur sit amet. Mauris vestibulum purus ac velit euismod gravida. Donec consequat semper dui, non eleifend risus pellentesque in. <br />"
                            "Vivamus fringilla neque nec sagittis pellentesque. In ut tellus dui. Ut vulputate malesuada mi, ut vehicula ipsum pellentesque nec. Nunc maximus mi ex, eget dignissim quam rutrum ut. Duis aliquam mollis neque id elementum. Etiam non purus erat. Nam ac leo vitae dolor ultricies luctus. Morbi quis nulla et massa feugiat consectetur. <br />"
                            "Maecenas mollis faucibus dolor, sed ultricies arcu aliquet et. Vestibulum varius nisl quis feugiat consequat. Nulla facilisi. Proin lacinia auctor tincidunt. Quisque laoreet dolor sit amet urna mollis, id ultricies nisi luctus. Nunc commodo nisi at elit suscipit ultrices. Nulla finibus velit sit amet est consequat vestibulum. <br />"
                            "Integer faucibus tellus id nunc consectetur, vel laoreet ipsum posuere. In vel neque ut eros ullamcorper finibus. Mauris tempor tristique purus a tincidunt. Curabitur sodales metus ut neque bibendum, id vestibulum nisl dignissim. Sed eget orci at dui blandit vehicula. Phasellus at semper ligula. Morbi dictum metus ac metus pellentesque, eget fermentum metus posuere. <br />"
                            "Vivamus eu aliquet metus, in tempor nisl. Vestibulum congue aliquam metus, id pellentesque metus pellentesque ac. Nulla ultrices lectus a augue efficitur ultrices. Morbi ullamcorper mauris tellus, in congue leo lobortis id. Vestibulum viverra posuere vestibulum. Etiam faucibus pharetra turpis, sit amet scelerisque lorem facilisis eget. Sed lacinia massa non fringilla rutrum.  <br />"
                            "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce commodo ante eu purus cursus, vel consectetur nisl tristique. Phasellus id facilisis mauris. Nulla convallis quam a eleifend congue. Vivamus ac orci eu massa semper feugiat nec et lorem. Mauris convallis, ex vitae consectetur scelerisque, est metus bibendum elit, nec pharetra turpis ligula a urna. <br />"
                            "Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae; Nam rhoncus ipsum vel lobortis euismod. Aliquam et condimentum sapien. Suspendisse non mauris nec magna scelerisque interdum eu a leo. In dapibus felis ac lectus dignissim, et posuere turpis viverra. Proin viverra lacinia nibh, eu aliquam leo dignissim at. Phasellus blandit lacinia velit eu sagittis. <br />"
                            "Praesent dapibus, velit a auctor condimentum, purus turpis ultricies massa, non bibendum urna turpis a nulla. Donec at condimentum orci. Nunc tristique sem vitae augue cursus, ut fringilla metus elementum. Fusce in quam non dolor rutrum iaculis. Nunc eu metus vel nisl rutrum efficitur. Phasellus ac ipsum malesuada, consequat odio a, pharetra velit. Ut tristique sapien ac nunc volutpat, ac commodo arcu consequat. <br />"
                            "Sed in consectetur diam. Suspendisse iaculis rutrum tellus nec aliquet. Curabitur nec sagittis justo. Morbi a turpis nec sapien sodales pretium. Quisque fermentum gravida nunc, nec scelerisque dolor consectetur sit amet. Mauris vestibulum purus ac velit euismod gravida. Donec consequat semper dui, non eleifend risus pellentesque in. <br />"
                            "Vivamus fringilla neque nec sagittis pellentesque. In ut tellus dui. Ut vulputate malesuada mi, ut vehicula ipsum pellentesque nec. Nunc maximus mi ex, eget dignissim quam rutrum ut. Duis aliquam mollis neque id elementum. Etiam non purus erat. Nam ac leo vitae dolor ultricies luctus. Morbi quis nulla et massa feugiat consectetur. <br />"
                            "Maecenas mollis faucibus dolor, sed ultricies arcu aliquet et. Vestibulum varius nisl quis feugiat consequat. Nulla facilisi. Proin lacinia auctor tincidunt. Quisque laoreet dolor sit amet urna mollis, id ultricies nisi luctus. Nunc commodo nisi at elit suscipit ultrices. Nulla finibus velit sit amet est consequat vestibulum. <br />"
                            "Integer faucibus tellus id nunc consectetur, vel laoreet ipsum posuere. In vel neque ut eros ullamcorper finibus. Mauris tempor tristique purus a tincidunt. Curabitur sodales metus ut neque bibendum, id vestibulum nisl dignissim. Sed eget orci at dui blandit vehicula. Phasellus at semper ligula. Morbi dictum metus ac metus pellentesque, eget fermentum metus posuere. <br />"
                            "Vivamus eu aliquet metus, in tempor nisl. Vestibulum congue aliquam metus, id pellentesque metus pellentesque ac. Nulla ultrices lectus a augue efficitur ultrices. Morbi ullamcorper mauris tellus, in congue leo lobortis id. Vestibulum viverra posuere vestibulum. Etiam faucibus pharetra turpis, sit amet scelerisque lorem facilisis eget. Sed lacinia massa non fringilla rutrum.  <br />"
                        "</pre>"
                    "</body>"
                "</html>",
                request.start_line.method,
                request.start_line.url,
                request.start_line.version,
                http_request_get_header_content(&request, "user-agent"),
                http_request_get_body(&request)->data
            );

            http_response_send_body(&response, &client, page, strlen(page));

            tcp_socket_close(&client);

        }

        fflush(stdout);

    }

    return 0;
}