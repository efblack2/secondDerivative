#include <mpi.h> 
#include "real.h"
#include "prototypes.h"
#include "dataDef.h"

void secDer_x(real *restrict der, real *fun, int xdim, int ydim,int start, int end )  
{
    // derivative in x
    
    for(int l = start; l < end; ++l) {
        for(int r = 1; r < ydim ; ++r) {
            for(int c = 1; c < xdim; ++c) {
                int k = c + r*COL2 + l*COL2*ROW2;
                der[k] =  0.25*(fun[k+1] + fun[k-1] - 2.0 * fun[k]);
                //der[l][r][c] =  0.25*(fun[l][r][c+1] + fun[l][r][c-1] - 2.0 * fun[l][r][c]);
            } // end for //
        } // end for //
    } // end for //
} // end of secDer_x() //

void secDer_y(real *restrict der,real *fun, int xdim, int ydim,int start, int end )  
{
    // derivative in y
    
    for(int c = 1; c < xdim; ++c) {
        for(int l = start; l < end; ++l) {
            for(int r = 1; r < ydim ; ++r) {
                int k = c + r*COL2 + l*COL2*ROW2;
                der[k] =  0.25*(fun[k+COL2] + fun[k-COL2] - 2.0 * fun[k]);
                //der[l][r][c] =  0.25*(fun[l][r][c+1] + fun[l][r][c-1] - 2.0 * fun[l][r][c]);
            } // end for //
        } // end for //
    } // end for //
} // end of secDer_y() //

void secDer_z(real *restrict der,real *fun, int xdim, int ydim,int start, int end )  
{
    // derivative in z
    
    for(int c = 1; c < xdim; ++c) {
        for(int r = 1; r < ydim; ++r) {
            for(int l = start; l < end; ++l) {
                int k = c + r*COL2 + l*COL2*ROW2;
                der[k] =  0.25*(fun[k+COL2*ROW2] + fun[k-COL2*ROW2] - 2.0 * fun[k]);
            } // end for //
        } // end for //
    } // end for //
} // end of secDer_z() //



/*
    int myWorldRank;
    MPI_Comm_rank(MPI_COMM_WORLD,&myWorldRank);
    
    if (myWorldRank == 1) {
        for(int l = start; l < end; ++l) {
            for(int r = 1; r < ydim ; ++r) {
                for(int c = 1; c < xdim; ++c) {
                    printf("%f  ", der[l][r][c]);
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
