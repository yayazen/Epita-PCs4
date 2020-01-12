#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <err.h>

#define BACKLOG 5
#define EXPRESSION_SIZE 128

void bc(int fd_in, int fd_out)
{
    dup2(fd_in, STDIN_FILENO);
    dup2(fd_out, STDOUT_FILENO);
    dup2(fd_out, STDERR_FILENO);

    execlp("bc", "bc", (char *) NULL);
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


    for (;;) {
        if ((cfd = accept(sfd, NULL, 0)) == -1)
            errx(EXIT_FAILURE, "accept: unblocked without any connections");

        if ((pid = fork()) == -1)
            errx(EXIT_FAILURE, "fork: failed to create child process");


        if (!pid) {
            close(sfd);
            
            bc(cfd, cfd);

            close(cfd);
            return EXIT_SUCCESS;
        }
        
        else { 
            signal(SIGCHLD, SIG_IGN);
            close(cfd);
        }
    }
}
