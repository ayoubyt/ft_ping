#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>



int main(int argc, char **argv){
    u_int16_t u = 0;
    char c = 'a';


    u = c;
    char *data = (char*)&u;

    for (size_t i = 0; i < 2; i++)
    {
        printf("%02x ", data[i]);
    }
    printf("\n");

}
