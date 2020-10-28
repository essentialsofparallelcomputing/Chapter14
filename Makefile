All: Explore OpenMP_basic OpenMP_explore OpenMP_likwid_pin MPI_basic MPI_likwid_pin Hybrid Quo
.PHONY: Explore OpenMP_basic OpenMP_explore OpenMP_likwid_pin MPI_basic MPI_hwloc MPI_likwid_pin Hybrid Quo

Explore:
	lstopo
	lscpu

OpenMP_basic:
	cd OpenMP && mkdir build && cd build && cmake -DCMAKE_VERBOSE=on .. && make && \
	export OMP_NUM_THREADS=4 && ./vecadd_opt3 && \
	export OMP_NUM_THREADS=4 && export OMP_PLACES=cores && \
	   export OMP_PROC_BIND=close && ./vecadd_opt3 && \
	export OMP_NUM_THREADS=4 && export OMP_PROC_BIND=spread && \
	   export OMP_PLACES=threads && ./vecadd_opt3

OpenMP_explore:
	cd OpenMP && mkdir build_explore && cd build_explore && cmake -DCMAKE_VERBOSE=on .. && make #&& \
	#../run.sh && \
	#python OpenMP_Affinity.py

OpenMP_likwid_pin:
	cd OpenMP && mkdir build_likwid && cd build_likwid && cmake -DCMAKE_VERBOSE=on .. && make && \
	likwid-pin -c 0-4 ./vecadd_opt3

MPI_basic:
	cd MPI && mkdir build && cd build && cmake .. && make && \
	./MPIAffinity | sort -n -k 4 && \
	mpirun -n 4 ./MPIAffinity |sort -n -k 4 && \
	mpirun -n 4 --npernode 4 ./MPIAffinity |sort -n -k 4 && \
	mpirun -n 4 --npersocket 4 --bind-to hwthread --report-bindings ./MPIAffinity |sort -n -k 4

MPI_hwloc:
	for core in `hwloc-calc --intersect core --sep " " all`; do hwloc-bind \
          core:${core} lstopo --no-io --pid 0 & done && \
	for socket in `hwloc-calc --intersect socket --sep " " all`; do hwloc-bind \
          socket:${socket}.core:0 lstopo --no-io --pid 0 & done && \
	cd MPI && mkdir build_hwloc && cd build_hwloc && cmake .. && make && \
        ../mpirun_distrib.sh "1 4" ./MPIAffinity && \
	mpirun -np 1 hwloc-bind core:1 ./MPIAffinity : -np 1 hwloc-bind core:22 ./MPIAffinity

MPI_likwid_pin:
	cd MPI && mkdir build_likwid && cd build_likwid && cmake .. && make && ./MPIAffinity | sort -n -k 4 && \
	likwid-mpirun -n 4 ./MPIAffinity

Hybrid:
	cd StreamTriad && mkdir build && cd build && cmake -DCMAKE_VERBOSE=1 .. && make && \
	mpirun -n 4 ./StreamTriad && \
	export OMP_NUM_THREADS=2 && export OMP_PROC_BIND=true && mpirun -n 2 ./StreamTriad && \
	../run.sh

Quo:
	cd Quo && make autobind && mpirun -n 4 ./autobind && make dynaffinity && mpirun -n 4 ./dynaffinity

clean:
	cd OpenMP && rm -rf build build_explore build_likwid
	cd MPI && rm -rf build build_hwloc build_likwid
	cd StreamTriad && rm -rf build
	cd Quo && make clean
