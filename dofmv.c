
#include "nrm.h"

dofmv(src, dst)      /* rename file from src to dst name */
char    *src, *dst;    /* tries real hard to unlink pre-existing dst file */
{

#ifdef RENAME
    if ((rename(src, dst)) != 0) {
#else
        if ((link(src, dst)) != 0) {
#endif RENAME

            if (errno != EEXIST) {
                errout("%s can't link %s and %s", progname, src, dst);
                return(ERR);
            }

            if (expunge(dst)) {  /* make sure dest is gone */
                errout("%s: can't delete old %s", progname, dst, "");
                return(ERR);
            }

#ifdef RENAME
            if ((rename(src, dst)) != 0) {  /* and try again ...  */
#else
                if ((link(src, dst)) != 0) {
#endif RENAME

                    errout("%s can't link %s and %s", progname, src, dst);
                    return(ERR);
                }

            }

            if (updatetime(dst, gtime)) {
                errout("%s can't set time on file %s", progname, dst, "");
                return(ERR);
            }

#ifdef RENAME
            return(0);
#else
            return(unlink(src));
#endif RENAME
        }
