HSOURCES=basename.c linkdir.c expunge.c  errout.c\
	do_nrm.c nrmmain.c urmmain.c \
	restore.c updatetime.c gtime.c printtimes.c
NSOURCES=efopen.c strindex.c ttyin.c  isdir.c pick.c 
OBJECTS=efopen.o ttyin.o errout.o pick.o strindex.o\
	basename.o expunge.o linkdir.o updatetime.o 
ALL=$(HSOURCES) $(NSOURCES) nrm.h\
	dofmv.c dodmv.c  savedir.c savefile.c\
	nrm.1 nrm.cleanup make_nrm.h makefile
NLINTSRC= efopen.c ttyin.c errout.c pick.c strindex.c\
	basename.c expunge.c linkdir.c updatetime.c\
	do_nrm.c nrmmain.c savedir.c savefile.c
ULINTSRC= efopen.c ttyin.c errout.c pick.c strindex.c\
	basename.c expunge.c linkdir.c updatetime.c\
	urmmain.c isdir.c dodmv.c dofmv.c restore.c
SYSBIN=/usr/local/bin/
MANDIR=/usr/local/man/man1/
CATDIR=/usr/local/man/cat1/
CLEANUP=/etc
SRC=/usr/local/src/cmd/nrm/nrm/

# if your system has the berkely rename(2) function available,
# use the RENAME define for CCFLAGS below:

#CCFLAGS=-O
CCFLAGS=-O -DRENAME

all: nrm urm gtime

nrm: $(OBJECTS) do_nrm.o nrmmain.o savedir.o savefile.o
	cc $(OBJECTS) do_nrm.o nrmmain.o savedir.o savefile.o -o nrm $(CCFLAGS) 
	strip nrm			

urm: $(OBJECTS) urmmain.o isdir.o dodmv.o dofmv.o restore.o
	cc $(OBJECTS) urmmain.o isdir.o dodmv.o dofmv.o restore.o -o urm $(CCFLAGS)
	strip urm

gtime: gtime.o printtimes.o errout.o
	cc gtime.o printtimes.o errout.o $(CCFLAGS) -o gtime

install: nrm urm nrm.1 nrm.cleanup sharfile
	-cp * $(SRC)
	-cp nrm $(SYSBIN)
	-cp urm $(SYSBIN)
	-cp gtime $(SYSBIN)
	-cp nrm.1 $(MANDIR)
	/bin/rm -f $(CATDIR)nrm.1
	-cp nrm.cleanup $(CLEANUP)

shar: $(ALL) 
	shar -bcv $(ALL) > sharfile 

lint: $(NLINTSRC) $(ULINTSRC)
	echo "******** lint output for nrm files *********" > fluff
	lint $(NLINTSRC) >> fluff
	echo "******** lint output for urm files *********" >> fluff
	lint $(ULINTSRC) >> fluff

$(HSOURCES):nrm.h

.c.o:
	cc -c $(CCFLAGS) $<

##### other stuff not included in shar:   
ncalls: 
	cat $(NLINTSRC) | calls -
ucalls: 
	cat $(ULINTSRC) | calls -
rmdir: killdir.o killdirmain.o isdir.o errout.o
	cc killdir.o killdirmain.o isdir.o errout.o $(CCFLAGS) -o rmdir
rlist: rlist.o rmain.o isdir.o
	cc rlist.o rmain.o isdir.o $(CCFLAGS) -o rlist
mvdir: ttyin.o efopen.o linkdir.o pick.o expunge.o mvdir.o updatetime.o mvdirmain.o isdir.o errout.o
	cc ttyin.o linkdir.o efopen.o mvdir.o pick.o expunge.o mvdirmain.o updatetime.o isdir.o errout.o $(CCFLAGS) -o mvdir
