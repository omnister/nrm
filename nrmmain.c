#include "nrm.h"
#include <signal.h>

char    *progname;
int iflag = 0;
int rflag = 0;
int fflag = 1;
int bflag = 0;      /* internal flag to tell if we're in the background */
int gtime = 0;      /* default gracetime offset for atimes. */
/* the (3 day) offset is built into     */
/* the nrm.cleanup script.  It deletes  */
/* files whose atime is older than      */
/* GRACETIME days   */

static char id[] = "@(#) Bugs to Rick Walker, hplabs!walker $Header: /va/walker/bin/src/nrm/sharfile,v 1.10 88/03/03 12:12:12 walker Exp $";

main(argc, argv)    /* nrm: recoverably delete files */
int argc;
char    *argv[];
{
    extern int  optind;  /* argv index of next option */
    extern int  opterr;
    extern char *optarg;
    int c;
    int errflag;
    int temp, returncode;

    errflag = 0;
    opterr = 0;     /* disables getopt's error msg's */

    progname = argv[0];

    while ((c = getopt(argc, argv, "ifrt:")) != EOF)
        switch (c) {
        case 'i':
            iflag++;
            break;
        case 'f':
            fflag = 0;
            break;
        case 'r':
            rflag++;
            break;
        case 't':
            gtime = (atoi(optarg) - GRACETIME);
            break;
        case '?':
            errflag++;
            break;
        }
    /* bug fix: find out if we are in the background... if so
    ** then set the background flag to prevent asking any questions...
    ** (we still print out certain errors (a la /bin/rm)    */

    if (signal(SIGINT, SIG_IGN) == SIG_IGN ) {
        bflag = 1;
    }

    if ((errflag && fflag) || (argc <= 1)) {
        fprintf(stderr, "usage: %s [-fir][-t time] file ...\n", progname);
        exit(2);
    } else {
        returncode = 0;
        for (; optind < argc; optind++) {
            temp = do_nrm(argv[optind]);
            if (temp > returncode)
                returncode = temp;
        }
    }
    return(returncode);
}


