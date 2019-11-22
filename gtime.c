#include "nrm.h"

char    *progname;  /* program name for error message */
int aflag = 0;
int cflag = 0;
int fflag = 1;
int mflag = 0;
int iflag = 0;  /* not used but needed to link with errout */


int main(int argc, char *argv[])    /* print file modification times */
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
    while ((c = getopt(argc, argv, "acm")) != EOF)
        switch (c) {
        case 'a':
            aflag++;
            break;
        case 'c':
            cflag++;
            break;
        case 'f':
            fflag = 0;
            break;
        case 'm':
            mflag++;
            break;
        case '?':
            errflag++;
            break;
        }
    if (errflag) {
        fprintf(stderr, "usage: %s [-acm] file ...\n", progname);
        exit(2);
    }

    if (!aflag && !cflag && !mflag) { /* proper defaults for flags */
        aflag++;
        cflag++;
        mflag++;
    }

    returncode = 0;
    for (; optind < argc; optind++) {
        temp = printtimes(argv[optind]);
        if (temp > returncode)
            returncode = temp;
    }
    return(returncode);
}


