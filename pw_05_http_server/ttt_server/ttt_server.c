#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <gmodule.h>
#include <glib/gprintf.h>
#include <netdb.h>

#include <err.h>
#include <errno.h>


#define PORT        "2048"
#define BACKLOG     5

#define HTTP_200    "HTTP/1.1 200 OK\r\n\r\n"

#define BUFFER_SIZE 512


char grid[] = "_________";

int player_count = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


extern int errno;

void * client_handler(void *arg);
void command_handler(int cfd, char *cmd);

void sendf(int cfd, const char *filename, ...);
void send_response(int cfd, const char *response, size_t size);

char * get_request(int cfd);
char * get_resource(char *request);
char * file_get_contents(const char *filename, size_t *size);


int main(void)
{
    int enable = 1;
    
    struct addrinfo hints;
    struct addrinfo *res, *rp;
    int sfd, ret;
    
    int cfd;


    printf("dynamic server\n");


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
        errx(EXIT_FAILURE, "bind: unsuccessful on port %s", PORT);


    if (listen(sfd, BACKLOG) == -1)
        err(EXIT_FAILURE, "listen");

    printf("bind and listening on port %s"
            " (queue limit: %d)\n", PORT, BACKLOG);


    for (;;) {
        
        cfd = accept(sfd, NULL, NULL);
        if (cfd == -1)
           err(EXIT_FAILURE, "accept");


        pthread_t thread;

        errno = pthread_create(&thread, NULL, client_handler, (void *) (long) cfd);
        if (errno)
            err(EXIT_FAILURE, "pthread_create");

        errno = pthread_detach(thread);
        if (errno)
            err(EXIT_FAILURE, "pthread_detach");
    }
}


void * client_handler(void *arg)
{
    int cfd;

    char *request, *resource;

    char *contents; 
    size_t size;

    
    cfd = (int) (long) arg;

    request = get_request(cfd);
    if (request) {
        
        resource = get_resource(request);

        printf("%d: %s\n", cfd, resource);
            
        
        contents = file_get_contents(resource, &size);
        if (!contents) {
            
            command_handler(cfd, resource);        
        }
        else {
            
            send_response(cfd, contents, size);

            g_free(contents);
        }
        
        
        g_free(resource); 
        g_free(request);
    }

    close(cfd);

    pthread_exit(NULL);
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


void send_response(int cfd, const char *response, size_t size)
{
    send(cfd, HTTP_200, sizeof(HTTP_200) - 1, MSG_MORE);
    write(cfd, response, size);
}


void sendf(int cfd, const char *filename, ...)
{
    va_list args;

    char *contents;

    char *response;
    size_t size;


    contents = file_get_contents(filename, &size);
    if (!contents)
        errx(EXIT_FAILURE, "%s: no such file in www/", filename);


    va_start(args, filename);
    size = g_vasprintf(&response, contents, args);
    va_end(args);
    
    send_response(cfd, response, size);
    
    g_free(contents);
    g_free(response);
}


void command_handler(int cfd, char *cmd)
{
    char *nickname, symbol;

    if (g_strcmp0(cmd, "update") == 0) {
        
        send_response(cfd, grid, 9);
    }
    else if (g_str_has_prefix(cmd, "set_")) {

        grid[cmd[5] - '0'] = cmd[4];
         
        send_response(cfd, grid, 9);
    }
    else if (g_str_has_prefix(cmd, "grid?nickname=")) {
        
        if (player_count > 1) {
        
            sendf(cfd, "busy.html");
        }
        else {
            
            pthread_mutex_lock(&mutex);
        
            player_count++;

            nickname = cmd + 14;
            symbol = (player_count == 1) ?
                'x':
                'o';

            pthread_mutex_unlock(&mutex);
        
            sendf(cfd, "grid.html", symbol, nickname);
        }
    }
    else if (g_strcmp0(cmd, "restart") == 0) {
        
        pthread_mutex_lock(&mutex);

        if (player_count == 2) {

            memset(grid, '_', 9);
            
            player_count = 1;
        }
        else {
            
            player_count = 2;
        }
        
        pthread_mutex_unlock(&mutex);
    
        send_response(cfd, grid, 9);
    }
    else {

        if (player_count < 2)
            sendf(cfd, "new_player.html");
        else
            sendf(cfd, "busy.html");
    }
}
