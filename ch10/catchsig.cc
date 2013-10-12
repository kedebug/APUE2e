#include <apue.h>

static void sig_usr(int signo) {
    if (signo == SIGUSR1)
        printf("SIGUSR1 recieved\n");
    else if (signo == SIGUSR2)
        printf("SIGUSR2 recieved\n");
    else
        err_dump("recieved signal: %d\n", signo);
}

int main() {
    if (signal(SIGUSR1, sig_usr) == SIG_ERR)
        err_sys("Cann't catch SIGUSR1\n");
    if (signal(SIGUSR2, sig_usr) == SIG_ERR)
        err_sys("Cann't catch SIGUSR2\n");
    while (true)
        pause();
}
