#!/bin/bash

declare CCS_PATH
declare CCS_LOAD_UTIL=ccs/ccs_base/scripting/examples/loadti/loadti.sh
declare CCS_MAKE_UTIL=ccs/utils/bin/gmake
declare CCS_MAKE_UTIL_FLAGS='-k -j 12 all -O'
declare PROJ_DEBUG_PATH=./Debug
declare PROJ_TARGCNF=./targetConfigs/MSP432P401R.ccxml
declare PROJ_TARGOUT

for CCS_DIR in $HOME/ti/ccs90{0,1} /opt/ti/ccs90{0,1}
do
    test -d $CCS_DIR || continue
    CCS_PATH=$CCS_DIR
done

if [ $CCS_PATH ]
then
    ( cd $PROJ_DEBUG_PATH && $CCS_PATH/$CCS_MAKE_UTIL $CCS_MAKE_UTIL_FLAGS )
    for OUT_FILE in $PROJ_DEBUG_PATH/*.out
    do
	[ $PROJ_TARGOUT ] && {
	    printf 'Error: Multiple .out files.\n' &>/dev/stderr
	    exit 2 ; }			   
	PROJ_TARGOUT=$OUT_FILE
    done
    $CCS_PATH/$CCS_LOAD_UTIL -c $PROJ_TARGCNF -l $PROJ_TARGOUT
else
    printf 'Error: CCS_PATH not found.\n' &>/dev/stderr
    exit 1
fi

exit 0
