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

int main(int argc, char **argv)
{
    for (size_t i = 0; i < 10; i++)
    {
        printf("z");
        fflush(stdout);
        usleep(300 * 1000);
        printf("\b");
        fflush(stdout);
        usleep(300 * 1000);
    }
    printf("\n");
}
