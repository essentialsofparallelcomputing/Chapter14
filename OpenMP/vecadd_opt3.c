#include <stdio.h>
#include <time.h>
#include "timer.h"
#include "omp.h"
#include "place_report_omp.h"

// large enough to force into main memory
#define ARRAY_SIZE 80000000
static double a[ARRAY_SIZE], b[ARRAY_SIZE], c[ARRAY_SIZE];

void vector_add(double *c, double *a, double *b, int n);

int main(int argc, char *argv[]){
#ifdef VERBOSE
   place_report_omp();
#endif
   struct timespec tstart;
   double time_sum = 0.0;
#pragma omp parallel
   {
#pragma omp for
      for (int i=0; i<ARRAY_SIZE; i++) {
         a[i] = 1.0;
         b[i] = 2.0;
      }

#pragma omp master
      cpu_timer_start(&tstart);
      vector_add(c, a, b, ARRAY_SIZE);
#pragma omp master
      time_sum += cpu_timer_stop(tstart);
   } // end of omp parallel

   printf(" %lf \n", time_sum);
}

void vector_add(double *c, double *a, double *b, int n)
{
#pragma omp for
   for (int i=0; i < n; i++){
      c[i] = a[i] + b[i];
   }
}
