#include "ft_ping.h"

state_t state;

int main(int argc, char **argv)
{
    // socket descriptor
    int sd;
    // destination address
    struct sockaddr_in dst_addr;
    // destination addrress data
    struct addrinfo *dst_addrinfos;
    // hints for getaddrinfo function
    struct addrinfo hints;
    // a tmp variable to store return values
    int r;
    uint8_t *icmp_packet;
    uint16_t icmp_packet_size;
    // buffer to put received icmp packts data on
    uint8_t *rcvbuff;
    int rcvbuffsize;
    int ihl;
    int ihllen;

    // setting id of all sent icmp packet to currrent
    // process id to filter incoming icmp packets based on it
    state.pack_id = getpid();

    // parsing flags and command line options
    // populating state.flags and state.dst with appropriate values
    arg_parse(argc, argv);

    // creating a raw socket to work with icmp packets
    // eathernet and ip headers are handled by kernel
    sd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sd < 0)
        error_and_exit("error : socket ");

    // getting internet header length with in this
    // case, specified by kernel
    r = getsockopt(sd, IPPROTO_IP, IP_TTL);
    if (r < 0)
        error_and_exit("error : getsockopt ");

    // putting destination address information in
    // 'dst_addrinfos' linked list
    ft_bzero(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;
    hints.ai_flags = AI_CANONNAME;
    r = getaddrinfo(state.dst, NULL, &hints, &dst_addrinfos);
    if (r < 0)
    {
        fprintf(stderr, "error : getaddrinfo : %s\n", gai_strerror(r));
        exit(EXIT_FAILURE);
    }
    state.dst_caninical_name = dst_addrinfos->ai_canonname;

    icmp_packet_size = sizeof(struct icmphdr) + state.flags.s;
    //  icmp packet to send wich contains
    //  icmp header (8 byes) + data
    icmp_packet = make_icmp_packet(icmp_packet_size);
    // size is : 60 (max internet header kength (ihl) * 4) + icmp packet size
    rcvbuffsize = 60 + icmp_packet_size;
    rcvbuff = malloc(rcvbuffsize);
    if (!rcvbuff)
        error_and_exit("error : malloc ");

    state.loop = 1;
    while (state.loop)
    {
        send_icmp_packet(sd, dst_addrinfos, icmp_packet, icmp_packet_size);
        if (state.flags.c && state.nsent >= state.flags.c)
            state.loop = 0;
    }
}

// int main(int argc, char**argv)
// {
//     printf("%lu\n", sizeof(struct iphdr));
// }

void error_and_exit(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}
