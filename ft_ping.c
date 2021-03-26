#include "ft_ping.h"

int main()
{
    int                 socket_fd;
    struct sockaddr_in  dst_addr;
    struct addrinfo     *res;
    struct addrinfo     hints;
    struct icmp         icmp;

    // socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    // if (socket_fd < 0)
    // {
    //     perror("error");
    //     exit(EXIT_FAILURE);
    // }

    // bzero(&dst_addr, sizeof(dst_addr));
    // dst_addr.sin_family = AF_INET;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;
    int r;
    if ((r = getaddrinfo("google.com", NULL, &hints, &res)) < 0)
    {
        fprintf(stderr, "getaddrinfo : %s\n", gai_strerror(r));
        exit(EXIT_FAILURE);
    }

    for (struct addrinfo* curent = res; curent != NULL; curent = curent->ai_next)
    {
        struct sockaddr_in *saddr = (struct sockaddr_in*)curent->ai_addr;
        char buff[1024];
        printf("%s\n", inet_ntop(AF_INET, &saddr->sin_addr.s_addr, buff, sizeof(buff)));
    }


    bzero(&icmp, sizeof(icmp));
    icmp.icmp_type = ICMP_ECHO;
    
    
    close(socket_fd);
}
