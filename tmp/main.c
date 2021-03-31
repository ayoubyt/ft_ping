#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <unistd.h>



int main(int argc, char **argv){
    struct timeval s;
    struct timeval e;
    int r;

    r = gettimeofday(&s, 0);
    printf("r = %d\n", r);
    sleep(1);

    r= gettimeofday(&e, 0);
    printf("r = %d\n", r);
    printf("ms = %ld\n", (e.tv_usec - s.tv_usec) / 1000);
}
