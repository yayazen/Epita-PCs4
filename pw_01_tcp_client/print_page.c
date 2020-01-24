#define _GNU_SOURCE

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

char *build_query(const char *host, size_t *len)
{
    char *query;
    *len = asprintf(&query, "GET http://www.%s HTTP/1.0\r\n\r\n", host);
    
    return query;
}

void print_page(const char *host)
{
    char *query;
    char buffer[BUFFER_SIZE + 1] = {'\0'};
    size_t len;


    struct addrinfo hints;
    struct addrinfo *res, *rp;
    int sfd, err;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;


    if ((err = getaddrinfo(host, "80", &hints, &res)))
        errx(EXIT_FAILURE, "getaddrinfo: %s", gai_strerror(err));

    for (rp = res; rp != NULL; rp = rp->ai_next) {
        if ((sfd = socket(res->ai_family, res->ai_socktype, 
                       res->ai_protocol)) == -1)
            continue;

        if (connect(sfd, res->ai_addr, res->ai_addrlen) == 0)
            break;

        close(sfd);
    }

    if (!rp)
        errx(EXIT_FAILURE, "connect: no address succeeded");

    freeaddrinfo(res);


    query = build_query(host, &len);
    if (write(sfd, query, len) != (ssize_t)len)
       errx(EXIT_FAILURE, "query: partial/failed write to socket stream");  
 
    while ((err = read(sfd, buffer, BUFFER_SIZE))) {
        if (err == -1)
            errx(EXIT_FAILURE, "read: partial/failed read from socket stream");
        
        printf("%s", buffer);
    }


    free(query);
    close(sfd);
}
