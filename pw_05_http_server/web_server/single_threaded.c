#include <stdio.h>
#include <stdlib.h>

#include <gmodule.h>
#include <unistd.h>
#include <netdb.h>

#include <err.h>
#include <errno.h>


#define PORT        "2048"
#define BACKLOG     5

#define INDEX       "index.html"

#define HTTP_200    "HTTP/1.1 200 OK\r\n\r\n"
#define HTTP_404    "HTTP/1.1 404 Not Found\r\n\r\n404 File Not Found."

#define BUFFER_SIZE 4096


extern int errno;

char * get_request(int cfd);
char * get_resource(char *request);
char * file_get_contents(const char *filename, size_t *size);


int main(void)
{
    char *request, *resource;

    char *contents;
    size_t size;

    int enable = 1;
    
    struct addrinfo hints;
    struct addrinfo *res, *rp;
    int sfd, ret;
    
    int cfd;


    printf("static server\n");


    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;


    ret = getaddrinfo(NULL, PORT, &hints, &res);
    if (ret)
        errx(EXIT_FAILURE, "getaddrinfo: %s", gai_strerror(ret));
    

    for (rp = res; rp != NULL; rp = rp->ai_next) {
        
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1)
            continue;

        ret = setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
        if (ret == -1)
            err(EXIT_FAILURE, "setsockopt: SO_REUSEADDR failure");

        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;

        close(sfd);
    }

    freeaddrinfo(res);
    
    if (!rp)
        errx(EXIT_FAILURE, "unsuccessful bind on port %s", PORT);


    if (listen(sfd, BACKLOG) == -1)
        err(EXIT_FAILURE, "listen");

    printf("bind and listening on port %s"
            " (queue limit: %d)\n", PORT, BACKLOG);


    for (;;) {
        
        cfd = accept(sfd, NULL, NULL);
        if (cfd == -1)
           err(EXIT_FAILURE, "accept");
        

        request = get_request(cfd);
        if (request) {
            
            resource = get_resource(request);
            
            printf("%d: %s\n", cfd, resource);
            
            if (g_strcmp0(resource, "slow.html") == 0) {
                
                printf("%d: server - sleeping for 10s\n", cfd);
                sleep(10);
            }

            
            contents = file_get_contents(resource, &size);
            if (!contents) {
                
                send(cfd, HTTP_404, sizeof(HTTP_404) - 1, 0);
            }
            else {


                send(cfd, HTTP_200, sizeof(HTTP_200) - 1, MSG_MORE);
                write(cfd, contents, size);

                g_free(contents);
            }

             
            g_free(resource); 
            g_free(request);
        }

        close(cfd);
    }
}


char * get_request(int cfd)
{
    char buffer[BUFFER_SIZE];
    int len;

    GString * request = g_string_new(NULL);
    gboolean valid = FALSE;


    while (!valid && (len = read(cfd, buffer, BUFFER_SIZE))) {
        
        if (len < 0) {

            g_string_free(request, TRUE);
            err(EXIT_FAILURE, "read in get_request");
        }
        
        g_string_append_len(request, buffer, len);

        valid = g_str_has_suffix(request->str, "\r\n\r\n");
    }

    return g_string_free(request, !valid);
}


char * get_resource(char *request)
{
    char *resource;
    size_t n;

    request += 5;
    n = 0;
    while (request[n] != ' ') {

        n++;
    }

    if (n == 0) {
        
        request = INDEX;
        n = sizeof(INDEX);
    }

    resource = g_strndup(request, n);

    return resource;
}


char * file_get_contents(const char *filename, size_t *size)
{
    char *path, *contents;

    path = g_strconcat("www/", filename, NULL);
    g_file_get_contents(path, &contents, size, NULL);

    g_free(path);

    return contents;
}
