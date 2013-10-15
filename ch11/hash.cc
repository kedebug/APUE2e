#include <stdlib.h>
#include <pthread.h>

#define NHASH 29
#define HASH(fp)  (((unsigned long)fp) % NHASH)

typedef struct _foo {
    int f_count;
    int f_id;
    pthread_mutex_t f_lock;
    _foo *f_next;
} foo;

foo *fh[NHASH];
pthread_mutex_t hashlock = PTHREAD_MUTEX_INITIALIZER;

foo *foo_alloc(void)
{
    foo *fp = NULL;

    if ((fp = (foo *)malloc(sizeof(foo))) != NULL) {
        fp->f_count = 1;
        if (pthread_mutex_init(&fp->f_lock, NULL) != 0) {
            free(fp);
            return NULL;
        }
        
        int id = HASH(fp);
        pthread_mutex_lock(&hashlock);
        fp->f_next = fh[id];
        fh[id] = fp;
        pthread_mutex_lock(&fp->f_lock);
        pthread_mutex_unlock(&hashlock);

        // some initialization
        pthread_mutex_unlock(&fp->f_lock);
    }
    return fp;
}

void foo_hold(foo *fp)
{
    pthread_mutex_lock(&fp->f_lock);
    fp->f_count += 1;
    pthread_mutex_unlock(&fp->f_lock);
}

void foo_rele(foo *fp) 
{
    pthread_mutex_lock(&fp->f_lock);
    if (fp->f_count == 1) {
        pthread_mutex_unlock(&fp->f_lock);
        pthread_mutex_lock(&hashlock);
        pthread_mutex_lock(&fp->f_lock);
        
        if (fp->f_count != 1) {
            fp->f_count -= 1;
            pthread_mutex_unlock(&fp->f_lock);
            pthread_mutex_unlock(&hashlock);
            return;
        }

        int id = HASH(fp);
        foo *tfp = fh[id];
        if (tfp == fp) {
            tfp = tfp->f_next;
        } else {
            while (tfp != NULL && tfp->f_next != fp)
                tfp = tfp->f_next;
            if (tfp->f_next == fp)
                tfp->f_next = fp->f_next;
        }
        pthread_mutex_unlock(&hashlock);
        pthread_mutex_unlock(&fp->f_lock);
        pthread_mutex_destroy(&fp->f_lock);
        free(fp);
    } else {
        fp->f_count -= 1;
        pthread_mutex_unlock(&fp->f_lock);
    }
}
