#include "ft_ping.h"

void receive_icmp_packet(int sd, uint8_t *rcvbuff, int rcvbuffsize)
{
    struct msghdr msghdr;
    // gather/scatter struct to store
    // received packet data on
    struct iovec iovec[1];
    int r;

    iovec[0].iov_base = rcvbuff;
    iovec[0].iov_len = rcvbuffsize;

    // populatin msghdr struct to pass it to recvmsg function

    ft_bzero(&msghdr, sizeof(msghdr));
    msghdr.msg_iov = iovec;
    msghdr.msg_iovlen = 1;

    r = recvmsg(sd, &msghdr, 0);
    if (r >= 0)
        handle_packet(rcvbuff);
}

void handle_packet(uint8_t *packet)
{
    struct sockaddr_in source_addr;
    struct ip *ip;
    struct icmp *icmp;
    // curent uinx epoch
    int r;

    ip = (struct ip *)packet;
    icmp = (struct icmp *)(packet + ip->ip_hl * 4);
    if (icmp->icmp_type == ICMP_ECHOREPLY)
    {

        if ((uint16_t)RBS(icmp->icmp_id) == (uint16_t)state.pack_id)
        {
            struct timeval current;
            double timerange;

            gettimeofday(&current, 0);
            timerange = TVMSDIFF(current, state.last_req_tv);

            print_packet(ip, icmp, timerange);
            rtt_update(timerange);
            state.nreceived++;
        }
    }
    else
    {
        // nested ip and icmp packet header contained in returned icmp packet
        // wich contains opriginal header + 64 bit of original data
        struct ip *nip = &(icmp->icmp_ip);
        struct icmp *nicmp = (struct icmp *)((uint8_t *)nip + ip->ip_hl * 4);

        if ((uint16_t)RBS(nicmp->icmp_id) == (uint16_t)state.pack_id)
        {
            switch (icmp->icmp_type)
            {
            case ICMP_TIME_EXCEEDED:
            case ICMP_DEST_UNREACH:
                print_error_packet(ip, nicmp, icmp->icmp_type);
                break;
            default:
                printf("unkown error..packet received with ICMP type %hu code %hu\n",
                       icmp->icmp_type,
                       icmp->icmp_code);
                break;
            }
            state.nerr++;
        }
    }
}

void print_packet(struct ip *ip, struct icmp *icmp, double timerange)
{
    static char source_addr_str[INET_ADDRSTRLEN + 1] = {0};
    struct addrinfo *sender_addrinfos;

    inet_ntop(AF_INET, &ip->ip_src.s_addr, source_addr_str, sizeof(source_addr_str) - 1);
    sender_addrinfos = get_sender_addrinfo(source_addr_str);
    printf(
        "%hu bytes from %s (%s): icmp_seq=%hu ttl=%hu time=%.1lf ms\n",
        RBS(ip->ip_len) - ip->ip_hl * 4,
        sender_addrinfos->ai_canonname,
        source_addr_str,
        RBS(icmp->icmp_seq),
        ip->ip_ttl,
        timerange);
    freeaddrinfo(sender_addrinfos);
}

void print_error_packet(struct ip *ip, struct icmp *nicmp, uint8_t error)
{
    static char source_addr_str[INET_ADDRSTRLEN + 1] = {0};
    struct addrinfo *sender_addrinfos;

    inet_ntop(AF_INET, &ip->ip_src.s_addr, source_addr_str, sizeof(source_addr_str) - 1);
    sender_addrinfos = get_sender_addrinfo(source_addr_str);

    printf(
        "%hu bytes from %s (%s): icmp_seq=%hu ",
        RBS(ip->ip_len) - ip->ip_hl * 4,
        sender_addrinfos->ai_canonname,
        source_addr_str,
        RBS(nicmp->icmp_seq));
    switch (error)
    {
    case ICMP_TIME_EXCEEDED:
        printf("Time to live exceeded\n");
        break;
    case ICMP_DEST_UNREACH:
        printf("Destination Host Unreachable\n");
        break;
    default:
        printf("\n");
        break;
    }
    freeaddrinfo(sender_addrinfos);
}

struct addrinfo *get_sender_addrinfo(char *addr)
{
    struct addrinfo hints;
    struct addrinfo *sender_addrinfos;
    int r;

    ft_bzero(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;
    hints.ai_flags = AI_CANONNAME;
    r = getaddrinfo(addr, NULL, &hints, &sender_addrinfos);
    if (r < 0)
    {
        fprintf(stderr, "error : getaddrinfo : %s\n", gai_strerror(r));
        exit(EXIT_FAILURE);
    }
    return sender_addrinfos;
}
