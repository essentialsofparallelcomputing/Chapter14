#include <mpi.h>
#include <stdio.h>
#include <hwloc.h>
#include "place_report_mpi.h"
int main(int argc, char **argv)
{
   int nprocs;
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

   /* bind on the first hyperthread */
   hwloc_topology_t topology;
   hwloc_topology_init(&topology);
   hwloc_topology_load(topology);

   hwloc_const_bitmap_t set = hwloc_topology_get_complete_cpuset(topology);
   int nbnodes = hwloc_bitmap_weight(set);
   printf("there are %d nodes in the machine\n",nbnodes);
   hwloc_cpuset_t cpuset = hwloc_bitmap_dup(set);
   //hwloc_bitmap_singlify(cpuset);
   hwloc_set_cpubind(topology, cpuset, HWLOC_CPUBIND_PROCESS);
   hwloc_bitmap_free(cpuset);

   place_report_mpi();

   hwloc_topology_destroy(topology);
   MPI_Finalize();
   return 0;
}
