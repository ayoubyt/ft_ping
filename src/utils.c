#include "ft_ping.h"

void display_stats(long time_elapsed)
{
    int loss = 100 - ((double)state.nreceived / state.nsent) * 100;

    printf("\n--- %s ping statistics ---\n", state.dst_canonical_name);
    printf("%d packets transmitted, %d received, %d%% packet loss, time %ldms\n",
           state.nsent,
           state.nreceived,
           loss,
           time_elapsed);
    if (state.nreceived > 0)
    {
        printf("rtt min/avg/max/ = %.3lf/%.3lf/%.3lf ms\n",
               state.rtt.min,
               state.rtt.sum / state.nreceived,
               state.rtt.max);
    }
}

void rtt_update(double val)
{
    if (val > state.rtt.max)
        state.rtt.max = val;
    if (val < state.rtt.min)
        state.rtt.min = val;
    state.rtt.sum += val;
}

/*
** set default values for state struct
** any unintilized value here is set to zero
** as 'c' language zeroes global variables
*/

void init_state()
{
    state.rtt.min = __DBL_MAX__;

    state.flags.s = DDSIZE;
    state.flags.i = DTI;
    state.flags.W = RCV_TIMEOUT;
    state.flags.t = IPDEFTTL;
}

/*
** a function to stop packet sending loop
** when SIGINT (CTRL-c) is ent to the calling process
*/

void sig_int_handler(int signal)
{
    state.loop = 0;
}