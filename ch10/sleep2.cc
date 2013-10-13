#include <apue.h>
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>

static jmp_buf env_alrm;

static void sig_alrm(int signo)
{
    longjmp(env_alrm, 1);
}

unsigned int sleep2(unsigned int nsecs)
{
    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
        return(nsecs);
    if (setjmp(env_alrm) == 0) {
        alarm(nsecs);
        pause();
    }
    return(alarm(0));
}

static void sig_int(int signo) 
{
    int i, j;
    volatile int k;

    printf("\nsig_int starting\n");
    k = 0;
    for (i = 0; i < 100000; i++)
        for (j = 0; j < 100000; j++)
            k += i * j;
    printf("sig_int finished\n");
}

int main(void)
{
    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("signal(SIGINT) error");

    unsigned int unslept = sleep2(5);
    printf("sleep2 returned: %u\n", unslept);

    exit(0);
}
