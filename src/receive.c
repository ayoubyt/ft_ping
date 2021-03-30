#include "ft_ping.h"

void receive_icmp_packet(int sd, uint8_t *rcvbuff, int rcvbuffsize)
{
    struct msghdr msghdr;
    // gather/scatter struct to store
    // received packet data on
    struct iovec iovec[1];
    // addrress of icmp echo reply sender
    struct sockaddr_in source_addr;
    struct ip *source_ip;
    struct icmp *icmp;
    int r;

    iovec[0].iov_base = rcvbuff;
    iovec[0].iov_len = rcvbuffsize;

    // populatin msghdr struct to pass it to recvmsg function

    msghdr.msg_name = &source_addr;
    msghdr.msg_namelen = sizeof(source_addr);
    msghdr.msg_iov = &iovec;
    msghdr.msg_iovlen = 1;

    r = recvmsg(sd, &msghdr, 0);
    if (r < 0)
        error_and_exit("error : recvmsg");

    source_ip = (struct ip *)rcvbuff;
    icmp = (struct icmp *)(rcvbuff + source_ip->ip_hl * 4);
}