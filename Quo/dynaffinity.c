// demo of switching between all MPI and OpenMP threading

#include "quo.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include "place_report.h"

void node_info_report(QUO_context qcontext, int *noderank, int *nnoderanks)
{
    int nnodes, nsockets, ncores, nhwthreads;
    // gather some info
    QUO_nnodes(qcontext, &nnodes);
    QUO_nsockets(qcontext, &nsockets);
    QUO_ncores(qcontext, &ncores);
    QUO_nqids(qcontext, nnoderanks);
    QUO_id(qcontext, noderank);
    QUO_npus(qcontext, &nhwthreads);

    /* print out node info */
    if (*noderank == 0)
        printf("Nodeinfo: nnodes %d nnoderanks %d nsockets %d ncores %d nhwthreads %d\n\n",
           nnodes, *nnoderanks, nsockets, ncores, nhwthreads);
    fflush(stdout);
}

void report_bindings(QUO_context qcontext, int rank){
    char *cbindstr = NULL;
    QUO_stringify_cbind(qcontext, &cbindstr);
    printf("rank %d's cpuset: %s\n", rank, cbindstr);
    free(cbindstr);
}

void SyncIt(void)
{
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Barrier(MPI_COMM_WORLD);
    usleep((rank) * 1000);
}

int main(int argc, char **argv)
{
    int rank, noderank, nnoderanks;
    int work_member = 0, max_members_per_res = 44;
    QUO_context qcontext;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    QUO_create(&qcontext, MPI_COMM_WORLD);

    node_info_report(qcontext, &noderank, &nnoderanks);

    SyncIt();
    QUO_bind_push(qcontext, QUO_BIND_PUSH_PROVIDED,
                  QUO_OBJ_CORE, noderank);
    SyncIt();

    QUO_auto_distrib(qcontext, QUO_OBJ_SOCKET, max_members_per_res,
                     &work_member);

    place_report_mpi_quo(qcontext);

    /* change binding policies to accommodate OMP threads on node 0 */
    bool on_rank_0s_node = rank < nnoderanks;
    if (on_rank_0s_node) {
        if (rank == 0) {
            printf("\nEntering OMP region...\n\n");
            // expands the caller's cpuset to all available resources on the node.
            QUO_bind_push(qcontext, QUO_BIND_PUSH_OBJ, QUO_OBJ_MACHINE, -1);
            report_bindings(qcontext, rank);
            /* do the OpenMP calculation */
            place_report_mpi_omp();
            /* revert to old binding policy */
            QUO_bind_pop(qcontext);
        }
        /* QUO_barrier because it's cheaper than MPI_Barrier on a node. */
        QUO_barrier(qcontext);
    }
    SyncIt();

    // Wrap-up
    QUO_free(qcontext);
    MPI_Finalize();
    return(0);
}
