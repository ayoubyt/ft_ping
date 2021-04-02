#include "ft_ping.h"

state_t state;

int main(int argc, char **argv)
{
    // socket descriptor
    int sd;
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
    // packet sending start time
    struct timeval start_tv;
    // end of sending packets time
    struct timeval end_tv;
    // secket timeout time value
    struct timeval timeout_tv;

    // initialize 'state' global struct with default values
    init_state();

    // setting id of all sent icmp packet to currrent
    // process id to filter incoming icmp packets based on it
    state.pack_id = getpid();

    // parsing flags and command line options
    // populating state.flags and state.dst with appropriate values
    arg_parse(argc, argv);

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
    inet_ntop(AF_INET,
              &((struct sockaddr_in *)dst_addrinfos->ai_addr)->sin_addr.s_addr,
              state.dst_addr, INET_ADDRSTRLEN);

    // creating a raw socket to work with icmp packets
    // eathernet and ip headers are handled by kernel
    sd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sd < 0)
        error_and_exit("error : socket ");

    // if -t option is given, set TTL of sent packet
    // to its argument
    r = setsockopt(sd, IPPROTO_IP, IP_TTL, &state.flags.t, sizeof(state.flags.t));
    if (r < 0)
        error_and_exit("error : setsockopt 1");

    // setting timeout of socket receive wait time
    timeout_tv.tv_sec = state.flags.W;
    timeout_tv.tv_usec = (long)(state.flags.W * 1000000) % 1000000;
    r = setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO, &timeout_tv, sizeof(timeout_tv));
    if (r < 0)
        error_and_exit("error : setsockopt 2");
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

    // handling SIGINT signal (CTRL-c) by stopping the loop
    // wjen SIGINT dilevered
    signal(SIGINT, sig_int_handler);

    printf("PING %s (%s), %d(%d) bytes of data\n",
           state.dst_canonical_name, state.dst_addr, state.flags.s, state.flags.s + 28);
    gettimeofday(&start_tv, 0);
    state.loop = 1;
    while (state.loop)
    {
        send_icmp_packet(sd, dst_addrinfos, sendbuff, sendbuffsize);
        receive_icmp_packet(sd, rcvbuff, rcvbuffsize);
        if (state.flags.c && state.nsent >= state.flags.c)
            break;
        usleep(state.flags.i * 1e6);
    }
    gettimeofday(&end_tv, 0);

    //  display stats
    display_stats(TVMSDIFF(end_tv, start_tv));

    /frre allocated memory
    freeaddrinfo(dst_addrinfos);
    free(rcvbuff);
    free(sendbuff);
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
