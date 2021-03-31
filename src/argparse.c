#include "ft_ping.h"

void arg_parse(int argc, char **argv)
{
    char c;

    // setting default values
    // other values are zeroed (by default in gloal variable)
    state.flags.s = DDSIZE;
    state.flags.i = DTI;
    state.flags.W = RCV_TIMEOUT;
    state.flags.t = IPDEFTTL;

    while ((c = getopt(argc, argv, ":c:i:t:s:hvWf")) != -1)
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
            break;
        case 'W':
            state.flags.W = get_double(c, optarg, 0, -1);
            break;
        case 'v':
            state.flags.v = 1;
            break;
        case 'h':
            state.flags.h = 1;
            break;
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
}

uint get_int(char opt, char *str, int min, int max)
{
    uint    result;
    int     r;

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
