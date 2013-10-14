#include <apue.h>
#include <pthread.h>

void cleanup(void *arg)
{
    printf("cleanup: %s\n", (char *)arg);    
}

void *thr_fn1(void *arg)
{
    printf("thread 1 start...\n");
    
    pthread_cleanup_push(cleanup, (void *)"thread 1 first cleanup handler");
    pthread_cleanup_push(cleanup, (void *)"thread 1 second cleanup handler");

    printf("thread 1 push complete\n");

    if (arg)         
        return (void *)1;
    
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);

    return (void *)1;
}

void *thr_fn2(void *arg)
{
    printf("thread 2 start...\n");
    
    pthread_cleanup_push(cleanup, (void *)"thread 2 first cleanup handler");
    pthread_cleanup_push(cleanup, (void *)"thread 2 second cleanup handler");

    printf("thread 2 push complete\n");

    if (arg)         
        pthread_exit((void *)2);
    
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);

    pthread_exit((void *)2);
}

int main()
{
    int err;
    pthread_t tid1, tid2;

    err = pthread_create(&tid1, NULL, thr_fn1, (void *)1);
    if (err) err_quit("cann't creat thread 1: %s\n", strerror(err));

    err = pthread_create(&tid2, NULL, thr_fn2, (void *)1);
    if (err) err_quit("cann't creat thread 2: %s\n", strerror(err));

    void *tret = NULL;
    err = pthread_join(tid1, &tret);
    if (err) err_quit("cann't join with thread 1: %s\n", strerror(err));
    printf("thread 1 exit code: %ld\n", (long)tret);

    err = pthread_join(tid2, &tret);
    if (err) err_quit("cann't join with thread 2: %s\n", strerror(err));
    printf("thread 2 exit code: %ld\n", (long)tret);

    exit(0);  
}
