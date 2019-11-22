#include "nrm.h"
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>   /* required for stat  */

char    *progname;  /* program name for error message */
int aflag = 0;
int cflag = 0;
int fflag = 1;
int mflag = 0;
int iflag = 0;  /* not used but needed to link with errout */
int needsmade(char *path1, char *path2);


int main(int argc, char **argv)    /* test to see if first file is newer than second */
{
    extern int  optind;  /* argv index of next option */
    extern int  opterr;
    int c;
    int errflag = 0;
    int temp, returncode;
    opterr = 0;     /* disables getopt's error msg's */

    progname = argv[0];
    if (argc != 3)
        errflag++;  /* print usage if no com args   */

    if (errflag) {
        fprintf(stderr, "usage: %s sourcefile targetfile\n", progname);
        fprintf(stderr, "  checks modification times of source, target.\n");
        fprintf(stderr, "  returns 0 if sourcefile is newer than target");
        fprintf(stderr, " or if target does not exist.\n");
        fprintf(stderr, "  returns 1 if target is newer than source\n");
        fprintf(stderr, "  returns 2 if source does not exist\n");
        exit(3);
    }

    return (needsmade(argv[1], argv[2]));
}


int needsmade(char *path1, char *path2)
{
	double delta;
    struct stat buf1;
    struct stat buf2;
    if (lstat(path1, &buf1) == -1) {
        return(2);
    }
    if (lstat(path2, &buf2) == -1) {
        return(0);
    }

	delta =  difftime(buf1.st_mtime, buf2.st_mtime);

	if (delta > 0.0)
		return 0;
	else
		return 1;
}




