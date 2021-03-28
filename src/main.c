#include "ft_ping.h"

int main()
{
    // socket descriptor
    int sd;
    // destination address
    struct sockaddr_in dst_addr;
    // destination addrress data
    struct addrinfo dst_addrinfo;
    // hints for getaddrinfo function
    struct addrinfo dst_addrinfo;


    // creating a raw socket to work with icmp packets
    // eathernet and ip headers are handled by kernel
    sd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sd < 0)
        error_and_exit("socket ");
}

void error_and_exit(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}
