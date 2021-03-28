#if !defined(FT_PING_H)
#define FT_PING_H

# include <sys/socket.h>
# include <sys/types.h>
# include <netdb.h>
# include <netinet/ip_icmp.h>
# include <arpa/inet.h>

#include "libft.h"

typedef struct
{
    int v_flag;
} state_t;

void error_and_exit(const char *msg);

#endif // FT_PING_H
