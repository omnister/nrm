#include <stdio.h>
#include <sys/types.h>  /* needed for stat */
#include <fcntl.h>
#include <sys/stat.h>   /* needed for stat */
#include <ndir.h>       /* needed to read directories across nfs */
#include "nrm.h"

linkdir(srcdir, dstdir)  /* recursive directory linker */
char    *srcdir;           /* returns 0 if no problems  */
char    *dstdir;           /* returns 1 on error   */
{                       /* assumes dstdir does not exist */

    extern int  errno;
    extern int  iflag, fflag;
    struct direct *dp;
    DIR * dirp;
    struct stat stbuf;
    char    srcname[BUFLEN];
    char    dstname[BUFLEN];
    extern char *progname;


    if (srcdir[0] == '\0')      /* current directory    */
        srcdir = ".";

    if ((dirp = opendir(srcdir)) == (DIR * ) - 1) {
        errout("%s: %s not removed: can't open", progname, srcdir, "");
        return(1);
    }

    if ((mkdir(dstdir, '\777') == -1) && errno != EEXIST) {
        errout("%s: %s not removed1: can't create %s", progname, srcdir, dstdir);
        return(1);
    }

    for (dp = readdir(dirp); dp != NULL; dp = readdir(dirp)) {
        if ((int) dp->d_fileno) {
            sprintf(srcname, "%s/%s", srcdir, dp->d_name);
            sprintf(dstname, "%s/%s", dstdir, dp->d_name);
            if (stat(srcname, &stbuf) == -1) {
                errout("%s: can't access %s", progname, srcname, "");
                return(1);
            }

            if ((stbuf.st_mode & S_IFMT) == S_IFDIR) {  /* it's a dir */
                if (strcmp(dp->d_name, "..")
                     && strcmp(dp->d_name, ".")) {
                    /* && (strindex(dp->d_name,".gone") == -1))  */
                    if (linkdir(srcname, dstname)) {
                        return(1);
                    }
                }
            } else {                                    /* it's a file */
                if (link(srcname, dstname)) {
                    errout("%s: %s not removed: can't move %s", 
                        progname, srcdir, srcname);
                    return(1);
                }
                if (updatetime(dstname, gtime)) {
                    return(1);
                }
            }
        }
    }
    if (updatetime(dstdir, gtime)) {
        return(1);
    }
    closedir(dirp);
    return(0);
}


