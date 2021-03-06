cmake_minimum_required(VERSION 2.8)
project(StreamTriad)

set (CMAKE_C_STANDARD 99)

if (CMAKE_VERBOSE)
   add_definitions(-DVERBOSE)
endif (CMAKE_VERBOSE)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O3")

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

find_package(Vector)

if (CMAKE_VECTOR_VERBOSE)
    set(VECTOR_C_FLAGS "${VECTOR_C_FLAGS} ${VECTOR_C_VERBOSE}")
endif (CMAKE_VECTOR_VERBOSE)

if (CMAKE_VECTOR_NOVEC)
    set(VECTOR_C_FLAGS "${VECTOR_NOVEC_C_FLAGS}")
endif (CMAKE_VECTOR_NOVEC)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${VECTOR_C_FLAGS}")

# Require MPI for this project:
find_package(MPI REQUIRED)

# Require OpenMP for this project:
find_package(OpenMP REQUIRED)

add_executable(StreamTriad StreamTriad.c place_report_mpi_omp.c timer.c timer.h)

set_target_properties(StreamTriad PROPERTIES INCLUDE_DIRECTORIES "${MPI_C_INCLUDE_PATH}")
set_target_properties(StreamTriad PROPERTIES COMPILE_FLAGS "${MPI_C_COMPILE_FLAGS} ${OpenMP_C_FLAGS}")
set_target_properties(StreamTriad PROPERTIES LINK_FLAGS "${OpenMP_C_FLAGS} ${MPI_C_LINK_FLAGS}")
target_link_libraries(StreamTriad PRIVATE "${MPI_C_LIBRARIES}")

# Cleanup
add_custom_target(distclean COMMAND rm -rf CMakeCache.txt CMakeFiles
                  Makefile cmake_install.cmake CTestTestfile.cmake Testing)
