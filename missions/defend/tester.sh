#!/bin/bash

COUNT=0
while true ; do
#-----------------------------------------------------
#        Evan and Felix Attacking
#-----------------------------------------------------    
    echo "launching mission with both robots attacking"
    ./launch_EA_FA.sh &
    last_pid=$!   
    sleep $[5]m
    
    echo "tester deploying robots"
    uPokeDB  targ_shoreside.moos DEPLOY_ALL=true MOOS_MANUAL_OVERRIDE_ALL=false RETURN_ALL=false STATION_KEEP_ALL=false

    count=0
    uQueryDB targ_shoreside.moos --condition="((TAGGED_VEHICLES=evan) or (TAGGED_VEHICLES=felix) or (RED_FLAG_GRABBED=true) or (BLUE_FLAG_GRABBED=true))"
    while [[ $? == 1 && $count -lt 20 ]] ; do
	sleep $[3]m
	count=$((count+1))
	uQueryDB targ_shoreside.moos --condition="(((TAGGED_VEHICLES=evan) or (TAGGED_VEHICLES=felix)) or ((RED_FLAG_GRABBED=true) or (BLUE_FLAG_GRABBED=true)))"
    done
       	
    echo "tester killing application"
    kill -KILL $last_pid > /dev/null
    sleep $[5]m

    echo "tester killing the moos"
    ktm
    sleep $[10]m

    echo "tester making test result folder"
    if [[ $COUNT == 0 ]] ; then
	mkdir results
	cd results
	mkdir results_EA_FA
    else
	cd results
    fi
    cd results_EA_FA
    mkdir test_$COUNT
    cd ../..
    
    echo "tester grepping log_evan data"
    cd m200
    cd LOG_EVAN_*
    aloggrep L*.alog NODE_REPORT_LOCAL evan.alog
    cd ../..
    mv m200/LOG_EVAN_*/evan.alog results/results_EA_FA/test_$COUNT/evan.alog

    echo "tester grepping log_felix data"
    cd m200
    cd LOG_FELIX_*
    aloggrep L*.alog NODE_REPORT_LOCAL felix.alog
    cd ../..
    mv m200/LOG_FELIX_*/felix.alog results/results_EA_FA/test_$COUNT/felix.alog

    echo "tester grepping log_shoreside data"
    cd shoreside
    cd LOG_SHORESIDE_*
    aloggrep L*.alog NODE_REPORT shoreside.alog
    cd ../..
    mv shoreside/LOG_SHORESIDE_*/shoreside.alog results/results_EA_FA/test_$COUNT/shoreside.alog
    
    echo "tester cleaning folder"
    ./clean.sh
    sleep $[5]m

#----------------------------------------------------------
#    Evan and Felix Defending
#----------------------------------------------------------
    echo "testing with both robots defending"
    ./launch_ED_FD.sh &
    last_pid=$!   
    sleep $[5]m
    
    echo "tester deploying robots"
    uPokeDB  targ_shoreside.moos DEPLOY_ALL=true MOOS_MANUAL_OVERRIDE_ALL=false RETURN_ALL=false STATION_KEEP_ALL=false

    count=0
    uQueryDB targ_shoreside.moos --condition="((TAGGED_VEHICLES=evan) or (TAGGED_VEHICLES=felix) or (RED_FLAG_GRABBED=true) or (BLUE_FLAG_GRABBED=true))"
    while [[ $? == 1 && $count -lt 20 ]] ; do
	sleep $[3]m
	count=$((count+1))
	uQueryDB targ_shoreside.moos --condition="(((TAGGED_VEHICLES=evan) or (TAGGED_VEHICLES=felix)) or ((RED_FLAG_GRABBED=true) or (BLUE_FLAG_GRABBED=true)))"
    done

    echo "tester killing application"
    kill -KILL $last_pid 2> /dev/null
    sleep $[5]m

    echo "tester killing the moos"
    ktm
    sleep $[10]m

    echo "tester making test result folder"
    cd results
    if [ $COUNT == 0 ] ; then
       mkdir results_ED_FD
    fi
    cd results_ED_FD
    mkdir test_$COUNT
    cd ../..
    
    echo "tester grepping log_evan data"
    cd m200
    cd LOG_EVAN_*
    aloggrep L*.alog NODE_REPORT_LOCAL evan.alog
    cd ../..
    mv m200/LOG_EVAN_*/evan.alog results/results_ED_FD/test_$COUNT/evan.alog

    echo "tester grepping log_felix data"
    cd m200
    cd LOG_FELIX_*
    aloggrep L*.alog NODE_REPORT_LOCAL felix.alog
    cd ../..
    mv m200/LOG_FELIX_*/felix.alog results/results_ED_FD/test_$COUNT/felix.alog

    echo "tester grepping log_shoreside data"
    cd shoreside
    cd LOG_SHORESIDE_*
    aloggrep L*.alog NODE_REPORT shoreside.alog
    cd ../..
    mv shoreside/LOG_SHORESIDE_*/shoreside.alog results/results_ED_FD/test_$COUNT/shoreside.alog
    
    echo "tester cleaning folder"
    ./clean.sh
    sleep $[5]m

