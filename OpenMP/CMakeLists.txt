cmake_minimum_required (VERSION 3.0)
project (vecadd)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set (CMAKE_C_STANDARD 99)

# Set OpenMP flags for a few compilers

find_package(Vector)
find_package(OpenMP)

# Adds build target of vecadd with source code files
add_executable(vecadd vecadd.c place_report_omp.c timer.c timer.h)
#set_target_properties(vecadd PROPERTIES COMPILE_FLAGS ${CMAKE_VEC_FLAGS})
set_target_properties(vecadd PROPERTIES COMPILE_FLAGS ${OpenMP_C_FLAGS})
set_target_properties(vecadd PROPERTIES LINK_FLAGS "${OpenMP_C_FLAGS}")

# Cleanup
add_custom_target(distclean COMMAND rm -rf CMakeCache.txt CMakeFiles
                  Makefile cmake_install.cmake vecadd.dSYM ipo_out.optrpt)
