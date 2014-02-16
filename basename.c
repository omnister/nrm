#include <string.h>
#include "nrm.h"

/* a  little driver for testing ... */
/*
main()
{
    char buf[BUFLEN];
    char buf2[BUFLEN];

    while(scanf("%s",buf) != EOF) {
        basename(buf,buf2);
        printf("dir = \"%s\", file = \"%s\"\n",buf,buf2);
    }
}
*/


void basename(dir, file)     /* called with full dir name in "dir"           */
char *dir, *file;       /* strips trailing '/''s                  */
{                       /* if called w/("/usr/local/bin/",s2) returns:   */
    /* with (s1=>/usr/local/,s2=>bin)                */
    char s1[BUFLEN];
    char s2[BUFLEN];
    char *p1,*p;
    int junk;   /* used to keep track of space left in strtok buffer */

    strcpy(s1,dir);
    p1 = s1;    /* p1 is pointer to private copy of dir string */

    while ((p = strtok(p1,"/")) != NULL) {
        p1 = NULL;  /* makes strtok work ... look it up! */
        /* next line makes things like: "a/b/" work  */
        /* without it the final "/" goofs things up  */
        junk = strlen(dir)-(strlen(p)+(p-s1));
        strcpy(s2,p);
    } /* s2 now points to stripped file name */
    dir[(strlen(dir)-strlen(s2))-junk] = '\0'; /* just path alone */
    strcpy(file,s2);
}
