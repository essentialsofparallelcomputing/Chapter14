cmake_minimum_required(VERSION 2.8)
project(MPIAffinity)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set (CMAKE_C_STANDARD 99)

set (CFLAGS "-g")

find_package(Vector)
find_package(MPI REQUIRED)

add_executable(MPIAffinity MPIAffinity.c place_report_mpi.c)

set_target_properties(MPIAffinity PROPERTIES INCLUDE_DIRECTORIES "${MPI_C_INCLUDE_PATH}")
set_target_properties(MPIAffinity PROPERTIES COMPILE_FLAGS "${MPI_C_COMPILE_FLAGS}")
set_target_properties(MPIAffinity PROPERTIES LINK_FLAGS "${MPI_C_LINK_FLAGS}")
target_link_libraries(MPIAffinity PRIVATE "${MPI_C_LIBRARIES}")

# Cleanup
add_custom_target(distclean COMMAND rm -rf CMakeCache.txt CMakeFiles
                  Makefile cmake_install.cmake)
