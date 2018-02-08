#include <omp.h>
#include "real.h"
#include "prototypes.h"

#include<stdio.h>
#include<stdlib.h>

void secDev(real ***restrict dev,real ***restrict fun, int xdim, int ydim, int zdim) 
{
    // derivative in x
    #pragma omp for schedule(static) // nowait
    for(int l = 1; l < zdim; ++l) {
        for(int r = 1; r < ydim ; ++r) {
            for(int c = 1; c < xdim; ++c) {
                dev[l][r][c] =  0.25*(fun[l][r][c+1] + fun[l][r][c-1] - 2.0 * fun[l][r][c]);
            } // end for //
        } // end for //
    } // end for //

    // derivative in y
    #pragma omp for schedule(static) // nowait
    for(int l = 1; l < zdim; ++l) {
        for(int r = 1; r < ydim ; ++r) {
            for(int c = 1; c < xdim; ++c) {
                dev[l][r][c] =  0.25*(fun[l][r+1][c] + fun[l][r-1][c] - 2.0 * fun[l][r][c]);
            } // end for //
        } // end for //
    } // end for //

    // derivative in z
    #pragma omp for schedule(static) // nowait
    for(int l = 1; l < zdim; ++l) {
        for(int r = 1; r < ydim ; ++r) {
            for(int c = 1; c < xdim; ++c) {
                dev[l][r][c] =  0.25*(fun[l+1][r][c] + fun[l-1][r][c] - 2.0 * fun[l][r][c]);
            } // end for //
        } // end for //
    } // end for //

//#pragma omp barrier    
} // end of secDev() //

/*
    #pragma omp single
    //#pragma omp for schedule(static) // nowait
    for(int l = 1; l < zdim; ++l) {
        for(int r = 1; r < ydim ; ++r) {
            for(int c = 1; c < xdim; ++c) {
                printf("%f  ", dev[l][r][c]);
            } // end for //
            printf("\n");
        } // end for //
        printf("\n\n\n");
    } // end for //
    #pragma omp barrier
    exit(0);
*/

