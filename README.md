# Chapter14 Affinity: truce with the kernel
This is from Chapter 14 of Parallel and High Performance Computing book, Robey and Zamora
Manning Publications, available at http://manning.com

The book may be obtained at
   http://www.manning.com/?a_aid=ParallelComputingRobey

Copyright 2019-2020 Robert Robey, Yuliana Zamora, and Manning Publications
Emails: brobey@earthlink.net, yzamora215@gmail.com

See License.txt for licensing information.

MPI requires either OpenMPI or MPICH
OpenMP comes bundled with most compilers
Hwloc comes with most MPIs

# System exploration commands (Book:Section 14.2)
      Shows output from the lstopo command
      Run with lstopo from command line
      Shows output from the lscpu command
      Run with lscpu from command line

# Placement report for OpenMP (Book: listings 14.1 through 14.3)
      OpenMP directory
      Build using cmake
      mkdir build && cd build && cmake -DCMAKE_VERBOSE=on .. && make
      Run with
      export OMP_NUM_THREADS=<num> && ./vecadd_opt3
      export OMP_PLACES=cores && export OMP_PROC_BIND=close && ./vecadd_opt3

# Exploring all placement and affinity settings (Book: listing 14.4)
      OpenMP directory
      Build with cmake
      mkdir build && cd build && cmake .. && make
      Run with
      ./run.sh
      Plot with
      python OpenMP_Affinity.py

# Process placement with MPI (Book: listings 14.5 and 14.6)
      MPI directory
      Build with cmake
      mkdir build && cd build && cmake .. && make
      mpirun -n 32 ./MPIAffinity |sort -n -k 4
      mpirun -n 32 --npernode 8 ./MPIAffinity |sort -n -k 4
      mpirun -n 32 --npersocket 8 ./MPIAffinity |sort -n -k 4
      mpirun -n 32 --npersocket 8 --bind-to core ./MPIAffinity |sort -n -k 4
      mpirun -n 32 --npersocket 8 --bind-to hwthread ./MPIAffinity |sort -n -k 4
      mpirun -n 32 --npersocket 8 --bind-to hwthread --report-bindings ./MPIAffinity |sort -n -k 4
      
# Process placement for MPI plus OpenMP (Book: listings 14.7 and 14.8)
      StreamTriad directory
      Build with cmake
      mkdir build && cd build && cmake -DCMAKE_VERBOSE=1 .. && make
      Run with
      export OMP_NUM_THREADS=2 && mpirun -n 44 --map-by socket ./StreamTriad
      export OMP_NUM_THREADS=2 && export OMP_PROC_BIND=true && \
         mpirun -n 44 --map-by ppr:22:socket:PE=1 ./StreamTriad
      ./run.sh

# Controlling affinity from the command line with hwloc (Book: listing from 14.9)
      for core in `hwloc-calc --intersect core --sep " " all`; do hwloc-bind \
         core:${core} lstopo --no-io --pid 0 & done
      for socket in `hwloc-calc --intersect socket --sep " " all`; do hwloc-bind \
         socket:${socket}.core:0 lstopo --no-io --pid 0 & done
      build with cmake
      mkdir build && cd build && cmake .. && make
      ./mpirun_distrib.sh "1 22" ./MPIAffinity
      mpirun -np 1 hwloc-bind core:1 ./MPIAffinity : -np 1 hwloc-bind core:22 ./MPIAffinity

# Controlling affinity from the command line with likwid-pin (Book: Section 14.6.2)
      export OMP_NUM_THREADS=44 && export OMP_PROC_BIND=spread && 
         export OMP_PLACES=threads && ./vecadd_opt3
      likwid-pin -c S0:0-21@S1:0-21 ./vecadd_opt3
      In MPI directory
      likwid-mpirun -n 44 ./MPIAffinity |sort -n -k 4
      likwid-mpirun -n 22 ./MPIAffinity |sort -n -k 4
      likwid-mpirun -n 22 -perdomain S:11 ./MPIAffinity |sort -n -k 4

# Setting affinities in your executable (Book: listing 14.10 and 14.11)
      build with makefile
      make autobind
      mpirun -n 44 ./autobind

# Changing affinities from your executable (Book: listing 14.12)
      build with makefile
      make dynaffinity
      mpirun -n 44 ./dynaffinity
