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

    int mySharedRank,sharedSize;
    MPI_Comm_rank(sm_comm,&mySharedRank);
    MPI_Comm_size(sm_comm,&sharedSize);
    
    double elapsed_time_x, elapsed_time_y, elapsed_time_z;
    int max_iterations=1;                                  // number of iterations

    if (argc > 1 ) {
        max_iterations=atoi(argv[1]);
        //MPI_Bcast(&max_iterations, 1, MPI_INT, 0,MPI_COMM_WORLD);
    } // end if //

    if (mySharedRank == root) {
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
    
    t1 = dimCube(LEV2,ROW2,COL2, &sm_win_t1, mySharedRank,&sm_comm);
    t2 = dimCube(LEV2,ROW2,COL2, &sm_win_t2, mySharedRank,&sm_comm);

    MPI_Win_lock_all(0,sm_win_t1);
    MPI_Win_lock_all(0,sm_win_t2);


    const int start = BLOCK_LOW (mySharedRank,sharedSize,(zdim-1))+1;
    const int end   = BLOCK_HIGH(mySharedRank,sharedSize,(zdim-1))+2;
    
    if (mySharedRank == root) {
        setFun(t1, xdim, ydim, zdim);
    } // end if

    //////////////////////////// x //////////////////////////
    MPI_Win_sync(sm_win_t1);
    //MPI_Win_sync(sm_win_t2);
    MPI_Barrier(sm_comm);
    elapsed_time_x = -MPI_Wtime();
    for (int n=1 ; n<=max_iterations; ++n) {
        secDev_x(t2,t1, xdim, ydim, start,end);
        //MPI_Win_sync(sm_win_t1);
        MPI_Win_sync(sm_win_t2);
        MPI_Barrier(sm_comm);
    }	// end of time loop n = 1,...,nstep //
    MPI_Barrier(sm_comm);
    elapsed_time_x += MPI_Wtime();
    //////////////////////////// x //////////////////////////


    //////////////////////////// y //////////////////////////
    MPI_Win_sync(sm_win_t1);
    //MPI_Win_sync(sm_win_t2);
    MPI_Barrier(sm_comm);
    elapsed_time_y = -MPI_Wtime();
    for (int n=1 ; n<=max_iterations; ++n) {
        secDev_y(t2,t1, xdim, ydim, start,end);
        //MPI_Win_sync(sm_win_t1);
        MPI_Win_sync(sm_win_t2);
        MPI_Barrier(sm_comm);
    }	// end of time loop n = 1,...,nstep //
    MPI_Barrier(sm_comm);
    elapsed_time_y += MPI_Wtime();
    //////////////////////////// y //////////////////////////

    //////////////////////////// z //////////////////////////
    MPI_Win_sync(sm_win_t1);
    //MPI_Win_sync(sm_win_t2);
    MPI_Barrier(sm_comm);
    elapsed_time_z = -MPI_Wtime();
    for (int n=1 ; n<=max_iterations; ++n) {
        secDev_z(t2,t1, xdim, ydim, start,end);
        //MPI_Win_sync(sm_win_t1);
        MPI_Win_sync(sm_win_t2);
        MPI_Barrier(sm_comm);
    }	// end of time loop n = 1,...,nstep //
    MPI_Barrier(sm_comm);
    elapsed_time_z += MPI_Wtime();
    //////////////////////////// z //////////////////////////


    MPI_Win_unlock_all(sm_win_t1);
    MPI_Win_unlock_all(sm_win_t2);

    
    if (mySharedRank == root) {
        printf ("for %d threads it tooks %14.6e seconds to finish x, %14.6e seconds to finish y, %14.6e seconds to finish z\n", 
        sharedSize,elapsed_time_x/max_iterations,elapsed_time_y/max_iterations,elapsed_time_z/max_iterations );
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

