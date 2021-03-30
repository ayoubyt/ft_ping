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
    uint8_t *sendbuff;
    uint16_t sendbuffsize;
    // buffer to put received icmp packts data on
    uint8_t *rcvbuff;
    int rcvbuffsize;

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
    state.dst_canonical_name = dst_addrinfos->ai_canonname;

    //  icmp packet to send wich contains
    //  icmp header (8 byes) + data
    sendbuffsize = sizeof(struct icmphdr) + state.flags.s;
    sendbuff = malloc(sendbuffsize);
    if (!sendbuff)
        error_and_exit("error : malloc ");

    // size is : ip header (with no options) + icmp packet size
    rcvbuffsize = sizeof(struct ip) + sendbuffsize;
    rcvbuff = malloc(rcvbuffsize);
    if (!rcvbuff)
        error_and_exit("error : malloc ");

    state.loop = 1;
    while (state.loop)
    {
        send_icmp_packet(sd, dst_addrinfos, sendbuff, sendbuffsize);
        receive_icmp_packet(sd, rcvbuff, rcvbuffsize);
        if (state.flags.c && state.nsent >= state.flags.c)
            break;
        usleep(state.flags.i * 1e6);
    }
}

// int main()
// {
//     uint16_t u = 5 * 6;
//     uint16_t ru;

//     // char c = 'a';
//     // u = c;
//     ru = RBS(5 * 6);

//     char *data = (char *)&u;
//     for (size_t i = 0; i < 2; i++)
//         printf("%02x ", data[i]);
//     printf("\n");
//     data = (char *)&ru;
//     for (size_t i = 0; i < 2; i++)
//         printf("%02x ", data[i]);
//     printf("\n");
// }

void error_and_exit(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}
