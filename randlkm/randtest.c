#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    char data[512] = "";
    int len = 0;
    int fd = open("/dev/" "randlkm", O_RDWR);

    if (fd < 0)
    {
        printf("open() error.\n");
        return -1;
    }

    if (read(fd, data, len) < 0)
    {
        printf("read() error.\n");
        return -1;
    }

    printf("Read: %s.\n", data);
    close(fd);

    return 0;
}