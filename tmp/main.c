#include <stdlib.h>
#include <stdio.h>
#include <errno.h>


int main(int argc, char **argv){
    double d;

    printf("%d\n", sscanf(argv[1], "%lf", &d));
    printf("%lf\n", d);
    // printf("%d\n", errno);
}
