#include "ft_ping.h"

int main()
{
    int socket_fd;


    socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (socket_fd < 0)
    {
        perror("error");
        exit(EXIT_FAILURE);
    }
    close(socket_fd);
}
