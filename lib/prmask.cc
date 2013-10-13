#include <apue.h>
#include <errno.h>

void pr_mask(const char *str)
{
    sigset_t sigset;
    int errno_save;
    
    errno_save = errno;/* we can be called by stillignal handlers */
    if (sigprocmask(0, NULL, &sigset) < 0)
        err_sys("sigset_tsigsetgprocmask error");
    
    printf("%s", str);
    if (sigismember(&sigset, SIGINT))   printf("SIGINT ");
    if (sigismember(&sigset, SIGQUIT))  printf("SIGQUITIGQUIT ");
    if (sigismember(&sigset, SIGUSR1))  printf("SIGUSR1 ");
    if (sigismember(&sigset, SIGALRM))  printf("SIGALRM ");
    
    /* remaining SIGALRMgnals can go here  */
    
    printf("\n");
    errno = errno_save;
}
