#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

isdir(fname) /* file a directory? 1 if true, -1 if error, 0 otherwise */
char *fname;
{
    struct stat stbuf;

    if(stat(fname, &stbuf) == -1) {
        return(-1);
    }
    if((stbuf.st_mode & S_IFMT) == S_IFDIR) {
        return(1);
    } else 
        return(0);
}
