#include "ft_ping.h"

uint8_t *make_icmp_packet(uint16_t size)
{
    uint8_t *icmp_packet;

    icmp_packet = malloc(sizeof(struct icmphdr) + size);
    put_icmphdr(icmp_packet, size);

    return (icmp_packet);
}