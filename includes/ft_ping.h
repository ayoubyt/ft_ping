#if !defined(FT_PING_H)
#define FT_PING_H

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>

#include "libft.h"

// default data size after icmp header
#define DDSIZE 56
// defaulr timr interval in secons beteen each
// sent icmp packet
#define DTI 1

// reverse byte order of short int
#define RBS(x) (((x) >> 8) | ((x) << 8))

typedef struct
{
    uint c;
    uint t;
    uint f;
    uint s;
    uint v;
    uint h;
    double i;
} flags_t;

typedef struct
{
    uint max;
    uint min;
    uint avg;
    uint mdev;
    struct timeval last_req_tv; // last icmp reqest time value
} timevalues_t;

typedef struct
{
    flags_t flags;
    char *dst;
    char *dst_canonical_name;
    int nreceived; // number of icmp received
    int nsent;     // number of icmp packets sent
    int pack_seq;  // icmp packet sequence counter
    int pack_id;   // icmp packet id
    int loop;      // packet sender loop condition
    timevalues_t time;
} state_t;

void arg_parse(int argc, char **argv);
uint get_int(char opt, char *str, int min, int max);
double get_double(char opt, char *str);

void error_and_exit(const char *msg);

void send_icmp_packet(int sd,
                      struct addrinfo *dst_addrinfo,
                      uint8_t *packet,
                      uint16_t packet_size);
void put_icmphdr(void *packet, uint16_t size);
uint16_t checksum(uint8_t *data, uint16_t size);

void receive_icmp_packet(int sd, uint8_t *rcvbuff, int rcvbuffsize);

extern state_t state;

#endif // FT_PING_H
