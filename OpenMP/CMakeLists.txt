cmake_minimum_required (VERSION 3.0)
project (vecadd_opt3)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set (CMAKE_C_STANDARD 99)

# Set OpenMP flags for a few compilers

find_package(Vector)
find_package(OpenMP)
find_package(MPI)

# Adds build target of vecadd_opt3 with source code files
add_executable(vecadd_opt3 vecadd_opt3.c place_report_omp.c timer.c timer.h)
set_target_properties(vecadd_opt3 PROPERTIES COMPILE_FLAGS ${VECTOR_C_FLAGS})
set_target_properties(vecadd_opt3 PROPERTIES COMPILE_FLAGS "-g ${OpenMP_C_FLAGS} -pthread")
set_target_properties(vecadd_opt3 PROPERTIES LINK_FLAGS "${OpenMP_C_FLAGS}")

# Cleanup
add_custom_target(distclean COMMAND rm -rf CMakeCache.txt CMakeFiles
                  Makefile cmake_install.cmake vecadd_opt3.dSYM ipo_out.optrpt)
