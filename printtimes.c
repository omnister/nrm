#include "nrm.h"
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>   /* required for stat  */

printtimes(path)
char    *path;
{
    extern int  aflag, cflag, mflag;
    struct stat buf;
    if (stat(path, &buf) == -1) {
        errout("%s: stat error in printtime()", progname, "", "");
        return(1);
    }
    printf("%s :\n", path);
    if (aflag)
        printf("\t    (read) access = %s", ctime(&buf.st_atime));
    if (cflag)
        printf("\t   (inode) change = %s", ctime(&buf.st_ctime));
    if (mflag)
        printf("\t(contents) modify = %s", ctime(&buf.st_mtime));
    return (0);
}


