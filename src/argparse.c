#include "ft_ping.h"

void arg_parse(int argc, char **argv)
{
    char c;

    // setting default values
    state.flags.i = 1;

    while ((c = getopt(argc, argv, ":c:i:t:s:")) != -1)
        switch (c)
        {
        case 'c':
            state.flags.c = get_int(c, optarg);
            break;
        case 'i':
            state.flags.i = get_double(c, optarg);
            break;
        case 't':
            state.flags.t = get_int(c, optarg);
            break;
        case 's':
            state.flags.s = get_int(c, optarg);
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
        fprintf(stderr, "error : destination address required\n", optopt);
        exit(EXIT_FAILURE);
    }
    state.dst = argv[optind];
}

uint get_int(char opt, char *str)
{
    uint result;

    if (!ft_isnumeric_str(str))
    {
        fprintf(stderr, "error : invalid argument to -%c : '%s'\n", opt, str);
        exit(EXIT_FAILURE);
    }
    result = ft_atoi(str);
    if (result < 1)
    {
        fprintf(stderr, "error : invalid argument to -%c : '%s' is less than the min value '1'\n", opt, str);
        exit(EXIT_FAILURE);
    }
    return result;
}

double get_double(char opt, char *str)
{
    double result;
    int r;

    r = sscanf(str, "%lf", &result);
    if (r != 1)
    {
        fprintf(stderr, "error : invalid argument to -%c : '%s'\n", opt, str);
        exit(EXIT_FAILURE);
    }
    return result;
}