#include <mpi.h>
#include <stdio.h>
#include "place_report_mpi.h"
int main(int argc, char **argv)
{
   MPI_Init(&argc, &argv);

   place_report_mpi();

   MPI_Finalize();
   return 0;
}
