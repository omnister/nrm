#include <stdio.h>

extern int  bflag;   /* => 1, if we are in the background, 0 otherwise */

pick(s) /* offer choice of s */
char    *s;
{
    char c;

    if (s[0]) {         /* format tweaking! (just like /bin/rm) */
        fprintf(stderr, "%s: ? ", s);
    } else {
        fprintf(stderr, " ? ");
    }

    if (bflag) {        /* if in the background, return 0 */
        fprintf(stderr, "n\n");
        return(0);      /* don't call ttyin if in background!! */
    } else {
	c = ttyin();
	return (c == 'y' || c == 'Y');
    }
}


