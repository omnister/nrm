#include <stdio.h>

extern int bflag;	/* => 1, if we are in the background, 0 otherwise */
extern int fflag;	/* => 0, if we are set quiet */

pick(s) /* offer choice of s */
char *s;
{
    fprintf(stderr, "%s: ? ",s);

    if (bflag) 			/* if in the background, return 0 */
		return(0);		/* don't call ttyin if in background!! */

    if (fflag  && (ttyin() == 'y')) 
        return(1);
    return(0);			/* if set quiet, return 0 */
}
