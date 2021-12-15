#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define MAX 80
#define PORT 80
#define SA struct sockaddr

#define HEADER(name, content) name ": " content "\r\n"
#define REQ(method, path) method " " path " HTTP/1.1\r\n"

void func(int sockfd)
{

    char* request = 
    REQ("GET", "/phpforum/")
    HEADER("Connection", "close")
    HEADER("Host", "www.php-forum.com")
    HEADER("Pragma", "no-cache")
    HEADER("Upgrade-Insecure-Requests", "0")
    HEADER("Accept", "text/html")
    HEADER("User-Agent", "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/95.0.4638.69 Safari/537.36")
    // Cache-Control: no-cache
    // Upgrade-Insecure-Requests: 1
    // User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/95.0.4638.69 Safari/537.36
    // Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
    // Accept-Encoding: gzip, deflate
    // Accept-Language: en-US,en;q=0.9,pt-BR;q=0.8,pt;q=0.7,es;q=0.6
    // Cookie: phpbb3_cfbs9_u=1; phpbb3_cfbs9_k=; phpbb3_cfbs9_sid=72cc2c863b3f3cd2a41a9635ed8879c5; _ga=GA1.2.1524225412.1636668556; _gid=GA1.2.936505494.1636668556
    "\r\n";

    printf("%s", request);

    // write(sockfd, http, strlen(http));
    send(sockfd, request, strlen(request), 0);

    printf("request sent\n\n");

    char response[1024*1024*4];
    int valread;
    while(valread = read(sockfd, response, 1024*1024)) {
        printf("%s", response);
    }
    close(sockfd);

	// char buff[MAX];
	// int n;
	// for (;;) {
	// 	bzero(buff, sizeof(buff));
	// 	printf("Enter the string : ");
	// 	n = 0;
	// 	while ((buff[n++] = getchar()) != '\n')
	// 		;
	// 	write(sockfd, buff, sizeof(buff));
	// 	bzero(buff, sizeof(buff));
	// 	read(sockfd, buff, sizeof(buff));
	// 	printf("From Server : %s", buff);
	// 	if ((strncmp(buff, "exit", 4)) == 0) {
	// 		printf("Client Exit...\n");
	// 		break;
	// 	}
	// }
}

int main()
{
	int sockfd, connfd;
	struct sockaddr_in servaddr/*, cli*/;

	// socket create and varification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else {
		printf("Socket successfully created..\n");
    }
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("51.15.158.208");
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
	else {
		printf("connected to the server..\n");
    }

	// function for chat
	func(sockfd);

	// close the socket
	close(sockfd);
}
