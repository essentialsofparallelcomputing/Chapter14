#include "quo.h"
#include <stdio.h>
#include <unistd.h>
#include "place_report.h"

// Demonstrating QUO auto distribute

int check_errors(int ncores, int nnoderanks, int noderank, int nranks, int nres)
{
    if (ncores < nnoderanks) {
        if (noderank == 0)
            printf("Error: %d core(s) < %d rank(s).\n", ncores, nranks);
        return(-1);
    }
    if (nres == 0) {
        if (noderank == 0)
           printf("Error: nsockets and nNUMA equal zero\n");
        return(-1);
    }
    return(0);
}

void SyncIt(void) 
{
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Barrier(MPI_COMM_WORLD);
    usleep(rank * 1000);
}

int main(int argc, char **argv)
{
    int ncores, nnoderanks, noderank, rank, nranks;
    int work_member = 0, max_members_per_res = 2, nres = 0;
    QUO_context qcontext;

    MPI_Init(&argc, &argv);
    QUO_create(&qcontext, MPI_COMM_WORLD);
    MPI_Comm_size(MPI_COMM_WORLD, &nranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    QUO_id(qcontext, &noderank);
    QUO_nqids(qcontext, &nnoderanks);
    QUO_ncores(qcontext, &ncores);

    QUO_obj_type_t tres = QUO_OBJ_NUMANODE;
    QUO_nnumanodes(qcontext, &nres);
    if (nres == 0) {
        QUO_nsockets(qcontext, &nres);
        tres = QUO_OBJ_SOCKET;
    }

    if ( check_errors(ncores, nnoderanks, noderank, nranks, nres) )
        return(-1);

    if (rank == 0)
        printf("\nDefault binding for MPI processes\n\n");
    place_report_mpi();

    SyncIt();
    QUO_bind_push(qcontext, QUO_BIND_PUSH_PROVIDED,
                  QUO_OBJ_CORE, noderank);
    SyncIt();

    QUO_auto_distrib(qcontext, tres, max_members_per_res,
                     &work_member);
    if (rank == 0)
        printf("\nProcesses should be pinned to the hw cores\n\n");
    place_report_mpi();

    SyncIt();
    QUO_bind_pop(qcontext);
    SyncIt();

    QUO_free(qcontext);
    MPI_Finalize();
    return(0);
}
