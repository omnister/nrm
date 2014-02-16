#include "nrm.h"
#define FEXISTS 0

int gtime = 0;  /* amount of offset (days) used when restoring times */
restore(path)   /* restore file/dir @ path: return zero if ok */
char    *path;
{
    extern char *progname;
    extern int  errno;
    extern int  fflag;
    char    filesrc[BUFLEN];
    char    dir[BUFLEN];
    char    file[BUFLEN];
    int index;
    int dirflag;            /* used to keep return codes from isdir() */
    errno = 0;              /* clear errno so no bogus msg's */

    /* the next block of statements allows the user to do  */
    /* things like "urm .gone/foo*" and have the shell do  */
    /* proper wildcard expansion... the initial ".gone" is */
    /* stripped.                                           */

    if ( (index = strindex(path, ".gone/")) != -1 ) {
        strcpy(path + index, path + index + 6);
        /* may be system dependant !!*/
    }

    if ((strindex(path, ".gone") != -1) || 
        !strcmp(path, "..") || 
        !strcmp(path, ".")) {
        errout("%s: can't restore %s", progname, path, "");
        return(0);
    }

    strcpy(dir, path);
    basename(dir, file);    /* puts dir in dir and file in file */
    /* called with full path in first arg */

    sprintf(filesrc, "%s%s/%s", dir, ".gone", file);
    if ((dirflag = isdir(filesrc)) == -1) {
        errno = 0;  /* make sure errout doesn't print system err msg */

        errout("deleted file: %s does not exist", path, "", "");
        return(2 * fflag);
    }

    if (!access(path, FEXISTS) && fflag) {
        fprintf(stderr, "overwrite existing file: ");
        if (!pick(path))
            return(0);
    }

    if (dofmv(filesrc, path)) {
    	if (dirflag) {   /* a directory */
            errout("%s: can't rename directory %s", progname, path, "");
    	} else {        /* must be a file */
            errout("%s: can't rename file %s", progname, path, "");
        }
        return(2 * fflag);
    }
    return(0);
}


