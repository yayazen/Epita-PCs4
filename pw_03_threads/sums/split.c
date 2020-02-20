#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <err.h>

//#define INITIALIZE_ARRAY

extern int errno;

struct thread_data
{
    long id;                    // Thread ID.
    pthread_t sys_id;           // Thread system ID.
    unsigned char *start;       // Points to the first element to add.
    long size;                  // Number of elements to add.
    long sum;                   // Sum of the elements.
};

// Return the sum of an array of bytes.
// 'start' points to the first element of an array.
// 'size' is the number of elements of the array.
unsigned long linear_sum(unsigned char *start, long size)
{
    unsigned long sum = 0;
    for (long n = 0; n < size; n++) {

        sum += start[n];
    }

    return sum;
}

// Define the thread function.
void * worker(void *arg)
{
    struct thread_data *data = (struct thread_data *)arg;

    data->sum = linear_sum(data->start, data->size);
    printf("Thread %02ld: %ld\n", data->id, data->sum);

    pthread_exit(NULL);
}

int main(int argc, char **argv)
{
    // Get the arguments.
    // argv[1] = array_size = the size of the array of bytes (greater than 63).
    // argv[2] = thread_number = the number of threads (between 1 and 16).
    // -----------------------------------------------------------------------

    if (argc != 3)
        errx(EXIT_FAILURE, "Usage: array_size thread_number");

    long array_size = atol(argv[1]);
    if (array_size <= 63)
        errx(EXIT_FAILURE, "The size of the array is not valid (must be greater than 63).");

    long thread_number = atol(argv[2]);
    if (thread_number < 1 || thread_number > 16)
        errx(EXIT_FAILURE, "The number of threads is not valid (between 1 and 16).");

    // -----------------------------------------------------------------------

    // Data for the threads.
    struct thread_data data[thread_number];

    // Allocate the array of bytes.
    unsigned char *bytes = malloc(array_size);
    if (bytes == NULL)
        errx(EXIT_FAILURE, "Not enough memory!\nTry to reduce the size of the array.");

#ifdef INITIALIZE_ARRAY
    // Initialize the array.
    printf("Initializing array........");
    fflush(stdout);
    for (long i = 0; i < array_size; i++)
        bytes[i] = 1;
    printf("OK\n");
#endif
    
    long sum;

    long tid;
    long chunk = array_size / thread_number;


    for (tid = 0; tid < thread_number; tid++) {

        data[tid].id = tid;
        
        data[tid].start = bytes + (chunk * tid);
        data[tid].size = (tid != thread_number - 1)
            ? chunk
            : array_size - (chunk * tid);


        errno = pthread_create(&data[tid].sys_id, NULL, worker, data + tid);
        if (errno) {
            err(EXIT_FAILURE, "pthread_create");
        }
    }


    sum = 0;
    for (tid = 0; tid < thread_number; tid++) {
        
        pthread_join(data[tid].sys_id, NULL);

        sum += data[tid].sum;
    }

    printf("Sum......: %ld", sum);

    
    free(bytes);

    return EXIT_SUCCESS;
}
