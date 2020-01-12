#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <err.h>
#include "echo.h"

#define INET_ADDRLEN 16
#define BACKLOG 5


void inet_str(struct in_addr *in, char *buffer)
{
    char *bytes = (char *) &in->s_addr;

    sprintf (buffer, "%d.%d.%d.%d",
        bytes[0], bytes[1], bytes[2], bytes[3]);
}



int main(int argc, char** argv)
{
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage:\n"
            "Arg 1 = Port number (e.g. 2048)");

    
    pid_t pid;

    struct addrinfo hints;
    struct addrinfo *res, *rp;
    int sfd, err;
    
    struct sockaddr_in client;
    char straddr[INET_ADDRLEN];
    socklen_t addrlen;
    int cfd;
    
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;



    if ((err = getaddrinfo(NULL, argv[1], &hints, &res)))
        errx(EXIT_FAILURE, "getaddrinfo: %s", gai_strerror(err));

    for (rp = res; rp != NULL; rp = rp->ai_next) {
        if ((sfd = socket(rp->ai_family, rp->ai_socktype,
                     rp->ai_protocol)) == -1)
            continue;
        
        setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, NULL, 0);

        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;
        
        close(sfd);
    }

    if (!rp)
        errx(EXIT_FAILURE, "bind: unsuccessful on port %s", argv[1]);

    freeaddrinfo(res);



    if (listen(sfd, BACKLOG) == -1)
        errx(EXIT_FAILURE, "listen: failed to set passive mark");

    printf("bind and listening on port %s "
           "(queue limit: %d)\n", argv[1], BACKLOG);


    for (;;) {
        addrlen = sizeof client;
        if ((cfd = accept(sfd, (struct sockaddr *)&client, &addrlen)) == -1)
            errx(EXIT_FAILURE, "accept: unblocked without any connections");

        if ((pid = fork()) == -1)
            errx(EXIT_FAILURE, "fork: failed to create child process");


        if (!pid) {
            close(sfd);
            
            inet_str(&client.sin_addr, straddr);
            printf("(pid = %d) connected from [%s] %d\n", 
                        getpid(), straddr, client.sin_port);

            echo(cfd, cfd);

            close(cfd);
            printf("(pid = %d) from [%s] %d has closed connection\n", 
                        getpid(), straddr, client.sin_port);

            return EXIT_SUCCESS;
        }
        
        else { 
            signal(SIGCHLD, SIG_IGN);
            close(cfd);
        }
    }
}
