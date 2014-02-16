#include "nrm.h"

savedir(path)       /* save directory in ./.gone/dir */
char    *path;         /* deletes dst first !!! */
{
    char    dstfile[BUFLEN];
    char    dir[BUFLEN];
    char    file[BUFLEN];
    char    sdir[BUFLEN]; /* dir to save */

    strcpy(dir, path);
    basename(dir, file); /* puts dir in dir and file in file */
    /* called with full path in first arg */
    sprintf(dstfile, "%s%s/%s", dir, ".gone", file);

    if (expunge(dstfile)) { /* make sure dest is gone */
        return(ERR);
    }

    sprintf(sdir, "%s%s", dir, ".gone");
    if ((mkdir(sdir, '\777') == -1) && errno != EEXIST) {
        errout("%s: %s not removed: can't create %s", progname, path, sdir);
        return(1);
    }   /* make sure .gone exists before calling linkdir */

    if (!(linkdir(path, dstfile))) {
        if (!expunge(path)) {    /* only kill path if linkdir ok! */
            if (updatetime(dstfile, gtime)) {
                return(ERR);
            }
            return(0);
        }
    } else {                /* linkdir failed so don't leave any */
        expunge(dstfile);       /* junk hanging around */
        return(1);
    }
    return(ERR);
}


