#include "ft_ping.h"

void display_stats(long time_elapsed)
{
    int loss = 100 - ((double)state.nreceived / state.nsent) * 100;

    printf("--- %s ping statistics ---\n", state.dst_canonical_name);
    printf("%d packets transmitted, %d received, %d%% packet loss, time %ldms\n",
           state.nsent,
           state.nreceived,
           loss,
           time_elapsed);
}
