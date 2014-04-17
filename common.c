/***********************************************************************
 *      Contains the definition of some common used functions
 * ********************************************************************/
#include <errno.h>
#include "define.h"

const char* program_name;

void system_error (const char* operation) 
{
    /* Generate an error message for errno */
    error (operation, strerror (errno));
}

void error (const char* cause, const char* message)
{
    /* Print an error meassage to stderr */
    fprintf (stderr, "%s: error: (%s) %s\n", program_name, cause, message);
    exit(1);
}

void* xmalloc (size_t size) {
    void* ptr = malloc (size);
    if (ptr == NULL) {
        fprintf (stderr, "Error in dynamic memory allocation - aborting\n");
        abort();
    }
    else
        return ptr;
}
