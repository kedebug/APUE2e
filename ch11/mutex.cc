#include <apue.h>
#include <pthread.h>

typedef struct _foo {
    int f_count;
    pthread_mutex_t f_lock;
} foo;

foo *foo_alloc(void)
{
    foo *fp;

    if ((fp = (foo *)malloc(sizeof(foo))) != NULL) {
        fp->f_count = 1;
        if (pthread_mutex_init(&fp->f_lock, NULL) != 0) {
            free(fp);
            return NULL;
        }
        // todo
    }
    return fp;
}

void foo_hold(foo *fp) 
{
    pthread_mutex_lock(&fp->f_lock);
    fp->f_count += 1;
    pthread_mutex_unlock(&fp->f_lock);
}

void foo_release(foo *fp) 
{
    pthread_mutex_lock(&fp->f_lock);
    if (--fp->f_count == 0) {
        pthread_mutex_unlock(&fp->f_lock);
        pthread_mutex_destroy(&fp->f_lock);
        free(fp);
    } else {
        pthread_mutex_unlock(&fp->f_lock);
    }
}
