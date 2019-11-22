
#include "nrm.h"

/* rename file from src to dst name */
/* tries real hard to unlink pre-existing dst file */
int dofmv(char *src, char *dst) 
{

    if ((rename(src, dst)) != 0) {
        if (errno != EEXIST) {
            errout("%s can't link %s and %s", progname, src, dst);
            return(ERR);
        }
        if (expunge(dst)) {  /* make sure dest is gone */
            errout("%s: can't delete old %s", progname, dst, "");
            return(ERR);
        }
        if ((rename(src, dst)) != 0) {  /* and try again ...  */
            errout("%s can't link %s and %s", progname, src, dst);
            return(ERR);
        }
    }

    if (updatetime(dst, gtime)) {
        errout("%s can't set time on file %s", progname, dst, "");
        return(ERR);
    }
    return(0);
}
