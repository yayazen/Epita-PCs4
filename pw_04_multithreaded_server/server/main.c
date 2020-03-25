#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netdb.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include <signal.h>
#include <err.h>
#include <errno.h>

#include "shared_queue.h"


#define INET_ADDRLEN    16
#define BACKLOG         5

#define NUM_THREADS     4

#define BUFFER_SIZE     512


extern int errno;

void echo(int fd_in, int fd_out);

void inet_str(struct in_addr *in, char *buffer);


void *worker(void *arg) {
    int cfd;
    struct shared_queue *queue = (struct shared_queue *)arg;
    
    for (;;) {
        cfd = shared_queue_pop(queue);
        
        echo(cfd, cfd);

        close(cfd);
    }
}


int main(int argc, char** argv)
{
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage:\n"
            "Arg 1 = Port number (e.g. 2048)");


    struct shared_queue *queue;
    pthread_t threads[NUM_THREADS];

    int enable = 1;

    struct addrinfo hints;
    struct addrinfo *res, *rp;
    int sfd, ret;
    
    struct sockaddr_in client;
    char straddr[INET_ADDRLEN];
    socklen_t addrlen;
    int cfd;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;


    ret = getaddrinfo(NULL, argv[1], &hints, &res);
    if (ret)
        errx(EXIT_FAILURE, "getaddrinfo: %s", gai_strerror(ret));

    for (rp = res; rp != NULL; rp = rp->ai_next) {
        
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1)
            continue;
        
        ret = setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
        if (ret)
            err(EXIT_FAILURE, "setsockopt: SO_REUSEADDR failure");

        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;
        
        close(sfd);
    }

    freeaddrinfo(res);

    if (!rp)
        errx(EXIT_FAILURE, "bind: unsuccessful on port %s", argv[1]);


    if (listen(sfd, BACKLOG) == -1)
        err(EXIT_FAILURE, "listen");

    printf("bind and listening on port %s\n", argv[1]);


    queue = new_shared_queue();
    
    for (size_t tid = 0; tid < NUM_THREADS; tid++) {
        
        errno = pthread_create(threads + tid, NULL, worker, queue);
        if (errno)
            err(EXIT_FAILURE, "pthread_create");
    }


    for (;;) {

        addrlen = sizeof client;
        
        cfd = accept(sfd, (struct sockaddr *)&client, &addrlen);
        if (cfd == -1)
            err(EXIT_FAILURE, "accept");

        inet_str(&client.sin_addr, straddr);  
        printf("connection from [%s] %d\n", straddr, client.sin_port);

        shared_queue_push(queue, cfd);
    }
}


void inet_str(struct in_addr *in, char *buffer)
{
    char *bytes = (char *) &in->s_addr;

    sprintf(buffer, "%d.%d.%d.%d",
        bytes[0], bytes[1], bytes[2], bytes[3]);
}


void echo(int fd_in, int fd_out)
{
    char buffer[BUFFER_SIZE];

    int r, w;
    int offset;

    while ((r = read(fd_in, buffer, BUFFER_SIZE - 1)) > 0) {

        if (r == -1)
            err(EXIT_FAILURE, "read");

        offset = 0;
        while (offset != r) {

            if ((w = write(fd_out, buffer + offset, r)) == -1)
                err(EXIT_FAILURE, "write");

            offset += w;
        }
    }
}
