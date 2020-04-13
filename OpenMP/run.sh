#!/bin/sh

calc_avg_stddev()
{
   #echo "Runtime is $1"
   awk '{
     sum = 0.0; sum2 = 0.0       # Initialize to zero
     for (n=1; n <= NF; n++) {   # Process each value on the line
       sum += $n;                # Running sum of values
       sum2 += $n * $n           # Running sum of squares
     }
     print " Number of trials=" NF ",    avg=" sum/NF ", std dev=" sqrt((sum2 - (sum*sum)/NF)/NF);
       }' <<< $1
}

conduct_tests()
{
   echo ""
   echo -n `printenv |grep OMP_` ${exec_string}
   foo=""
   for index in {1..10}
   do
      time_result=`${exec_string}`
      time_val[$index]=${time_result}
      foo="$foo ${time_result}"
   done
   calc_avg_stddev "${foo}"
}

exec_string="./vecadd_opt3 "

conduct_tests

THREAD_COUNT="88 44 22 16 8 4 2 1"

for my_thread_count in ${THREAD_COUNT}
do
   unset OMP_PLACES
   unset OMP_PROC_BIND
   export OMP_NUM_THREADS=${my_thread_count}

   conduct_tests

   PLACES_LIST="threads cores sockets"
   BIND_LIST="true false close spread master"

   for my_place in ${PLACES_LIST}
   do
      for my_bind in ${BIND_LIST}
      do
         export OMP_NUM_THREADS=${my_thread_count}
         export OMP_PLACES=${my_place}
         export OMP_PROC_BIND=${my_bind}

         conduct_tests
      done
   done
done
