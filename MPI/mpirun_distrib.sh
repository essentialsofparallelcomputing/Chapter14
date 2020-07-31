#!/bin/sh
PROC_LIST=$1
EXEC_NAME=$2
OUTPUT="mpirun "
for core in ${PROC_LIST}
do
    OUTPUT="$OUTPUT  -np 1 hwloc-bind core:${core} ${EXEC_NAME} :"
done
OUTPUT=`echo ${OUTPUT} | sed -e 's/:$/\n/'`
eval ${OUTPUT} 
