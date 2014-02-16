#include "nrm.h"
#include <sys/types.h>  /* required for utime */
#include <unistd.h>     /* required for utime */
#include <sys/stat.h>   /* required for stat  */
#include <fcntl.h>      /* required for open */

long    time();

updatetime(path, offset)
char    *path;
int offset; /* offset (in days) */
{
    int fd;
    char    junk[2];   /* used by read call */
    struct stat buf;
    struct utimbuf times;
    if (stat(path, &buf) == -1) {
        errout("%s: can't stat %s", progname, path, "");
        return(1);
    }
    times.actime =  time(0) + ((time_t) (offset * SECS_PER_DAY));
    times.modtime = buf.st_mtime;
    if ((utime(path, &times)) == -1) {
        if ((offset == 0) && ((errno == EPERM) || (errno == EACCES)))
            if ((fd = open(path, O_RDONLY)) != -1) { /* last try */
                if (read(fd, junk, 1) != ERR) { /* read will modify atime */
                    close(fd);  /* don't want to run out of fd's!!! */
                    return(0);
                }
            }
        errout("warning: %s: can't set time on file %s", progname, path, "");
        return(0); /* warning */
    }
    return(0);
}


