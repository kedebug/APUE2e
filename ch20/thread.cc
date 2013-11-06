#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/time.h>

void* thr_fn(void* arg) {
    printf("pid=%X\n", syscall(SYS_gettid));
    while (true) {
        struct timeval tstart, tend;
        gettimeofday(&tstart, NULL);
        int k = 0;
        for (size_t i = 0; i < INT_MAX / 2; i++)
            k++;
        gettimeofday(&tend, NULL);
        long int uses = 1000000 * (tend.tv_sec - tstart.tv_sec) + 
          (tend.tv_usec - tstart.tv_usec);
        printf("loop time: %lld\n", uses); 
    } 
}

int main() {
    pthread_t thr1, thr2, thr3;
    pthread_create(&thr1, NULL, thr_fn, NULL);
    //pthread_create(&thr2, NULL, thr_fn, NULL);
    //pthread_create(&thr3, NULL, thr_fn, NULL);
    pthread_join(thr1, NULL);
    //pthread_join(thr2, NULL);
}