#----------------------------------------------------------
#     Evan Attacking and Felix Defending
#----------------------------------------------------------
    ./launch_EA_FD.sh &
    last_pid=$!   
    sleep $[5]m
    
    echo "tester deploying robots"
    uPokeDB  targ_shoreside.moos DEPLOY_ALL=true MOOS_MANUAL_OVERRIDE_ALL=false RETURN_ALL=false STATION_KEEP_ALL=false

    count=0
    uQueryDB targ_shoreside.moos --condition="((TAGGED_VEHICLES=evan) or (TAGGED_VEHICLES=felix) or (RED_FLAG_GRABBED=true) or (BLUE_FLAG_GRABBED=true))"
    while [[ $? == 1 && $count -lt 20 ]] ; do
	sleep $[3]m
	count=$((count+1))
	uQueryDB targ_shoreside.moos --condition="(((TAGGED_VEHICLES=evan) or (TAGGED_VEHICLES=felix)) or ((RED_FLAG_GRABBED=true) or (BLUE_FLAG_GRABBED=true)))"
    done

    echo "tester killing application"
    kill -KILL $last_pid 2> /dev/null
    sleep $[5]m

    echo "tester killing the moos"
    ktm
    sleep $[10]m

    echo "tester making test result folder"
    cd results
    if [ $COUNT == 0 ] ; then
       mkdir results_EA_FD
    fi
    cd results_EA_FD
    mkdir test_$COUNT
    cd ../..
    
    echo "tester grepping log_evan data"
    cd m200
    cd LOG_EVAN_*
    aloggrep L*.alog NODE_REPORT_LOCAL evan.alog
    cd ../..
    mv m200/LOG_EVAN_*/evan.alog results/results_EA_FD/test_$COUNT/evan.alog

    echo "tester grepping log_felix data"
    cd m200
    cd LOG_FELIX_*
    aloggrep L*.alog NODE_REPORT_LOCAL felix.alog
    cd ../..
    mv m200/LOG_FELIX_*/felix.alog results/results_EA_FD/test_$COUNT/felix.alog

    echo "tester grepping log_shoreside data"
    cd shoreside
    cd LOG_SHORESIDE_*
    aloggrep L*.alog NODE_REPORT shoreside.alog
    cd ../..
    mv shoreside/LOG_SHORESIDE_*/shoreside.alog results/results_EA_FD/test_$COUNT/shoreside.alog
    
    echo "tester cleaning folder"
    ./clean.sh
    sleep $[5]m
 #--------------------------------------------------------
 #       Evan Defending and Felix Attacking
 #--------------------------------------------------------
    ./launch_ED_FA.sh &
    last_pid=$!   
    sleep $[5]m
    
    echo "tester deploying robots"
    uPokeDB  targ_shoreside.moos DEPLOY_ALL=true MOOS_MANUAL_OVERRIDE_ALL=false RETURN_ALL=false STATION_KEEP_ALL=false

    count=0
    uQueryDB targ_shoreside.moos --condition="((TAGGED_VEHICLES=evan) or (TAGGED_VEHICLES=felix) or (RED_FLAG_GRABBED=true) or (BLUE_FLAG_GRABBED=true))"
    while [[ $? == 1 && $count -lt 20 ]] ; do
	sleep $[3]m
	count=$((count+1))
	uQueryDB targ_shoreside.moos --condition="(((TAGGED_VEHICLES=evan) or (TAGGED_VEHICLES=felix)) or ((RED_FLAG_GRABBED=true) or (BLUE_FLAG_GRABBED=true)))"
    done

    echo "tester killing application"
    kill -KILL $last_pid 2> /dev/null
    sleep $[5]m

    echo "tester killing the moos"
    ktm
    sleep $[10]m

    echo "tester making test result folder"
    cd results
    if [ $COUNT == 0 ] ; then
       mkdir results_ED_FA
    fi
    cd results_ED_FA
    mkdir test_$COUNT
    cd ../..
    
    echo "tester grepping log_evan data"
    cd m200
    cd LOG_EVAN_*
    aloggrep L*.alog NODE_REPORT_LOCAL evan.alog
    cd ../..
    mv m200/LOG_EVAN_*/evan.alog results/results_ED_FA/test_$COUNT/evan.alog

    echo "tester grepping log_felix data"
    cd m200
    cd LOG_FELIX_*
    aloggrep L*.alog NODE_REPORT_LOCAL felix.alog
    cd ../..
    mv m200/LOG_FELIX_*/felix.alog results/results_ED_FA/test_$COUNT/felix.alog

    echo "tester grepping log_shoreside data"
    cd shoreside
    cd LOG_SHORESIDE_*
    aloggrep L*.alog NODE_REPORT shoreside.alog
    cd ../..
    mv shoreside/LOG_SHORESIDE_*/shoreside.alog results/results_ED_FA/test_$COUNT/shoreside.alog
    
    echo "tester cleaning folder"
    ./clean.sh
    sleep $[5]m
    
    echo "tester restarting"
    COUNT=$((COUNT+1)) 
done
