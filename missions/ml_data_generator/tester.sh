#!/bin/bash

COUNT=0
while true ; do
    echo "tester launching mission"
    ./launch.sh 15 &
    last_pid=$!   
    
    echo "tester deploying robots"
    uPokeDB  targ_shoreside.moos DEPLOY_ALL=true MOOS_MANUAL_OVERRIDE_ALL=false RETURN_ALL=false STATION_KEEP_ALL=false
    sleep $[15]m

    echo "tester killing application"
    kill -KILL $last_pid 2> /dev/null
    sleep $[5]m

    echo "tester killing the moos"
    ktm
    sleep $[5]m

    echo "tester making test result folder"
    cd test_results
    mkdir test_$COUNT
    cd ..
    sleep $[10]m
    
    echo "tester grepping log_gilda data"
    cd LOG_GILDA_*
    aloggrep L*.alog NODE_REPORT_LOCAL gilda.alog
    cd ..
    mv LOG_GILDA_*/gilda.alog test_results/test_$COUNT/gilda.alog
    sleep $[1]m

    echo "tester grepping log_henry data"
    cd LOG_HENRY_*
    aloggrep L*.alog NODE_REPORT_LOCAL henry.alog
    cd ..
    mv LOG_HENRY_*/henry.alog test_results/test_$COUNT/henry.alog
    sleep $[1]m

    echo "tester grepping log_shoreside data"
    cd LOG_SHORESIDE_*
    aloggrep L*.alog NODE_REPORT shoreside.alog
    cd ..
    mv LOG_SHORESIDE_*/shoreside.alog test_results/test_$COUNT/shoreside.alog
    sleep $[1]m
    
    echo "tester cleaning folder"
    ./clean.sh
    sleep $[1]m
    
    echo "tester restarting"
    COUNT=$((COUNT+1)) 
done
