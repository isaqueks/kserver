#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define HOST "google.com"

int hostname_to_ip(char *  , char *);

int main(int argc , char *argv[])
{
	
	char ip[100];
	
	hostname_to_ip(HOST , ip);
	printf("%s resolved to %s" , HOST , ip);
	
	printf("\n");
	
}
/*
	Get ip from domain name
 */

int hostname_to_ip(char * hostname , char* ip)
{
	struct hostent *he;
	struct in_addr **addr_list;
	int i;
		
	if ( (he = gethostbyname( hostname ) ) == NULL) 
	{
		// get the host info
		herror("gethostbyname");
		return 1;
	}

	addr_list = (struct in_addr **) he->h_addr_list;
	
	for(i = 0; addr_list[i] != NULL; i++) 
	{
		//Return the first one;
		// strcpy(ip , inet_ntoa(*addr_list[i]) );
        unsigned char* ipv4 = (char*) &addr_list[i]->s_addr;
        // strcpy(ip, addr_list[i]->s_addr);
        sprintf(ip, "%d.%d.%d.%d", ipv4[0], ipv4[1], ipv4[2], ipv4[3]);
		return 0;
	}
	
	return 1;
}