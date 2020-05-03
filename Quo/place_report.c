
#define _GNU_SOURCE

#include "quo.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
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

#ifdef _OPENMP
void place_report_mpi_omp(void)
{
   int rank;
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

   int socket_global[144];
   char clbuf_global[144][7 * CPU_SETSIZE];

   #pragma omp parallel
   {
      if (omp_get_thread_num() == 0 && rank == 0){
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
         printf("Hello from rank %02d, thread %02d, on %s. (core affinity = %2s) OpenMP socket is %2d\n",
                 rank, i, hnbuf, clbuf_global[i], socket_global[i]);
      }
   }
}
#endif

void place_report_mpi(void)
{
  int rank, nprocs;
  cpu_set_t coremask;
  char clbuf[7 * CPU_SETSIZE], hnbuf[64];

  memset(clbuf, 0, sizeof(clbuf));
  memset(hnbuf, 0, sizeof(hnbuf));

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

  int pid = (int)getpid();
  gethostname(hnbuf, sizeof(hnbuf));
  sched_getaffinity(0, sizeof(coremask), &coremask);
  cpuset_to_cstr(&coremask, clbuf);
  int *pid_global = (int *)malloc(nprocs*sizeof(int));
  char *hnbuf_global = (char *)malloc(nprocs*sizeof(char)*64);
  char *clbuf_global = (char *)malloc(nprocs*sizeof(char)*7*CPU_SETSIZE);
  MPI_Gather(hnbuf,64,MPI_CHARACTER,hnbuf_global,64,MPI_CHAR,0,MPI_COMM_WORLD);
  MPI_Gather(clbuf,7*CPU_SETSIZE,MPI_CHARACTER,clbuf_global,7*CPU_SETSIZE,MPI_CHAR,0,MPI_COMM_WORLD);
  MPI_Gather(&pid,1,MPI_INT,pid_global,1,MPI_INT,0,MPI_COMM_WORLD);
  if (rank == 0){
     for (int irank = 0; irank < nprocs; irank++){
        printf("Hello from process %d, rank %d, on %s. (core affinity = %s)\n",
                pid_global[irank],irank,hnbuf_global+64*irank,clbuf_global+7*CPU_SETSIZE*irank);
     }
  }
  free(hnbuf_global);
  free(clbuf_global);
  free(pid_global);
}

void place_report_mpi_quo(QUO_context quo)
{
  int rank, nprocs;
  char *cbindstr = NULL;
  cpu_set_t coremask;
  char clbuf[7 * CPU_SETSIZE], hnbuf[64];

  memset(clbuf, 0, sizeof(clbuf));
  memset(hnbuf, 0, sizeof(hnbuf));

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

  int pid = (int)getpid();
  QUO_stringify_cbind(quo, &cbindstr);
  int clen = strlen(cbindstr)+1;
  int clen_max;
  MPI_Allreduce(&clen, &clen_max, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
  gethostname(hnbuf, sizeof(hnbuf));
  sched_getaffinity(0, sizeof(coremask), &coremask);
  cpuset_to_cstr(&coremask, clbuf);
  int *pid_global = NULL;
  char *hnbuf_global = NULL;
  char *clbuf_global = NULL;
  char *cbind_global = NULL;
  if (rank == 0){
     pid_global = (int *)malloc(nprocs*sizeof(int));
     hnbuf_global = (char *)malloc(nprocs*sizeof(char)*64);
     clbuf_global = (char *)malloc(nprocs*sizeof(char)*7*CPU_SETSIZE);
     cbind_global = (char *)malloc(nprocs*sizeof(char)*clen_max);
  }
  MPI_Gather(hnbuf,64,MPI_CHARACTER,hnbuf_global,64,MPI_CHAR,0,MPI_COMM_WORLD);
  MPI_Gather(clbuf,7*CPU_SETSIZE,MPI_CHARACTER,clbuf_global,7*CPU_SETSIZE,MPI_CHAR,0,MPI_COMM_WORLD);
  MPI_Gather(&pid,1,MPI_INT,pid_global,1,MPI_INT,0,MPI_COMM_WORLD);
  MPI_Gather(cbindstr,clen_max,MPI_CHARACTER,cbind_global,clen_max,MPI_CHARACTER,0,MPI_COMM_WORLD);
  if (rank == 0){
     for (int irank = 0; irank < nprocs; irank++){
        printf("Hello from process %d, rank %d, on %s. (core affinity = %s) cbind [%s]\n",
                pid_global[irank],irank,hnbuf_global+64*irank,clbuf_global+7*CPU_SETSIZE*irank,
                cbind_global+irank*clen_max);
     }
  }
  free(cbindstr);
  if (rank == 0){
     free(hnbuf_global);
     free(clbuf_global);
     free(pid_global);
     free(cbind_global);
  }
}
