#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <err.h>

extern int errno;

void* fn_thread(void* arg);


int main(int argc, char** argv)
{
    // Check the number of arguments.
    if (argc < 2)
        errx(EXIT_FAILURE, "The number of threads is missing.");
    else if (argc > 2)
        errx(EXIT_FAILURE, "Specify only the number of threads.");

    
    long nb_threads;
    if ((nb_threads = atol(argv[1])) <= 0)
        errx(EXIT_FAILURE, "The number of threads is not valid.");
    

    pthread_t threads[nb_threads];
    long tid;

    for (tid = 0; tid < nb_threads; tid++) {
        
        errno = pthread_create(threads + tid, NULL, fn_thread, (void *)tid);

        if (errno) {
            close(STDOUT_FILENO);
            err(EXIT_FAILURE, "pthread_create");
        }
    }


    for (tid = 0; tid < nb_threads; tid++) {

        pthread_join(threads[tid], NULL);
    }


    return EXIT_SUCCESS;
}

// Define the thread function.
void* fn_thread(void *arg)
{
    sleep(rand() % 3);

    long tid = (long)arg;
    printf("Hello from thread %ld!\n", tid);

    pthread_exit(NULL);
}
