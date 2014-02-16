#include "nrm.h"
#include <sys/types.h>
#include <sys/dir.h>
#include <fcntl.h>
#include <errno.h>

expunge(path)   /* recursive file/directory deleter */
char *path;     /* returns 0 if ok, -1 if can't open, */
{                   /* 1,2,3... for # of unremovable subdirs */
    int fd;         /* roughly equ. to /bin/rm -rf  */
    int errflag;
    struct direct nbuf;
    char name[BUFLEN];
    extern int errno;
    extern char *progname;
    errflag = 0;

    if (path[0] == '\0')    {       /* current directory    */
        errflag = 0;
        errout("%s: expunge called with null argument",progname,"","");
        return(errflag++);
    }

    if(unlink(path) == 0) {     /* assume reg. file so use unlink call */
        return(0);
    } else {                    /* some problem */
        if (errno == ENOENT) /* file didn't exist */
            return(0);  /* no problem, file didn't exist */
        else if (errno != EPERM) { /* not a directory */
            errout("%s: %s not removed: can't unlink ",progname,path,"");
            return(errflag++);
        }
    }           /* falls through here only if path is a directory */

    if ((fd=open(path, O_RDONLY)) == -1) {
        errout("%s: %s not removed: can't read",progname,path,"");
        return(errflag++);
    }
    while (read(fd,(char *) &nbuf,sizeof(struct direct)) > 0) {
        if (nbuf.d_ino) {
            sprintf(name,"%s/%s",path,nbuf.d_name);
            if(strcmp(nbuf.d_name,"..") && strcmp(nbuf.d_name,".")) {
                errflag += expunge(name);
            }
        }
    }
    close(fd);
    if(rmdir(path) == -1) {
        errout("%s: %s not removed: can't unlink",progname,path,"");
        errflag++;
    }
    return(errflag);
}
