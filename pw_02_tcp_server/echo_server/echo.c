#include <stdlib.h>
#include <unistd.h>
#include <err.h>

#define BUFFER_SIZE 512

void echo(int fd_in, int fd_out)
{
    char buffer[BUFFER_SIZE];

    int r, w;
    int offset;

    while ((r = read(fd_in, buffer, BUFFER_SIZE - 1)) > 0) {
        if (r == -1)
            errx(EXIT_FAILURE, "read: Could not read from input stream");

        offset = 0;
        while (offset != r) {

            if ((w = write(fd_out, buffer + offset, r)) == -1)
                errx(EXIT_FAILURE, "write: Could not write to output stream");

            offset += w;
        }
    }
}
