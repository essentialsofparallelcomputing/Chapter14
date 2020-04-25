#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sched.h>
#include <mpi.h>
#include <omp.h>

/* Heavily modified from xthi.c code */
/* xthi.c code is used in examples for hybrid MPI/OpenMP affinity from a few HPC sites */
/* xthi.c originally borrowed some of this code from util-linux-2.13-pre7/schedutils/taskset.c */
static char *cpuset_to_cstr(cpu_set_t *mask, char *str)
{
  char *ptr = str;
  int i, j, entry_made = 0;
  for (i = 0; i < CPU_SETSIZE; i++) {
    if (CPU_ISSET(i, mask)) {
      int run = 0;
      entry_made = 1;
      for (j = i + 1; j < CPU_SETSIZE; j++) {
        if (CPU_ISSET(j, mask)) run++;
        else break;
      }
      if (!run)
        sprintf(ptr, "%d,", i);
      else if (run == 1) {
        sprintf(ptr, "%d,%d,", i, i + 1);
        i++;
      } else {
        sprintf(ptr, "%d-%d,", i, i + run);
        i += run;
      }
      while (*ptr != 0) ptr++;
    }
  }
  ptr -= entry_made;
  *ptr = 0;
  return(str);
}

void place_report_mpi_omp(void)
{
   #pragma omp parallel
   {
      if (omp_get_thread_num() == 0){
         printf("Running with %d thread(s)\n",omp_get_num_threads());
         int bind_policy = omp_get_proc_bind();
         switch (bind_policy)
         {
            case omp_proc_bind_false:
               printf("  proc_bind is false\n");
               break;
            case omp_proc_bind_true:
               printf("  proc_bind is true\n");
               break;
            case omp_proc_bind_master:
               printf("  proc_bind is master\n");
               break;
            case omp_proc_bind_close:
               printf("  proc_bind is close\n");
               break;
            case omp_proc_bind_spread:
               printf("  proc_bind is spread\n");
         }
         printf("  proc_num_places is %d\n",omp_get_num_places());
      }
   }

   int socket_global[144];
   int rank;
   char clbuf_global[144][7 * CPU_SETSIZE];

   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   #pragma omp parallel
   {
      int thread = omp_get_thread_num();
      cpu_set_t coremask;
      char clbuf[7 * CPU_SETSIZE], hnbuf[64];
      memset(clbuf, 0, sizeof(clbuf));
      memset(hnbuf, 0, sizeof(hnbuf));
      gethostname(hnbuf, sizeof(hnbuf));
      sched_getaffinity(0, sizeof(coremask), &coremask);
      cpuset_to_cstr(&coremask, clbuf);
      strcpy(clbuf_global[thread],clbuf);
      socket_global[omp_get_thread_num()] = omp_get_place_num();
      #pragma omp barrier
      #pragma omp master
      for (int i=0; i<omp_get_num_threads(); i++){
         printf("Hello from rank %d, thread %d, on %s. (core affinity = %s)\n",
                 rank, i, hnbuf, clbuf_global[i], socket_global[i]);
      }
   }
}
