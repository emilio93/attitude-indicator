#!/bin/bash

declare TI_CARD_ID='0451:bef3'
declare CCS_PATH
declare CCS_LOAD_UTIL=ccs/ccs_base/scripting/examples/loadti/loadti.sh
declare CCS_MAKE_UTIL=ccs/utils/bin/gmake
declare CCS_MAKE_UTIL_FLAGS='-k -j 12 all -O'
declare PROJ_DEBUG_PATH=./Debug
declare PROJ_TARGCNF=./targetConfigs/MSP432P401R.ccxml
declare PROJ_TARGOUT

## IDE VARS DECLARATION
declare ECL_IDE=ccs/eclipse/eclipse
declare ECL_IDE_FLAGS='-noSplash -data $ECL_WS
        -application com.ti.ccstudio.apps.projectBuild
	-ccs.projects $PROJ_NAME
	-ccs.configuration Debug'
declare PROJ_NAME="$( grep name .project | head -n 1 |
			 tr '<>' '|' | cut -f 3 -d'|' )"
declare ECL_WS=$HOME/workspace_v9/

for CCS_DIR in $HOME/ti/ccs90{0,1} /opt/ti/ccs90{0,1}
do
    test -d $CCS_DIR || continue
    CCS_PATH=$CCS_DIR
done

if [ $CCS_PATH ]
then
    ( cd $PROJ_DEBUG_PATH && $CCS_PATH/$CCS_MAKE_UTIL $CCS_MAKE_UTIL_FLAGS ) ||
	( rm -rf $PROJ_DEBUG_PATH ; eval $CCS_PATH/$ECL_IDE $ECL_IDE_FLAGS )
    for OUT_FILE in $PROJ_DEBUG_PATH/*.out
    do
	[ $PROJ_TARGOUT ] && {
	    printf 'Error: Multiple .out files.\n' &>/dev/stderr
	    exit 2 ; }			   
	PROJ_TARGOUT=$OUT_FILE
    done
    lsusb -d $TI_CARD_ID && test -f $PROJ_TARGOUT &&
	$CCS_PATH/$CCS_LOAD_UTIL -c $PROJ_TARGCNF -l $PROJ_TARGOUT
else
    printf 'Error: CCS_PATH not found.\n' &>/dev/stderr
    exit 1
fi

exit 0
