#include <stdio.h>
#include <errno.h>
#include <string.h>

#define ERR -1
#define BUFLEN 	512
#define GRACETIME	3	/* default gracetime for deleted files */
						/* if you change this, then also change */
						/* nrm.1  &  nrm.cleanup*/
#define SECS_PER_DAY 86400
extern int gtime;
extern char *progname;

void exit();
void errout();
void basename();
