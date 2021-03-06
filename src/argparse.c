#include "ft_ping.h"

void arg_parse(int argc, char **argv)
{
    char c;

    while ((c = getopt(argc, argv, ":c:i:t:s:W:hvf")) != -1)
        switch (c)
        {
        case 'c':
            state.flags.c = get_int(c, optarg, 1, -1);
            break;
        case 't':
            state.flags.t = get_int(c, optarg, 1, MAXTTL);
            break;
        case 's':
            state.flags.s = get_int(c, optarg, 0, -1);
            break;
        case 'f':
            state.flags.f = 1;
            break;
        case 'i':
            state.flags.i = get_double(c, optarg, 0, -1);
            state.flags.i_s = FALSE;
            break;
        case 'W':
            state.flags.W = get_double(c, optarg, 0, -1);
            break;
        case 'v':
            state.flags.v = 1;
            break;
        case 'h':
            state.flags.h = 1;
            print_help_and_exit();
        case '?':
            fprintf(stderr, "error : unknown option `-%c'.\n", optopt);
            exit(EXIT_FAILURE);
        case ':':
            fprintf(stderr, "error : option -%c requires an argument.\n", optopt);
            exit(EXIT_FAILURE);
        }
    if (optind == argc)
    {
        fprintf(stderr, "error : destination address required\n");
        exit(EXIT_FAILURE);
    }
    state.dst = argv[optind];
    if (state.flags.f && !state.flags.i_s)
        state.flags.i = 0;
}

uint get_int(char opt, char *str, int min, int max)
{
    int result;
    int r;

    r = sscanf(str, "%u", &result);
    if (r != 1)
    {
        fprintf(stderr, "error : invalid argument to -%c : '%s'\n", opt, str);
        exit(EXIT_FAILURE);
    }
    if (min > -1 && result < min)
    {
        fprintf(stderr, "error : invalid argument to -%c : min value is %d while %d specified.\n", opt, min, result);
        exit(EXIT_FAILURE);
    }
    if (max > -1 && result > max)
    {
        fprintf(stderr, "error : invalid argument to -%c : max value is %d while %d specified.\n", opt, max, result);
        exit(EXIT_FAILURE);
    }

    return result;
}

double get_double(char opt, char *str, double min, double max)
{
    double result;
    int r;

    r = sscanf(str, "%lf", &result);
    if (r != 1)
    {
        fprintf(stderr, "error : invalid argument to -%c : '%s'\n", opt, str);
        exit(EXIT_FAILURE);
    }
    if (min > -1 && result < min)
    {
        fprintf(stderr, "error : invalid argument to -%c : min value is %lf while %lf specified.\n", opt, min, result);
        exit(EXIT_FAILURE);
    }
    if (max > -1 && result > max)
    {
        fprintf(stderr, "error : invalid argument to -%c : max value is %lf while %lf specified.\n", opt, max, result);
        exit(EXIT_FAILURE);
    }
    return result;
}

void print_help_and_exit()
{
    printf("\
Usage\n\
    ping [options] <destination>\n\
\n\
    -f              flood ping\n\
    -h              print help and exit\n\
    -i <interval>   seconds between sending each packet\n\
    -s <size>       use <size> as number of data bytes to be sent\n\
    -t <ttl>        define time to live\n\
    -v              verbose output\n\
    -W <timeout>    time to wait for response\n\
");
    exit(2);
}