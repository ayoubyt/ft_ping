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

    // parsing flags and command line options
    // populating state.flags and state.dst with appropriate values
    arg_parse(argc, argv);

    // creating a raw socket to work with icmp packets
    // eathernet and ip headers are handled by kernel
    sd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sd < 0)
        error_and_exit("erro : socket ");

    // putting destination address information in
    // 'dst_addrinfos' linked list
    ft_bzero(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;
    r = getaddrinfo(state.dst, NULL, &hints, &dst_addrinfos);
    if (r < 0)
    {
        fprintf(stderr, "error : getaddrinfo : %s\n", gai_strerror(r));
        exit(EXIT_FAILURE);
    }

    icmp_packet_size = sizeof(struct icmphdr) + state.flags.s;
    icmp_packet = make_icmp_packet(icmp_packet_size);

    while (TRUE)
    {
        send_icmp_packet(sd, dst_addrinfos, icmp_packet, icmp_packet_size);
        if (state.flags.c && state.nsent >= state.flags.c)
            break;
    }
}

// int main(int argc, char**argv)
// {
//     printf("%lu\n", sizeof(struct icmphdr));
// }

void error_and_exit(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}
