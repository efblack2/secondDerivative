#include <omp.h>
#include "real.h"
#include "prototypes.h"
#include "dataDef.h"

void secDer_x(real *restrict der,real *fun, int xdim, int ydim, int zdim) 
{
    // derivative in x
    #pragma omp for schedule(static) nowait
    for(int l = 1; l < zdim; ++l) {
        for(int r = 1; r < ydim ; ++r) {
            for(int c = 1; c < xdim; ++c) {
                int k = c + r*COL2 + l*ROW2*COL2;
                der[k] =  0.25*(fun[k+1] + fun[k-1] - 2.0 * fun[k]);
                //der[l][r][c] =  0.25*(fun[l][r][c+1] + fun[l][r][c-1] - 2.0 * fun[l][r][c]);
            } // end for //
        } // end for //
    } // end for //
} // end of secDer_x() //

void secDer_y(real *restrict der,real *fun, int xdim, int ydim, int zdim) 
{
    // derivative in y
    for(int c = 1; c < xdim; ++c) {
        #pragma omp for schedule(static) nowait
        for(int l = 1; l < zdim; ++l) {
            for(int r = 1; r < ydim ; ++r) {
                int k = c + r*COL2 + l*ROW2*COL2;
                der[k] =  0.25*(fun[k+COL2] + fun[k-COL2] - 2.0 * fun[k]);
                //der[l][r][c] =  0.25*(fun[l][r+1][c] + fun[l][r-1][c] - 2.0 * fun[l][r][c]);
            } // end for //            
        } // end for //
    } // end for //
} // end of secDer_y() //

void secDer_z(real *restrict der,real *fun, int xdim, int ydim, int zdim) 
{
    // derivative in z
    for(int c = 1; c < xdim; ++c) {
        for(int r = 1; r < ydim ; ++r) {
            #pragma omp for schedule(static) nowait
            for(int l = 1; l < zdim; ++l) {
                int k = c + r*COL2 + l*ROW2*COL2;
                der[k] =  0.25*(fun[k+ROW2*COL2] + fun[k-ROW2*COL2] - 2.0 * fun[k]);
                //der[l][r][c] =  0.25*(fun[l+1][r][c] + fun[l-1][r][c] - 2.0 * fun[l][r][c]);
            } // end for //
        } // end for //
    } // end for //
} // end of secDer_z() //


/*
    #pragma omp single
    //#pragma omp for schedule(static) // nowait
    for(int l = 1; l < zdim; ++l) {
        for(int r = 1; r < ydim ; ++r) {
            for(int c = 1; c < xdim; ++c) {
                printf("%f  ", der[l][r][c]);
            } // end for //
            printf("\n");
        } // end for //
        printf("\n\n\n");
    } // end for //
    #pragma omp barrier
    exit(0);
*/

