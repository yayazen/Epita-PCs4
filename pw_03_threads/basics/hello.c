#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <pthread.h>

void* fn_thread(void* arg);

int main(int argc, char** argv)
{
    // Check the number of arguments.
    if (argc < 2)
        errx(EXIT_FAILURE, "The number of threads is missing.");
    else if (argc > 2)
        errx(EXIT_FAILURE, "Specify only the number of threads.");
    
    int rc;
    pthread_t *thrd;
    long nb_thrd;
    
    // - Convert the argument into a long integer.
    //   Use atol(3).
    //   This value represents the number of threads.
    // - If the argument is not valid (i.e. lower than or equal to zero),
    //   exit with an error message.
    if ((nb_thrd = atol(argv[1])) <= 0)
        errx(EXIT_FAILURE, "The number of threads is not valid.");

    // - Create and execute the threads.
    thrd = malloc(nb_thrd * sizeof(pthread_t));

    for (long t = 0; t < nb_thrd; t++) {
        //   If an error occurs, exit with an error message.
        //   You can use err(3), but the 'errno' variable is not set automatically.
        //   You have to set it manually to the return value of pthread_create(). 
        if ((rc = pthread_create(thrd + t, NULL, fn_thread, NULL)))
            errx(rc, "in pthread_create, has returned %d\n", rc);
    }
    // - Wait for the threads to end.
    // - Return from the function.

    free(thrd);
    pthread_exit(0);
}

// Define the thread function.
void* fn_thread(void* arg __attribute__((unused)))
{
    // - Print a message.
    printf("Hello from thread!\n");

    // - Return from the function.
    pthread_exit(NULL);
}
