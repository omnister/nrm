#include "nrm.h"

char    *progname;  /* program name for error message */
int fflag = 1;
int iflag = 0;      /* dummy global for isdir() */
int bflag = 0;      /* dummy global for isdir() */

static char id[] = "@(#) Bugs to Rick Walker, walker@opus.hpl.hp.com $Header: urmmain.c,v 1.1 93/05/13 17:42:24 walker Exp $";

int main(int argc, char *argv[])    /* urm: restore deleted files/dirs */
{
    extern int  optind;  /* argv index of next option */
    extern int  opterr;
    int c;
    int errflag = 0;
    int temp, returncode;
    opterr = 0;     /* disables getopt's error msg's */

    progname = argv[0];
    if (argc == 1)
        errflag++;  /* print usage if no com args   */
    while ((c = getopt(argc, argv, "f")) != EOF)
        switch (c) {
        case 'f':
            fflag = 0;
            break;
        case '?':
            errflag++;
            break;
        }
    if (errflag && fflag) {
        fprintf(stderr, "usage: %s [-f] file ...\n", progname);
        exit(2);
    } else {
        returncode = 0;
        for (; optind < argc; optind++) {
            temp = restore(argv[optind]);
            if (temp > returncode)
                returncode = temp;
        }
    }
    return(returncode);
}


