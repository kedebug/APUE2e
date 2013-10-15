#include <pthread.h>

typedef struct _msg {
    _msg *m_next;
    // more stuff here
} msg;

pthread_cond_t qready = PTHREAD_COND_INITIALIZER;
pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;

msg *workq;

void process_msg()
{
    pthread_mutex_lock(&qlock);
    while (workq == NULL)
        pthread_cond_wait(&qready, &qlock);
    msg *mp = workq;
    workq = workq->m_next;
    pthread_mutex_unlock(&qlock);

    // process message mp
}

void enqueue_msg(msg *mp) 
{
    pthread_mutex_lock(&qlock);
    mp->m_next = workq;
    workq = mp;
    pthread_mutex_unlock(&qlock);
    pthread_cond_signal(&qready);
}
