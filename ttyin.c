#include <stdio.h>
void exit();
extern int  fflag;

int ttyin(void) /* process response from /dev/tty (version 1) */
{
    char    buf[BUFSIZ];
    FILE * efopen();
    static FILE *tty = NULL;

    if (tty == NULL)
        tty = efopen("/dev/tty", "r");
    if (fgets(buf, BUFSIZ, tty) == NULL || buf[0] == 'q' || buf[0] == 'Q')
        exit(0);
    return(buf[0]); /* else ordinary line */
}


