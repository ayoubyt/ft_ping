#include "ft_ping.h"

void send_icmp_packet(int sd,
                      struct addrinfo *dst_addrinfo,
                      uint8_t *packet,
                      uint16_t packet_size)
{
    int r;

    r = sendto(sd, packet, packet_size, 0, dst_addrinfo->ai_addr, sizeof(*dst_addrinfo->ai_addr));
    if (r < 0)
    {
        perror("error : sendto ");
        exit(EXIT_FAILURE);
    }
    state.nsent++;
}

uint8_t *make_icmp_packet(uint16_t size)
{
    // icmp packet to send wich contains
    // icmp header (8 byes) + data
    uint8_t *icmp_packet;

    icmp_packet = malloc(sizeof(struct icmphdr) + size);
    put_icmphdr(icmp_packet, size);

    return (icmp_packet);
}

void put_icmphdr(void *packet, uint16_t size)
{
    struct icmphdr *res;
    uint16_t pid;
    uint16_t seq;

    res = (struct icmphdr *)packet;
    pid = getpid();
    seq = ++state.pack_seq;

    ft_bzero(res, sizeof(*res));
    res->type = ICMP_ECHO;
    res->un.echo.id = STOBS(pid);
    res->un.echo.sequence = STOBS(seq);

    res->checksum = checksum(packet, size);
}

uint16_t checksum(uint8_t *data, uint16_t size)
{
    uint16_t sum = 0;
    uint16_t *data16 = (uint16_t *)(data);

    for (size_t i = 0; i < size / 2; i++)
        sum += data16[i];
    if (size % 2 == 1)
    {
        uint16_t tmp = 0;
        *(&tmp) = *(data + size - 1);
        sum += tmp;
    }
    return (~sum);
}
