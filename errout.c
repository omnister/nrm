#include "nrm.h"

void errout(s1,s2,s3,s4) /* print errors conditionally on fflag & iflag*/
char *s1,*s2,*s3,*s4;
{
    extern int iflag;
    extern int fflag;
    extern int errno;
    extern int sys_nerr;
    extern char *sys_errlist[];

    if(fflag || iflag) {    /* verbose or interactive */
        fprintf(stderr,s1,s2,s3,s4);
        if (errno > 0 && errno < sys_nerr)
            fprintf(stderr, ": %s", sys_errlist[errno]);
        fprintf(stderr,"\n");
    }
}
