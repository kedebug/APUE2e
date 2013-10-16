#include <apue.h>
#include <pthread.h>
#include <signal.h>

pthread_mutex_t lock_flag = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_wait = PTHREAD_COND_INITIALIZER;

int quitflag;
sigset_t mask;

void *thr_fn(void *arg)
{
    int err, signo;

    while (true) {
        err = sigwait(&mask, &signo);
        if (err) err_exit(err, "sigwait failed");

        switch (signo) {
        case SIGINT:
            printf("\ninterrupt\n");
            break;
        case SIGQUIT:
            pthread_mutex_lock(&lock_flag);
            quitflag = 1;
            pthread_mutex_unlock(&lock_flag);
            pthread_cond_signal(&cond_wait);
            return 0;
        default:
            printf("unexpected signal: %d\n", signo);
            exit(1);
        }
    }
}

int main() 
{
    sigset_t oldmask;
    pthread_t tid;
    int err;

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGQUIT);
    err = pthread_sigmask(SIG_BLOCK, &mask, &oldmask);
    if (err) err_exit(err, "SIG_BLOCK error");

    err = pthread_create(&tid, NULL, thr_fn, NULL);
    if (err) err_exit(err, "can't create thread");

    pthread_mutex_lock(&lock_flag);
    while (quitflag == 0)
        pthread_cond_wait(&cond_wait, &lock_flag);
    pthread_mutex_unlock(&lock_flag);

    err = sigprocmask(SIG_SETMASK, &oldmask, NULL);
    if (err) err_sys("SIG_SETMASK error");

    exit(0);
}
