#include <stdlib.h>
#include <pthread.h>

typedef struct _job {
    _job *j_next;
    _job *j_prev;
    pthread_t j_id;
    // more stuff here
} job;

typedef struct _queue {
    job *q_head;
    job *q_tail;
    pthread_rwlock_t q_lock;
} queue;

int queue_init(queue *qp) 
{
    qp->q_head = NULL;
    qp->q_tail = NULL;

    int err = pthread_rwlock_init(&qp->q_lock, NULL);
    if (err != 0) 
        return err;

    // some initialization

    return 0;
}

void queue_insert(queue *qp, job *jp)
{
    pthread_rwlock_wrlock(&qp->q_lock);
    jp->j_next = qp->q_head;
    jp->j_prev = NULL;
    if (qp->q_head != NULL) 
        qp->q_head->j_prev = jp;
    else
        qp->q_tail = jp;
    qp->q_head = jp;
    pthread_rwlock_unlock(&qp->q_lock);
}

void job_append(queue *qp, job *jp)
{
    pthread_rwlock_wrlock(&qp->q_lock);
    jp->j_prev = qp->q_tail;
    jp->j_next = NULL;
    if (qp->q_tail != NULL) 
        qp->q_tail->j_next = jp;
    else
        qp->q_head = jp;
    qp->q_tail = jp;
    pthread_rwlock_unlock(&qp->q_lock);
}

void job_remove(queue *qp, job *jp) 
{
    pthread_rwlock_wrlock(&qp->q_lock);
    if (jp == qp->q_head) {
        qp->q_head = jp->j_next;
        if (qp->q_tail == jp)
            qp->q_tail = NULL;
    } else if (jp == qp->q_tail) {
        qp->q_tail = jp->j_prev;
        if (qp->q_head == jp)
            qp->q_head = NULL;
    } else {
        jp->j_prev->j_next = jp->j_next;
        jp->j_next->j_prev = jp->j_prev;
    }
    pthread_rwlock_unlock(&qp->q_lock);
}

job *job_find(queue *qp, pthread_t tid) 
{
    pthread_rwlock_rdlock(&qp->q_lock);
    job *jp;
    for (jp = qp->q_head; jp != qp->q_tail; jp = jp->j_next)
        if (jp->j_id == tid)
            break;
    pthread_rwlock_unlock(&qp->q_lock);
    return jp;
}
