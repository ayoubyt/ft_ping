#if !defined(FT_PING_H)
#define FT_PING_H

#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/time.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>

#include "libft.h"

// default data size after icmp header
#define DDSIZE 56
// default time interval in secons between each
// sent icmp packet
#define DTI 1
// max time for socket to wait for imcp replies in seconds
#define RCV_TIMEOUT 1

// reverse byte order of short int
#define RBS(x) (((x) >> 8) | ((x) << 8))

// diffrent in mileseconds of two 'timeval' structures
#define TVMSDIFF(tv1, tv2) (                             \
    (double)(((tv1.tv_sec) * 1000000 + (tv1.tv_usec)) -  \
             ((tv2.tv_sec) * 1000000 + (tv2.tv_usec))) / \
    1000)

typedef struct
{
    // variable to check i specification
    // (if it's given or not)
    t_bool i_s;
    uint c;   // number of icmp packets to send
    uint t;   // sett TTL pf packet
    uint f;   // flood ping
    uint s;   // size of icmp data
    uint v;   // verbose (dosent do anything LOL)
    uint h;   // print help
    double W; // timeoute pf packet receive
    double i; // interval between each packet sent
} flags_t;

typedef struct
{
    double max;
    double min;
    double avg;
    double sum;
} timevalues_t;

typedef struct
{
    flags_t flags;
    char *dst;
    char dst_addr[INET_ADDRSTRLEN + 1];
    char dst_rev_name[NI_MAXHOST + 1];
    char *dst_canonical_name;
    uint nreceived;             // number of icmp received
    uint nsent;                 // number of icmp packets sent
    uint nerr;                  // number of errors occured
    int pack_seq;               // icmp packet sequence counter
    int pack_id;                // icmp packet id
    int loop;                   // packet sender loop condition
    struct timeval last_req_tv; // last icmp reqest time value
    timevalues_t rtt;
} state_t;

void arg_parse(int argc, char **argv);
uint get_int(char opt, char *str, int min, int max);
double get_double(char opt, char *str, double min, double max);
void print_help_and_exit();

void error_and_exit(const char *msg);

void send_icmp_packet(int sd,
                      struct addrinfo *dst_addrinfo,
                      uint8_t *packet,
                      uint16_t packet_size);
void put_icmphdr(void *packet, uint16_t size);
uint16_t checksum(uint8_t *data, uint16_t size);

void receive_icmp_packet(int sd, uint8_t *rcvbuff, int rcvbuffsize);
void handle_packet(uint8_t *packet);
struct addrinfo *get_sender_addrinfo(char *addr);
void print_packet(struct ip *ip, struct icmp *icmp, double timerange);
void print_error_packet(struct ip *ip, struct icmp *nicmp, uint8_t error);
void get_source_name_and_addr(struct ip *ip, char *name, char *addr);

void display_stats(long time_elapsed);
void rtt_update(double val);
void init_state();
void sig_int_handler();

extern state_t state;

#endif // FT_PING_H
