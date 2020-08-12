All: OpenMP MPI Hybrid Quo
.PHONY: OpenMP MPI Hybrid Quo

OpenMP:
	cd OpenMP && mkdir build && cd build && cmake -DCMAKE_VERBOSE=on .. && make && ./vecadd_opt3 && \
	likwid-pin -c 0-4 ./vecadd_opt3

MPI:
	cd MPI && mkdir build && cd build && cmake .. && make && ./MPIAffinity | sort -n -k 4 && \
	likwid-mpirun -n 4 ./MPIAffinity # && \
        # ../mpirun_distrib.sh "1 4" ./MPIAffinity

Hybrid:
	cd StreamTriad && mkdir build && cd build && cmake -DCMAKE_VERBOSE=1 .. && make && mpirun -n 4 ./StreamTriad

Quo:
	cd Quo && make autobind && mpirun -n 4 ./autobind && make dynaffinity && mpirun -n 4 ./dynaffinity

clean:
	cd OpenMP && rm -rf build
	cd MPI && rm -rf build
	cd StreamTriad && rm -rf build
	cd Quo && make clean




