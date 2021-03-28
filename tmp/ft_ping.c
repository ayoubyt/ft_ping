#include "ft_ping.h"

int main()
{
    int socket_fd;
    struct sockaddr_in dst_addr;
    struct addrinfo *res;
    struct addrinfo hints;
    struct icmp icmp;

    socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (socket_fd < 0)
    {
        perror("error");
        exit(EXIT_FAILURE);
    }
    bzero(&dst_addr, sizeof(dst_addr));

    bzero(&hints, sizeof(hints));
    dst_addr.sin_family = AF_INET;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;
    int r;
    if ((r = getaddrinfo("google.com", NULL, &hints, &res)) < 0)
    {
        fprintf(stderr, "getaddrinfo : %s\n", gai_strerror(r));
        exit(EXIT_FAILURE);
    }

    for (struct addrinfo *curent = res; curent != NULL; curent = curent->ai_next)
    {
        struct sockaddr_in *saddr = (struct sockaddr_in *)curent->ai_addr;
        char buff[1024];
        printf("%s\n", inet_ntop(AF_INET, &saddr->sin_addr.s_addr, buff, sizeof(buff)));
    }

    bzero(&icmp, sizeof(icmp));
    icmp.icmp_type = ICMP_ECHO;
    icmp.icmp_code = 0;
    icmp.icmp_seq = htons(1);
    icmp.icmp_id = htons(getpid());

    // check sum calc
    uint16_t checksum = 0;
    uint16_t *words = (uint16_t *)&icmp;

    for (size_t i = 0; i < (sizeof(icmp) / 2); i++)
        checksum += words[i];
    checksum = ~checksum;

    icmp.icmp_cksum = checksum;
    // printf("%d\n", checksum);
    // sending data


    /*
    **  print usfull data
    */


    unsigned ttl;
    unsigned ttllen;
    r = getsockopt(socket_fd, IPPROTO_IP, IP_TTL, &ttl, &ttllen);
    if (r < 0)
    {
        perror("sendto error");
        exit(EXIT_FAILURE);
    }

    printf("icmp packet sent with id = %u, seq = %u, ttl = %u, ttlen = %u\n", ntohs(icmp.icmp_id), ntohs(icmp.icmp_seq),  ttl, ttllen);


    if (sendto(socket_fd, &icmp, sizeof(icmp), 0, res->ai_addr, sizeof(*res->ai_addr)) < 0)
    {
        perror("sendto error");
        exit(EXIT_FAILURE);
    }

    /*
    ** receive socket
    */
    char rcvbuff[1024];
    struct iovec iovec[1];
    struct msghdr msg;
    struct sockaddr_in from_addr;

    bzero(&from_addr, sizeof(from_addr));
    bzero(&msg, sizeof(msg));

    iovec[0].iov_base = rcvbuff;
    iovec[0].iov_len = sizeof(rcvbuff);
    msg.msg_iov = iovec;
    msg.msg_iovlen = 1;
    msg.msg_name = &from_addr;
    msg.msg_namelen = sizeof(from_addr);

    r = recvmsg(socket_fd, &msg, 0);
    if (r < 0)
    {
        perror("recmsg");
        exit(EXIT_FAILURE);
    }

    struct ip *src_ip = (struct ip *)rcvbuff;
    struct icmp *rcv_icmp = (struct icmp *)(rcvbuff + src_ip->ip_hl * 4);

    printf("icmp packet RECEIVED with id = %u, seq = %u, ttl = %u\n", ntohs(rcv_icmp->icmp_id), ntohs(rcv_icmp->icmp_seq), src_ip->ip_ttl);

    // printf("pi protocol = %u\n", src_ip->ip_p);

    close(socket_fd);
}
