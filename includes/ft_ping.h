#if !defined(FT_PING_H)
#define FT_PING_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>

#include "libft.h"

typedef struct
{
    uint c;
    uint t;
    double i;
    uint s;
} flags_t;

typedef struct
{
    flags_t flags;
    char    *dst;
    int     nreceived;
    int     nsent;
} state_t;

extern state_t state;

void arg_parse(int argc, char **argv);
uint get_int(char opt, char *str);
double get_double(char opt, char *str);

void error_and_exit(const char *msg);

#endif // FT_PING_H
