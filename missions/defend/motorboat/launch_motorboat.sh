#!/bin/bash

SHORE_IP=192.168.1.151
SHORE_LISTEN="9300"

TIME_WARP=1
HELP="no"
JUST_BUILD="no"
START_POS="0,0,0"
VNAME="chase"
VPORT="9013"
SHARE_LISTEN="9313"

# for coloring purposes
VTEAM="blue"

for ARGI; do
    if [ "${ARGI}" = "--help" -o "${ARGI}" = "-h" ] ; then
        HELP="yes"
    elif [ "${ARGI//[^0-9]/}" = "$ARGI" -a "$TIME_WARP" = 1 ]; then
        TIME_WARP=$ARGI
    elif [ "${ARGI}" = "--just_build" -o "${ARGI}" = "-j" ] ; then
        JUST_BUILD="yes"
        echo "Just building files; no vehicle launch."
   else
      echo "Undefined argument:" $ARGI
      echo "Please use -h for help."
      exit 1
    fi
done

if [ "${HELP}" = "yes" ]; then
    echo "$0 [SWITCHES]"
    echo "  --just_build, -j"
    echo "  --help,       -h"
    exit 0;
fi

echo "Assembling MOOS file targ_${VNAME}.moos ."

nsplug meta_motorboat.moos targ_${VNAME}.moos -f  \
       VNAME="${VNAME}"    \
       VPORT=$VPORT                 \
       SHARE_LISTEN=$SHARE_LISTEN   \
       WARP=$TIME_WARP              \
       SHORE_LISTEN=$SHORE_LISTEN   \
       SHORE_IP=$SHORE_IP           \
       VTYPE="mokai"                \
       VTEAM=$VTEAM

echo "Assembling BHV file targ_${VNAME}.bhv ."

nsplug meta_motorboat.bhv targ_${VNAME}.bhv -f  \
       VNAME="${VNAME}"    \
       VPORT=$VPORT                 \
       SHARE_LISTEN=$SHARE_LISTEN   \
       WARP=$WARP                   \
       SHORE_LISTEN=$SHORE_LISTEN   \
       SHORE_IP=$SHORE_IP           \
       VTYPE="mokai"                

if [ ${JUST_BUILD} = "yes" ] ; then
    echo "Files assembled; vehicle not launched; exiting per request."
    exit 0
fi

if [ ! -e targ_${VNAME}.moos ]; then echo "no targ_${VNAME}.moos!"; exit 1; fi
if [ ! -e targ_${VNAME}.bhv ]; then echo "no targ_${VNAME}.bhv!"; exit 1; fi

echo "Launching $VNAME MOOS Community."
pAntler targ_${VNAME}.moos >& /dev/null &
uMAC targ_${VNAME}.moos

echo "Killing all processes ..."
kill -- -$$
echo "Done killing processes."
