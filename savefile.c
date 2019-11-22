#include "nrm.h"
#include <sys/types.h>

#define FEXISTS 0

/*************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <time.h>

#define SEPCHAR '%'
#define FEXISTS 0

#define LONGNAME    "ThisFilenameIsLong"
#define LONGERNAME  "ThisFilenameIsLonger"
/*************************************/



char *ntostr();
char *findhash();

/* mv file from path to .gone dir */
/* tries real hard to unlink pre-existing dst file */

int savefile(char *path)	    
{
    char    dstfile[BUFLEN*3];
    char    dstdir[BUFLEN*2];
    char    tmp[BUFLEN*4];
    char    dir[BUFLEN];
    char    file[BUFLEN];
    char    timestring[BUFLEN];
    time_t  now;


    strcpy(dir, path);
    basenm(dir, file); /* puts dir in dir and file in file */
    sprintf(dstdir, "%s%s", dir, ".gone");

    if (!sflag) {
	sprintf(dstfile, "%s/%s", dstdir, file);
    } else {
	time(&now);
	if (islong(dir)) {	    /* long filename system */
	    strftime(timestring,BUFLEN,"-%y-%m-%d-%H:%M:%S",localtime(&now));
	    sprintf(tmp,"%s/%s%s",dstdir,file,timestring);

	    /* avoid using hash if possible */
	    if (access(tmp, FEXISTS) == 0) {    /* collision */
	    	strcpy(dstfile,findhash(tmp)); 
	    } else {				/* no collision */
	    	strcpy(dstfile,tmp);	
	    }

	} else {	    	    /* short filename system */
	    sprintf(tmp,"%s/%-.11s",dstdir,file);
	    strcpy(dstfile,findhash(tmp));
	}
    } 

    /*
    ** the following 5 lines are needed only because rename(src,dst)
    ** returns *no* error on HPUX, and does not remove the src file 
    ** when the src and dst files happen to be linked.  Because of this,
    ** we have to pre-remove the dst file.  If/when rename() is fixed, 
    ** these lines can be removed...  <RCW 7/20/94>
    */
    if (expunge(dstfile)) { /* make sure dest is gone */
	errout("%s: %s not removed: can't link with %s",
	    progname, path, dstfile);
	return(ERR);
    }

    if (rename(path, dstfile)) {    /* problems... */
	if (errno == ENOENT)	 { /* directory doesn't exist */
	    /* printf("errno = %d\n",errno); */
	    if ((mkdir(dstdir, 0777) == -1) && errno != EEXIST) {
		errout("%s: %s not removed: can't create %s",
		    progname, path, dstdir);
		return(ERR);
	    }
	    if (chmod(dstdir, 0777)) {
		errout("%s: error in setting mode of .gone:",
		    progname, "", "");
		/* return(ERR); */ /* not fatal */
	    }
	} else if (expunge(dstfile)) { /* make sure dest is gone */
	    errout("%s: %s not removed: can't link with %s",
		progname, path, dstfile);
	    return(ERR);
	} 

	if (rename(path, dstfile)) {    /* so try again ...	   */
	    errout("%s: %s not removed: can't link with %s",
		progname, path, dstfile);
	    return(ERR);
	}
    } 
    
    if (fflag && verbose) {	/* no problems, but verbose */
	 printf("%s\n",path);
    }   

    if (updatetime(dstfile, gtime)) {
	/* expunge(dstfile);  */
	return(ERR);
    }

    return(0);
}


/* Called with "path". Finds a suffix "x" such that "path"%"x"
** does not exist, where "x" is a random two character base-62 
** string generated by ntostr().
** 
** routine will try up to 15 times to find an unused destination
** path, but will set "x" to "" if no hash is found.
**
** returns pointer to new path on completion.
*/

char *findhash(path)
char *path;
{
    static char dest[BUFLEN];
    static int initialized = 0;
    int i,n,pid;

    if (!initialized) {
	pid=getpid();
    	srand((unsigned) time((time_t *) 0) + (unsigned) pid);
	initialized++;
    }

    for (i=0;i<15;i++) {    /* get 15 tries at hashing */
	n = (rand()+pid)%3840;
    	sprintf(dest, "%s%s", path,ntostr(n));

	if (access(dest, FEXISTS)) {     /* doesn't exist */
	    return(dest);
	} 
    }
    return(path);
}

/* macro to convert number (n) into base 62 character representation */
#define C(n)  (((n)<=9)?((n)+'0'):(((n)>9&&n<=35)?((n)+'A'-10):((n)+'a'-36)))

char *ntostr(n)
int n;
{
    static char str[BUFLEN];

    if (n < 0) {
        printf("ntostr() called with negative number!\n");
        exit(1);
    } else if (n<62) {
        str[0] = SEPCHAR;
        str[1] = (char) (C(n));
        str[2] = (char) ('\0');
    } else {
        str[0] = SEPCHAR;
        str[1] = (char) (C(n/62));
        str[2] = (char) (C(n%62));
        str[3] = (char) ('\0');
    }
    return str;
}

/* islong(path)
**    Determine whether "path" is on a LONG or SHORT file system.
**    Return 1 for LONG, 0 for SHORT filesystem (or on error).
**
**    Caches info so it will not repeat test when called twice
**    in succession with an identical path string.
*/

int islong(char *path) 
{

    static char savedpath[BUFLEN] = "";
    char temppath[BUFLEN*2];
    static int  savedstatus = 0;
    int i;
    int fd;

    if (strcmp(savedpath, path) != 0) {
	strcpy(savedpath,path);

	sprintf(temppath,"%s/%s",savedpath,LONGNAME);

	if((fd = open (temppath, O_RDWR|O_CREAT,0777)) == -1) {
	    savedstatus = 0;	    /* error -> SHORT */
	} else {

	    sprintf(temppath,"%s/%s",savedpath,LONGERNAME);

	    if (access(temppath, FEXISTS) == 0) {
		savedstatus = 0;    /* truncated so SHORT */
	    } else {
		savedstatus = 1;    /* not truncated so LONG */
	    }

	    close(fd);		    /* cleanup */

	    sprintf(temppath,"%s/%s",savedpath,LONGNAME);
	    if (unlink(temppath)) {
		errout("%s: unlink error in islong()\n",progname);
	    }
	}
    }
    return(savedstatus);
}
