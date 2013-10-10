
#include <apue.h>
#include <fcntl.h>

char buf1[] = "abcdef";
char buf2[] = "ABCDEF";

int main() {
    int fd;
    if (fd = (creat("file.hold", FILE_MODE)) < 0) 
        err_sys("creat error");

    if (write(fd, buf1, 6) != 6)
        err_sys("write error");

    exit(0);
}
