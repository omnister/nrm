#include "nrm.h"
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>

expunge(path)          /* recursive file/directory deleter */
char    *path;         /* returns 0 if ok, -1 if can't open, */
{                      /* 1,2,3... for # of unremovable subdirs */
                       /* roughly equ. to /bin/rm -rf  */
    int errflag;
    struct dirent *dp;
    DIR * dirp;
    char    name[BUFLEN];
    extern int  errno;
    extern char *progname;
    errflag = 0;

    if (path[0] == '\0')    {       /* current directory    */
        errflag = 0;
        errout("%s: expunge called with null argument", progname, "", "");
        return(errflag++);
    }

    if (unlink(path) == 0) {         /* assume reg. file so use unlink call */
        return(0);
    } else {                        /* some problem */
        if (errno == ENOENT) {       /* file didn't exist */
            return(0);              /* no problem, file didn't exist */
        } else if (errno == EPERM)  {
	    ;	/* fall through */
	} else if (errno == EISDIR) {
	    ;	/* fall through */
	} else if (errno == ENOTDIR) {
	    /* rather than try to fix the problem, just notify the */
	    /* user that something is wrong here... */
            errout("%s: %s error. Check that '.gone' is really a directory",
		progname, path, "");
            return(errflag++);
	} else {  /* not a directory */
	    printf("errno = %d\n",errno);
            errout("%s: %s not removed: can't unlink ", progname, path, "");
            return(errflag++);
        }
    }           /* falls through here only if path is a directory */

    if ((dirp = opendir(path)) == (DIR * ) - 1) {
        errout("%s: %s not removed: can't read", progname, path, "");
        return(errflag++);
    }
    for (dp = readdir(dirp); dp != NULL; dp = readdir(dirp)) {
        if (dp->d_ino) {
            sprintf(name, "%s/%s", path, dp->d_name);
            if (strcmp(dp->d_name, "..") && strcmp(dp->d_name, ".")) {
                errflag += expunge(name);
            }
        }
    }
    close(dirp);
    if (rmdir(path) == -1) {
        errout("%s: %s not removed (1): can't unlink", progname, path, "");
        errflag++;
    } 
    return(errflag);
}


