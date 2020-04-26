#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <omp.h>
#include "timer.h"
#include "place_report_mpi_omp.h"

#define NTIMES 50
// large enough to force into main memory
#define STREAM_ARRAY_SIZE 20000000

int main(int argc, char *argv[]){

   int provided;
   MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &provided);

   int nprocs, rank;
   MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   if (rank == 0){
      if (provided != MPI_THREAD_FUNNELED){
         printf("Error: MPI_THREAD_FUNNELED not available. Aborting ...\n");
         MPI_Finalize();
         exit(0);
      }
   }

#ifdef VERBOSE
   place_report_mpi_omp();
#endif

   int ibegin = STREAM_ARRAY_SIZE *(rank  )/nprocs;
   int iend   = STREAM_ARRAY_SIZE *(rank+1)/nprocs;
   int nsize = iend-ibegin;
   double *a = malloc(nsize * sizeof(double));
   double *b = malloc(nsize * sizeof(double));
   double *c = malloc(nsize * sizeof(double));

   // initializing data and arrays
   double scalar = 3.0; //, time_sum = 0.0;
#pragma omp parallel
   {
      #pragma omp for simd
      for (int i=0; i<nsize; i++) {
         a[i] = 1.0;
         b[i] = 2.0;
      }

      double time_sum = 0.0;
      struct timespec tstart;
      cpu_timer_start(&tstart);
      for (int k=0; k<NTIMES; k++){
         // stream triad loop
         #pragma omp for simd
         for (int i=0; i<nsize; i++){
            c[i] = a[i] + scalar*b[i];
         }
         // to keep the compiler from optimizing out the loop
         c[1]=c[2];
      }
      time_sum += cpu_timer_stop(tstart);
      if (rank == 0 && omp_get_thread_num() == 0) printf("Average runtime is %lf msecs\n", time_sum/NTIMES);
   }

   free(a);
   free(b);
   free(c);

   MPI_Finalize();
   return(0);
}

