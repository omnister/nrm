#include <stdio.h>
void exit();
void errout();

FILE *efopen(file, mode)    /* fopen file, die if can't */
char *file, *mode;
{
    FILE *fp, *fopen();
    extern char *progname;

    if ((fp = fopen(file, mode)) != NULL)
        return(fp);
    errout("%s: can't open file %s mode %s",
        progname, file, mode);
    exit(1);
    return(0);    /* unreachable statement to pacify lint */
}
