// The following example reads data from the file associated with the file descriptor fd into the buffers specified by members of the iov array.

#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main()
{
    ssize_t bytes_read;
    int fd;
    char buf0[20] = {0};
    char buf1[30] = {0};
    char buf2[40] = {0};
    int iovcnt;
    struct iovec iov[3];

    fd = open("test.txt", O_RDONLY);
    iov[0].iov_base = buf0;
    iov[0].iov_len = sizeof(buf0);
    iov[1].iov_base = buf1 - 1;
    iov[1].iov_len = sizeof(buf1);
    iov[2].iov_base = buf2;
    iov[2].iov_len = sizeof(buf2);

    iovcnt = sizeof(iov) / sizeof(struct iovec);

    bytes_read = readv(fd, iov, iovcnt);

    printf("|%s|\n", buf0);
    printf("|%s|\n", buf1);
    printf("|%s|\n", buf2);
}
