#include "nrm.h"
#include <sys/types.h> /* for stat */
#include <sys/stat.h>  /* for stat */

do_nrm(fname)
char    *fname;
{
    extern char *progname;
    extern int  errno;
    extern int  iflag, fflag, rflag, bflag;
    struct stat stbuf;
    int dirflag, symflag;

    errno = 0;      /* clear errno so no bogus msg's */

    if (lstat(fname, &stbuf) == -1) {
        errout("%s: can't access %s", progname, fname, "");
        return(2 * fflag);
    }
    dirflag =  ((stbuf.st_mode & S_IFMT) == S_IFDIR);
    symflag =  ((stbuf.st_mode & S_IFMT) == S_IFLNK);

    if ((strindex(fname, ".gone") != -1) || 
        !strcmp(fname, "..") || 
        !strcmp(fname, ".")) { /* prevents nasty recursion ! */
        fprintf(stderr, "%s: cannot remove %s\n", progname, fname);
        /* return(0); */
        return(2);			/* treat as an error */
    }
    if (dirflag && !rflag) {
        fprintf(stderr, "%s: %s directory, try -r option\n", progname, fname);
        return(2);
    }

    if (iflag) {
        if (dirflag)
            fprintf(stderr, "directory ");
        if (!pick(fname))
            return(0);
    } else if (fflag ) {     /* verbose mode */
        if (access(fname, 02) < 0 && !symflag ) {
            if (!bflag)  {   /* not in background */
                fprintf(stderr, "%s: %o mode", fname, stbuf.st_mode & 0777);
                if (!pick(""))
                    return(0);
            } else {        /* in background */
                fprintf(stderr, "%s: removing \'%s\' with %o mode\n",
                    progname, fname, stbuf.st_mode & 0777);
            }
        }
    }

    if (savefile(fname)) {
        return(2 * fflag);
    }
    return(0);
}

