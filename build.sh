#!/bin/bash

declare TI_CARD_ID='0451:bef3'
declare CCS_PATH
declare CCS_PATH_LEGA=$HOME/ti/ccsv6
declare CCS_LOAD_UTIL=ccs_base/scripting/examples/loadti/loadti.sh
declare CCS_MAKE_UTIL=utils/bin/gmake
declare CCS_MAKE_UTIL_FLAGS='-k -j 12 all -O'
declare PROJ_DEBUG_PATH=./Debug
declare PROJ_TARGCNF=./targetConfigs/MSP432P401R.ccxml
declare PROJ_TARGCNF_LEGA=./targetConfigs/MSP432P401R_PS.ccxml
declare PROJ_TARGOUT
declare -l RESPON

## IDE VARS DECLARATION
declare ECL_IDE=eclipse/eclipse
declare ECL_IDE_FLAGS='-noSplash -data $ECL_WS
        -application com.ti.ccstudio.apps.projectBuild
	-ccs.projects $PROJ_NAME
	-ccs.configuration Debug'
declare PROJ_NAME="$( grep name .project | head -n 1 |
			 tr '<>' '|' | cut -f 3 -d'|' )"
declare ECL_WS=$HOME/workspace_v9/
declare TEMP_FILE=$( mktemp /tmp/ccs_burn.XXXX )

## AnsiC{olors}
declare -r ANSI_YELLOW='\033[1;33m'
declare -r ANSI_NOCOLOR='\033[0m'

for CCS_DIR in $HOME/ti/ccs90{0,1}/ccs /opt/ti/ccs90{0,1}/ccs
do
    test -d $CCS_DIR || continue
    CCS_PATH=$CCS_DIR
done

if [ $CCS_PATH ]
then
    ( cd $PROJ_DEBUG_PATH && $CCS_PATH/$CCS_MAKE_UTIL $CCS_MAKE_UTIL_FLAGS 2> $TEMP_FILE ) ||
	(
	    cat $TEMP_FILE
	    grep --color=always '[ ]error[ ]' $TEMP_FILE
	    while :
	    do
		sleep 0.2
		printf "$ANSI_YELLOW"
		read -n 1 -p 'Do you want to refresh Makefiles? [Y/n] ' RESPON
		printf "\n$ANSI_NOCOLOR"
		if [ ! $RESPON ] || [[ $RESPON =~ [yn] ]]; then break; fi
	    done
	    if [[ $RESPON == 'y' ]] || [ ! $RESPON ]
	    then
		rm -rf $PROJ_DEBUG_PATH ; eval $CCS_PATH/$ECL_IDE $ECL_IDE_FLAGS
	    fi
	)
    for OUT_FILE in $PROJ_DEBUG_PATH/*.out
    do
	[ $PROJ_TARGOUT ] && {
	    rm $TEMP_FILE
	    printf 'Error: Multiple .out files.\n' &>/dev/stderr
	    exit 2 ; }			   
	PROJ_TARGOUT=$OUT_FILE
    done
    >$TEMP_FILE # Erase gmake info from buffer
    if ( cd $PROJ_DEBUG_PATH && $CCS_PATH/$CCS_MAKE_UTIL $CCS_MAKE_UTIL_FLAGS &>/dev/null )
    then
	lsusb -d $TI_CARD_ID && test -f $PROJ_TARGOUT &&
	    $CCS_PATH/$CCS_LOAD_UTIL -c $PROJ_TARGCNF -l $PROJ_TARGOUT &> $TEMP_FILE
	lsusb -d $TI_CARD_ID && test -f $PROJ_TARGOUT &&
	    if grep -c 'no longer supported' $TEMP_FILE
	    then
		$CCS_PATH_LEGA/$CCS_LOAD_UTIL -c $PROJ_TARGCNF_LEGA -l $PROJ_TARGOUT &> $TEMP_FILE
	    fi
    fi
    cat $TEMP_FILE
    rm $TEMP_FILE
else
    rm $TEMP_FILE
    printf 'Error: CCS_PATH not found.\n' &>/dev/stderr
    exit 1
fi

exit 0
