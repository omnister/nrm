#include <stdio.h>

extern int fflag;

pick(s) /* offer choice of s */
char *s;
{
    fprintf(stderr, "%s: ? ",s);

	/* if set quiet, always return no (0) */
	/* fflag always cleared if we are in the background ... */

    if (fflag && (ttyin() == 'y')) 
        return(1);
    return(0);
}
