#include "nrm.h"
#include <signal.h>

#include <stdio.h>
#include <unistd.h>
#include <getopt.h>

char    *progname;
int dflag = 0;
int hflag = 0;
int iflag = 0;
int rflag = 0;
int sflag = 0;      /* turn on sequenced backups */
int fflag = 1;
int bflag = 0;      /* internal flag to tell if we're in the background */
int verbose = 0;      
int vflag = 0;      

int gtime = 0;      /* default gracetime offset for atimes. */
                    /* the (3 day) offset is built into     */
                    /* the nrm.cleanup script.  It deletes  */
                    /* files whose atime is older than      */
                    /* GRACETIME days   */


int main (argc, argv) /* nrm: recoverably delete files */
int argc;
char **argv;
{
    int c;
    int errflag;
    int temp, returncode;

    extern int  optind;  /* argv index of next option */
    extern int  opterr;
    extern char *optarg;

    errflag = 0;
    opterr = 0;     /* disables getopt's error msg's */

    progname = argv[0];

    while (1) {
        int option_index = 0;
        static struct option long_options[] = {
            {"directory", 0, 0, 'r'},
            {"force", 0, 0, 'f'},
            {"interactive", 0, 0, 'i'},
            {"recursive", 0, 0, 'r'},
            {"help", 0, 0, 'h'},
            {"gracetime", 1, 0, 't'},
            {"sequenced", 1, 0, 's'},
            {"version", 0, &vflag, 1},
            {"verbose", 0, 0, 'v'},
            {0, 0, 0, 0}
        };

        c = getopt_long (argc, argv, "dfhirRst:v",
        long_options, &option_index);
        if (c == -1) break;

        switch (c) {
            case 0:
                /* printf ("option %s", long_options[option_index].name); */
                /* if (optarg) printf (" with arg %s", optarg); */
                /* printf ("\n"); */
                break;

            case 'f':
                fflag = 0;
                break;
            case 'h':
                hflag++;
                break;
            case 'i':
                iflag++;
                break;
            case 'd':
            case 'R':
            case 'r':
                rflag++;
                break;
            case 's':
                sflag++;
                break;
            case 't':
                gtime = (atoi(optarg) - GRACETIME);
                break;
            case 'v':
                verbose++;
                break;
            default:
                errflag++;
                fprintf(stderr, "%s: invalid option --%c\n", 
                    progname, optopt);
                fprintf(stderr, "Try `%s --help' for more information.\n",
                    progname);
                exit(1);
                break;
        }
    }

    /* bug fix: find out if we are in the background... if so
    ** then set the background flag to prevent asking any questions...
    ** (we still print out certain errors (a la /bin/rm)    */

    if (signal(SIGINT, SIG_IGN) == SIG_IGN ) {
        bflag = 1;
    }

    if (hflag) {
        printf("Usage: %s [OPTION]... FILE...\n",progname);
        printf("Recoverably remove the FILE(s).\n");

        printf("  -d, --directory       remove directories and contents recursively\n"); 
        printf("  -f, --force           ignore nonexistent files, never prompt\n");
        printf("  -t, --gracetime <n>   number of days until permanent deletion\n");
        printf("  -i, --interactive     prompt before any removal\n");
        printf("  -r, -R, --recursive   remove the contents of directories recursively\n");
        printf("  -s, --sequenced       use time-sequenced backups\n");
        printf("  -v, --verbose         explain what is being done\n");
        printf("  -h, --help            display this help and exit\n");
        printf("      --version         output version information and exit\n");
        printf("\n");
        printf("Report bugs to walker@omnisterra.com\n");  
        exit(0);
    }

    if (vflag) {
        printf("nrm: Linux version 0.1 (walker@omnisterra.com)\n");
        exit(0);
    }

    /* rm returns usage message if no files are specified */
    // if (((errflag) || (optind >= argc)) && fflag) {

    if ((errflag && fflag) || (optind >= argc)) {
        fprintf(stderr, "%s: too few arguments\n", 
            progname);
        fprintf(stderr, "Try `%s --help' for more information.\n",
            progname);
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
