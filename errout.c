#include "nrm.h"
#include <string.h>

void errout(s1, s2, s3, s4) /* print errors conditionally on fflag & iflag*/
char    *s1, *s2, *s3, *s4;
{
    extern int  iflag;
    extern int  fflag;
    extern int  errno;

    /* for Linux removed declarations -> now in stdio.h */
    /* extern int  sys_nerr; */
    /* extern char *sys_errlist[];  */

    if (fflag || iflag) {    /* verbose or interactive */
        fprintf(stderr, s1, s2, s3, s4);
        fprintf(stderr, ": %s\n", strerror(errno));
    }
}


