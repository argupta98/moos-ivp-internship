#!/bin/bash 
#-------------------------------------------------------
#  Part 1: Check for and handle command-line arguments
#-------------------------------------------------------
SHORE_IP="192.168.1"
SHORE_LISTEN="9300"
M200_IP="192.168.10.1" #jing front seat
JUST_MAKE="no"
COOL_FAC=50
COOL_STEPS=1000
CONCURRENT="true"
ADAPTIVE="false"
SURVEY_X=70
SURVEY_Y=-100
HEIGHT1=150
HEIGHT2=150
WIDTH1=120
WIDTH2=120
LANE_WIDTH1=25
LANE_WIDTH2=25
DEGREES1=270
DEGREES2=0

for ARGI; do
    #help:
    if [ "${ARGI}" = "--help" -o "${ARGI}" = "-h" ] ; then 
        HELP="yes"
        UNDEFINED_ARG=""	
    #time warmp
    elif [ "${ARGI//[^0-9]/}" = "$ARGI" -a "$TIME_WARP" = 1 ]; then 
        TIME_WARP=$ARGI
    elif [ "${ARGI:0:6}" = "--warp" ] ; then
        WARP="${ARGI#--warp=*}"
        UNDEFINED_ARG=""
    elif [ "${ARGI}" = "--just_build" -o "${ARGI}" = "-j" ] ; then
	JUST_MAKE="yes"
    elif [ "${ARGI:0:6}" = "--cool" ] ; then
        COOL_FAC="${ARGI#--cool=*}"
        UNDEFINED_ARG=""
    elif [ "${ARGI:0:8}" = "--angle=" ] ; then
	echo "GOT ANGLE"
        DEGREES1="${ARGI#--angle=*}"
        UNDEFINED_ARG=""
    elif [ "${ARGI:0:8}" = "--angle1" ] ; then
        echo "GOT ANGLE1"
	DEGREES1="${ARGI#--angle1=*}"
        UNDEFINED_ARG=""
    elif [ "${ARGI:0:8}" = "--angle2" ] ; then
        echo "GOT ANGLE2"
	DEGREES2="${ARGI#--angle2=*}"
        UNDEFINED_ARG="" 
    elif [ "${ARGI:0:5}" = "--key" ] ; then
        KEY="${ARGI#--key=*}"
        UNDEFINED_ARG=""

    elif [ "${ARGI}" = "--adaptive" -o "${ARGI}" = "-a" ] ; then
        ADAPTIVE="true"
        UNDEFINED_ARG=""
    elif [ "${ARGI}" = "--unconcurrent" -o "${ARGI}" = "-uc" ] ; then
        CONCURRENT="false"
        UNDEFINED_ARG=""
    else 
	printf "Bad Argument: %s \n" $ARGI
	exit 0
    fi
done
if [ "${HELP}" = "yes" ]; then
    printf "%s [SWITCHES]            \n" $0
    printf "Switches:                \n"
    printf "  --warp=WARP_VALUE      \n"
    printf "  --adaptive, -a         \n"
    printf "  --unconcurrent, -uc       \n"
    printf "  --angle=DEGREE_VALUE   \n"
    printf "  --angle1=DEGREE_VALUE archie   \n"
    printf "  --angle2=DEGREE_VALUE betty  \n"
    printf "  --cool=COOL_FAC        \n"
    printf "  --just_build, -j       \n"
    printf "  --help, -h             \n"
    exit 0;
fi
#-------------------------------------------------------
#  Part 2: Create the .moos and .bhv files. 
#-------------------------------------------------------  

VNAME2="betty"
START_POS2="50,0"

                                                                                             
nsplug meta_vehicle.moos targ_$VNAME2.moos -f \
   VNAME=$VNAME2      START_POS=$START_POS2                    \
   VPORT="9002"       SHARE_LISTEN="9302"                      \
   SHORE_IP=$SHORE_IP SHORE_LISTEN=$SHORE_LISTEN M200_IP=$M200_IP \
   VTYPE=KAYAK          COOL_FAC=$COOL_FAC  COOL_STEPS=$COOL_STEPS\
   CONCURRENT=$CONCURRENT  ADAPTIVE=$ADAPTIVE

nsplug meta_vehicle.bhv targ_$VNAME2.bhv -f VNAME=$VNAME2      \
    START_POS=$START_POS2 SURVEY_X=$SURVEY_X SURVEY_Y=$SURVEY_Y \
        HEIGHT=$HEIGHT2   WIDTH=$WIDTH2 LANE_WIDTH=$LANE_WIDTH2 \
        DEGREES=$DEGREES2


if [ ${JUST_MAKE} = "yes" ] ; then
    exit 0
fi

#-------------------------------------------------------
#  Part 3: Launch the processes
#-------------------------------------------------------
printf "Launching $VNAME2 MOOS Community (WARP=%s) \n" 
pAntler targ_$VNAME2.moos >& /dev/null &
sleep .25
printf "Done \n"

uMAC targ_$VNAME2.moos

printf "Killing all processes ... \n"
kill %1  
printf "Done killing processes.   \n"

