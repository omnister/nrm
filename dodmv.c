#include "nrm.h"

dodmv(src,dst)      /* rename directory from src to dst name */
char *src, *dst;    /* deletes dst first !!! */
{
    if (expunge(dst)) { /* make sure dest is gone */
        errout("%s: can't delete old %s",progname,dst,"");
    }

    if (!(linkdir(src,dst))) {
        if(!expunge(src))   /* only kill src if linkdir ok! */
            return(0);
    } else {                /* linkdir failed so don't leave any */
        expunge(dst);       /* junk hanging around */
        return(1);
    }
    return(1);              /* unreachable but keeps lint happy */
}
