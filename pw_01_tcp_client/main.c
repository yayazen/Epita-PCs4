#include "print_page.h"
#include <stdlib.h>
#include <err.h>

int main(int argc, char **argv) 
{
    if (argc != 2)
        errx(EXIT_FAILURE, "(Argument error)");
    
    print_page(argv[1]);

    return EXIT_SUCCESS;
}
