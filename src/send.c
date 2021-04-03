#include "ft_ping.h"

void send_icmp_packet(int sd,
                      struct addrinfo *dst_addrinfo,
                      uint8_t *sendbuff,
                      uint16_t sendbuffsize)
{
    int r;

    put_icmphdr(sendbuff, sendbuffsize);
    gettimeofday(&state.last_req_tv, 0);
    r = sendto(sd,
               sendbuff,
               sendbuffsize,
               0,
               dst_addrinfo->ai_addr,
               sizeof(*dst_addrinfo->ai_addr));
    if (r < 0)
    {
        perror("error : sendto ");
        // exit(EXIT_FAILURE);
    }
    state.nsent++;
    if (state.flags.f)
        IG(write(1, ".", 1));
}

void put_icmphdr(void *packet, uint16_t size)
{
    struct icmphdr *res;
    uint16_t pid;
    uint16_t seq;

    res = (struct icmphdr *)packet;
    pid = state.pack_id;
    seq = ++state.pack_seq;

    ft_bzero(res, sizeof(*res));
    res->type = ICMP_ECHO;
    res->un.echo.id = RBS(pid);
    res->un.echo.sequence = RBS(seq);
    res->checksum = checksum(packet, size);
}

uint16_t checksum(uint8_t *data, uint16_t size)
{
    uint32_t sum = 0;
    uint16_t *data16 = (uint16_t *)(data);

    for (size_t i = 0; i < size / 2; i++)
        sum += data16[i];
    if (size % 2 == 1)
    {
        uint16_t tmp = 0;
        *((uint8_t *)&tmp) = data[size - 1];
        sum += tmp;
    }
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    return ((uint16_t)~sum);
}
