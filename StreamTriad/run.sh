#!/bin/sh
LOGICAL_PES_AVAILABLE=`lscpu |grep '^CPU(s):' |cut -d':' -f 2`
SOCKETS_AVAILABLE=`lscpu |grep '^Socket(s):' |cut -d':' -f 2`
THREADS_PER_CORE=`lscpu |grep '^Thread(s) per core:' |cut -d':' -f 2`
POST_PROCESS="\&| grep -e Average -e mpirun |sort -n -k 4"
THREAD_LIST_FULL="2 4 11 22 44"
THREAD_LIST_SHORT="2 11 22"

unset OMP_PLACES
unset OMP_CPU_BIND
unset OMP_NUM_THREADS

RUN_STRING="mpirun -n 44 ./StreamTriad ${POST_PROCESS}"
export OMP_NUM_THREADS=2
echo ${RUN_STRING}
eval ${RUN_STRING}

RUN_STRING="mpirun -n 44 --map-by socket ./StreamTriad ${POST_PROCESS}"
echo ${RUN_STRING}
eval ${RUN_STRING}

export OMP_PROC_BIND=true

for num_threads in ${THREAD_LIST_FULL}
do
   export OMP_NUM_THREADS=${num_threads}

   HW_PES_PER_PROCESS=$((${OMP_NUM_THREADS}/${THREADS_PER_CORE}))
   MPI_RANKS=$((${LOGICAL_PES_AVAILABLE}/${OMP_NUM_THREADS}))
   PES_PER_SOCKET=$((${MPI_RANKS}/${SOCKETS_AVAILABLE}))

   RUN_STRING="mpirun -n ${MPI_RANKS} --map-by socket:PE=${HW_PES_PER_PROCESS} ./StreamTriad ${POST_PROCESS}"
   echo ${RUN_STRING}
   eval ${RUN_STRING}
done

for num_threads in ${THREAD_LIST_FULL}
do
   export OMP_NUM_THREADS=${num_threads}

   HW_PES_PER_PROCESS=$((${OMP_NUM_THREADS}/${THREADS_PER_CORE}))
   MPI_RANKS=$((${LOGICAL_PES_AVAILABLE}/${OMP_NUM_THREADS}))
   PES_PER_SOCKET=$((${MPI_RANKS}/${SOCKETS_AVAILABLE}))

   RUN_STRING="mpirun -n ${MPI_RANKS} --map-by ppr:${PES_PER_SOCKET}:socket:PE=${HW_PES_PER_PROCESS} ./StreamTriad ${POST_PROCESS}"
   echo ${RUN_STRING}
   eval ${RUN_STRING}
done

THREADS_PER_CORE=1
export OMP_PROC_BIND=spread
for num_threads in ${THREAD_LIST_SHORT}
do
   export OMP_NUM_THREADS=${num_threads}

   HW_PES_PER_PROCESS=$((${OMP_NUM_THREADS}/${THREADS_PER_CORE}))
   MPI_RANKS=$((${LOGICAL_PES_AVAILABLE}/${OMP_NUM_THREADS}/2))
   PES_PER_SOCKET=$((${MPI_RANKS}/${SOCKETS_AVAILABLE}))

   RUN_STRING="mpirun -n ${MPI_RANKS} --map-by socket:PE=${HW_PES_PER_PROCESS} ./StreamTriad ${POST_PROCESS}"
   echo ${RUN_STRING}
   eval ${RUN_STRING}
done

for num_threads in ${THREAD_LIST_SHORT}
do
   export OMP_NUM_THREADS=${num_threads}

   HW_PES_PER_PROCESS=$((${OMP_NUM_THREADS}/${THREADS_PER_CORE}))
   MPI_RANKS=$((${LOGICAL_PES_AVAILABLE}/${OMP_NUM_THREADS}/2))
   PES_PER_SOCKET=$((${MPI_RANKS}/${SOCKETS_AVAILABLE}))

   RUN_STRING="mpirun -n ${MPI_RANKS} --map-by ppr:${PES_PER_SOCKET}:socket:PE=${HW_PES_PER_PROCESS} ./StreamTriad ${POST_PROCESS}"
   echo ${RUN_STRING}
   eval ${RUN_STRING}
done
