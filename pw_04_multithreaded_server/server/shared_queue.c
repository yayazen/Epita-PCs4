#include <errno.h>
#include <err.h>

#include "shared_queue.h"


extern int errno;

struct shared_queue* new_shared_queue(void)
{
    struct shared_queue *queue = calloc(1, sizeof(struct shared_queue));
    if (!queue)
        goto err;

    if (sem_init(&queue->size, 0, 0) == -1)
        goto err;
    
    if (sem_init(&queue->lock, 0, 1) == -1)
        goto err;


    return queue;

err:
    err(EXIT_FAILURE, "new_shared_queue");
}


void shared_queue_push(struct shared_queue *queue, int val)
{
    sem_wait(&queue->lock);

    struct queue *rear, *data;

    data = malloc(sizeof(struct queue));
    if (!data)
        err(EXIT_FAILURE, "shared_queue_push");

    data->val = val;

    rear = queue->store;
    if (!rear)
        data->next = data;
    else {
        data->next = rear->next;
        rear->next = data;
    }

    queue->store = data;

    sem_post(&queue->size);
    sem_post(&queue->lock);
}


int shared_queue_pop(struct shared_queue *queue)
{
    sem_wait(&queue->size);
    sem_wait(&queue->lock);

    struct queue *rear, *front;
    int val;

    rear = queue->store;
    if (!rear)
        errx(EXIT_FAILURE, "shared_queue_pop: empty queue");
    
    front = rear->next;
    if (rear == front)
        queue->store = NULL;
    else
        rear->next = front->next;

    val = front->val;
    free(front);

    sem_post(&queue->lock);

    return val;
}


void shared_queue_destroy(struct shared_queue *queue) {
    struct queue *ptr, *next;

    ptr = queue->store;
    if (ptr) {

        do {
            next = ptr->next;
            free(ptr);
        
            ptr = next;
        } while (ptr != queue->store);
    }

    free(queue);
}

