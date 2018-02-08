// size of plate
#define ROWS       640
#define COLUMNS    640
#define LEVELS     640

#define ROW2       (ROWS+2)
#define COL2       (COLUMNS+2)
#define LEV2       (LEVELS+2)

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <sys/time.h>
#include <mpi.h>
#include "real.h"
#include "dimCube.h"
#include "prototypes.h"
#include "myMPI.h"


int main(int argc, char *argv[])
{

    const int root=0; 
    int provided;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);

    MPI_Comm sm_comm;
    MPI_Comm_split_type(MPI_COMM_WORLD,MPI_COMM_TYPE_SHARED, 0,MPI_INFO_NULL, &sm_comm);

    int myWorldRank, worldSize;
    
    MPI_Comm_rank(MPI_COMM_WORLD,&myWorldRank);
    MPI_Comm_size(MPI_COMM_WORLD,&worldSize);

    int mySharedRank,sharedSize;
    MPI_Comm_rank(sm_comm,&mySharedRank);
    MPI_Comm_size(sm_comm,&sharedSize);
    //int nNodes=CEILING(worldSize,sharedSize);
    
    //int nthreads;
    double elapsed_time;
    int max_iterations;                                  // number of iterations

    if (argc > 1 ) {
        max_iterations=atoi(argv[1]);
    } // end if //
    
    if ( (argc <= 1 || max_iterations < 0 ) ) {  // using rank 0 because of scanf()
        if (myWorldRank == 0) {                              // using rank 0 because of scanf()
            printf("Maximum iterations [100-4000]?: ");fflush(stdout);
            while ( !scanf("%d", &max_iterations)  || max_iterations < 0 ) {
                printf("wrong input value. Try again... ");fflush(stdout);
            } // end while //
        } // end if //
        MPI_Bcast(&max_iterations, 1, MPI_INT, 0,MPI_COMM_WORLD);    
    } // endif //

    if (myWorldRank == root) {
        printf("Running %d iterations \n",max_iterations);fflush(stdout);
        if (sizeof(real) == 8) {
            printf("Double precision version\n");
        } else {
            printf("Single precision version\n");
        } // end if
    } // end if
    

// Arrays and other variables //

    real ***t1, ***t2;
    
    const int xdim=ROW2-1;
    const int ydim=COL2-1;
    const int zdim=LEV2-1;
    
   
    MPI_Win sm_win_t1;
    MPI_Win sm_win_t2;
    
    t1 = dimCube(LEV2,ROW2,COL2, &sm_win_t1, myWorldRank,&sm_comm);
    t2 = dimCube(LEV2,ROW2,COL2, &sm_win_t2, myWorldRank,&sm_comm);

    MPI_Win_lock_all(0,sm_win_t1);
    MPI_Win_lock_all(0,sm_win_t2);


    const int start = BLOCK_LOW (myWorldRank,sharedSize,(zdim-1))+1;
    const int end   = BLOCK_HIGH(myWorldRank,sharedSize,(zdim-1))+2;
    
    if (myWorldRank == root) {
        setFun(t1, xdim, ydim, zdim);
    } // end if

    MPI_Win_sync(sm_win_t1);
    //MPI_Win_sync(sm_win_t2);
    MPI_Barrier(sm_comm);
    elapsed_time = -MPI_Wtime();
    

    for (int n=1 ; n<=max_iterations; n++) {
        secDev(t2,t1, xdim, ydim, start,end);
        //  . . . Set boundary conditions for T                     //
        MPI_Win_sync(sm_win_t1);
        MPI_Win_sync(sm_win_t2);
        MPI_Barrier(sm_comm);
    }	// end of time loop n = 1,...,nstep //


    MPI_Win_unlock_all(sm_win_t1);
    MPI_Win_unlock_all(sm_win_t2);
    
    MPI_Barrier(sm_comm);
    elapsed_time += MPI_Wtime();
    
    if (myWorldRank == root) {
        printf ("\n\nIt tooks %14.6e seconds for %d processes to finish\n", elapsed_time, sharedSize);
        if (sizeof(real) == 8) {
            printf("Double precision version\n");
        } else {
            printf("Single precision version\n");
        } // end if
    } // end if

    freeCube(&t2, &sm_win_t2);
    freeCube(&t1, &sm_win_t1);
    MPI_Finalize();

    return 0;
} // end main() //

