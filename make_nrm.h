#!/bin/sh

#this script resolves the fact that nrm, and urm are dependant on
#the default GRACETIME value set in nrm.cleanup.
#The nrm.h header is edited by this script to match the nrm.cleanup
#definition.

newtime=`grep "^GRACETIME=" nrm.cleanup | tr -cd "[0-9]" ""`

	echo "\tsetting GRACETIME = $newtime in nrm.h (as defined in nrm.cleanup)"

	sed "/GRACETIME.*/s//GRACETIME $newtime/" nrm.h >nrm.h.tmp
	/bin/mv nrm.h.tmp nrm.h
