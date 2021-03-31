#include "ft_ping.h"

void receive_icmp_packet(int sd, uint8_t *rcvbuff, int rcvbuffsize)
{
    struct msghdr msghdr;
    // gather/scatter struct to store
    // received packet data on
    struct iovec iovec[1];
    // addrress of icmp echo reply sender
    struct sockaddr_in source_addr;
    char source_addr_str[INET_ADDRSTRLEN + 1] = {0};
    struct ip *source_ip;
    struct icmp *icmp;
    int r;
    // curent uinx epoch
    struct timeval current;
    // time interval between last icmp echo 
    // reqest sent and it receive time
    double time_elapsed;

    iovec[0].iov_base = rcvbuff;
    iovec[0].iov_len = rcvbuffsize;

    // populatin msghdr struct to pass it to recvmsg function

    msghdr.msg_name = &source_addr;
    msghdr.msg_namelen = sizeof(source_addr);
    msghdr.msg_iov = iovec;
    msghdr.msg_iovlen = 1;

    r = recvmsg(sd, &msghdr, 0);
    if (r < 0)
        error_and_exit("error : recvmsg");

    gettimeofday(&current, 0);
    
    time_elapsed = (double)(current.tv_usec - state.time.last_req_tv.tv_usec) / 1000;

    source_ip = (struct ip *)rcvbuff;
    icmp = (struct icmp *)(rcvbuff + source_ip->ip_hl * 4);

    printf(
    "%hu bytes from %s (%s): icmp_seq=%hu ttl=%hu time=%.1lf ms\n",
    RBS(source_ip->ip_len) - source_ip->ip_hl * 4,
    state.dst_canonical_name,
    inet_ntop(AF_INET, &source_addr.sin_addr, source_addr_str, sizeof(source_addr_str)),
    RBS(icmp->icmp_seq),
    source_ip->ip_ttl,
    time_elapsed
    );
}
