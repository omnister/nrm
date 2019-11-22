#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define ERR -1
#define BUFLEN	512
#define WARNSIZE    10000000	/* filesize that will trigger warning */
                                /* about using nrm */

#define GRACETIME   3	/* default gracetime for deleted files */
						/* if you change this, then also change */
						/* nrm.1  &  nrm.cleanup */

#define SEPCHAR '%'	/* separation character for sequenced backups */

#define SECS_PER_DAY 86400
extern int iflag, fflag, rflag, bflag, sflag;
extern int gtime, verbose;
extern char *progname;


void exit();
void errout();
void basenm();
extern int printtimes(char *path);
extern int dofmv(char *src, char *dst);
extern int pick(char *s);
extern int restore(char *path);
extern int expunge(char *path);
extern int isdir(char *fname);
extern int islong(char *path);
extern int savefile(char *path);
extern int ttyin(void);
extern int do_nrm(char *fname);
extern int strindex(char *s, char *t);
extern int updatetime(char *path, int offset);

