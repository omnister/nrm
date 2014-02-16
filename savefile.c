#include "nrm.h"

savefile(path)      /* mv file from path to .gone dir */
char    *path;         /* tries real hard to unlink pre-existing dst file */
{
    char    dstfile[BUFLEN];
    char    dstdir[BUFLEN];
    char    dir[BUFLEN];
    char    file[BUFLEN];

    strcpy(dir, path);
    basename(dir, file); /* puts dir in dir and file in file */
    /* called with full path in first arg */

    sprintf(dstdir, "%s%s", dir, ".gone");
    sprintf(dstfile, "%s/%s", dstdir, file);

    if (link(path, dstfile)) { /* problems... */
        if (errno == EEXIST) {  /* file already exists */
            if (expunge(dstfile)) {  /* make sure dest is gone */
                return(ERR);
            }
        } else if (errno == ENOENT) { /* directory doesn't exist */
            if ((mkdir(dstdir, '\777') == -1) && errno != EEXIST) {
                errout("%s: %s not removed: can't create %s",
                    progname, path, dstdir);
                return(ERR);
            }
        } else {
            errout("%s: %s not removed: can't link with %s",
                progname, path, dstfile);
            return(ERR);
        }

        if (link(path, dstfile)) {   /* so try again ...    */
            errout("%s: %s not removed: can't link with %s",
                progname, path, dstfile);
            return(ERR);
        }
    }

    if (updatetime(dstfile, gtime)) {
        expunge(dstfile); /* don't leave sitting around */
        return(ERR);
    }

    if (unlink(path)) {
        errout("%s: %s not removed: can't unlink", progname, path, "");
        expunge(dstfile); /* don't leave sitting around */
        return(ERR);
    }

    return(0);
}


