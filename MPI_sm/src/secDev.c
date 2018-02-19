#include <mpi.h> 
#include "real.h"
#include "prototypes.h"
#include "myMPI.h"


#include <stdio.h>
#include <stdlib.h>

void secDev_x(real ***restrict dev,real ***restrict fun, int xdim, int ydim,int start, int end )  
{
    // derivative in x
    for(int l = start; l < end; ++l) {
        for(int r = 1; r < ydim ; ++r) {
            for(int c = 1; c < xdim; ++c) {
                dev[l][r][c] =  0.25*(fun[l][r][c+1] + fun[l][r][c-1] - 2.0* fun[l][r][c]);
            } // end for //
        } // end for //
    } // end for //
    //MPI_Barrier(MPI_COMM_WORLD);

} // end of secDev_x() //

void secDev_y(real ***restrict dev,real ***restrict fun, int xdim, int ydim,int start, int end )  
{
    // derivative in y
    for(int l = start; l < end; ++l) {
        for(int c = 1; c < xdim; ++c) {
            for(int r = 1; r < ydim ; ++r) {
                dev[l][r][c] =  0.25*(fun[l][r+1][c] + fun[l][r-1][c] - 2.0* fun[l][r][c]);
            } // end for //
        } // end for //
    } // end for //
    //MPI_Barrier(MPI_COMM_WORLD);

} // end of secDev_y() //



void secDev_z(real ***restrict dev,real ***restrict fun, int xdim, int ydim,int start, int end )  
{
    // derivative in z
    for(int r = 1; r < ydim ; ++r) {
        for(int c = 1; c < xdim; ++c) {
            for(int l = start; l < end; ++l) {
                dev[l][r][c] =  0.25*(fun[l+1][r][c] + fun[l-1][r][c] - 2.0* fun[l][r][c]);
            } // end for //
        } // end for //
    } // end for //
    //MPI_Barrier(MPI_COMM_WORLD);

} // end of secDev_y() //



/*
    int myWorldRank;
    MPI_Comm_rank(MPI_COMM_WORLD,&myWorldRank);
    
    if (myWorldRank == 1) {
        for(int l = start; l < end; ++l) {
            for(int r = 1; r < ydim ; ++r) {
                for(int c = 1; c < xdim; ++c) {
                    printf("%f  ", dev[l][r][c]);
                } // end for //
                printf("\n");
            } // end for //
            printf("\n\n\n");
        } // end for //
        
    } // end if //
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    exit(0);
*/
