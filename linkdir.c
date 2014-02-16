#include <stdio.h>
#include <sys/types.h>  /* needed for stat */
#include <sys/dir.h>
#include <fcntl.h>
#include <sys/stat.h>   /* needed for stat */
#include "nrm.h"

linkdir(srcdir,dstdir)  /* recursive directory linker */
char *srcdir;           /* returns 0 if no problems  */

char *dstdir;           /* returns 1 on error   */
{                       /* assumes dstdir does not exist */

    int fd;             /* assumes that .gone directory exists */
    extern int errno;
    extern int iflag, fflag;
    struct direct nbuf;
    struct stat stbuf;
    char srcname[BUFLEN];
    char dstname[BUFLEN];
    extern char *progname;

    if (srcdir[0] == '\0')      /* current directory    */
        srcdir = ".";

    if ((fd=open(srcdir, O_RDONLY)) == -1) {
        errout("%s: %s not removed: can't open",progname,srcdir,"");
        return(1);
    }

    if((mkdir(dstdir,'\777') == -1) && errno != EEXIST) {
        errout("%s: %s not removed1: can't create %s",progname,srcdir,dstdir);
        return(1);
    }

    while (read(fd,(char *) &nbuf,sizeof(struct direct)) > 0) {
        if (nbuf.d_ino) {
            sprintf(srcname,"%s/%s",srcdir,nbuf.d_name);
            sprintf(dstname,"%s/%s",dstdir,nbuf.d_name);

            if(stat(srcname, &stbuf) == -1) {
                errout("%s: can't access %s",progname,srcname,"");
                return(1);
            }

            if ((stbuf.st_mode & S_IFMT) == S_IFDIR) {  /* it's a dir */
                if(strcmp(nbuf.d_name,"..")
                    && strcmp(nbuf.d_name,".")) {
                    /* && (strindex(nbuf.d_name,".gone") == -1))  */
                    if (linkdir(srcname,dstname))
                        return(1);
                }
            } else {                                    /* it's a file */
                if(link(srcname,dstname)) {
                    errout("%s: %s not removed: can't move %s", 
                        progname,srcdir,srcname);
                    return(1);
                }
                if(updatetime(dstname,gtime))
                    return(1);
            }
        }
    }
    if(updatetime(dstdir,gtime)) {
        return(1);
    }
    close(fd);
    return(0);
}
