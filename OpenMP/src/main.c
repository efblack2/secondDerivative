#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>
#include "real.h"
#include "dimCube.h"
#include "prototypes.h"
#include "dataDef.h"

int main(int argc, char *argv[])
{
    int nthreads=0;
    double elapsed_time_x, elapsed_time_y, elapsed_time_z;
    struct timeval tp;

    int max_iterations=5;                               // number of iterations

    if (argc > 1 ) {
        max_iterations=atoi(argv[1]);
    } // endif //

    printf("Running %d iterations \n",max_iterations);


// Arrays and other variables //

    const int xdim=ROW2-1;
    const int ydim=COL2-1;
    const int zdim=LEV2-1;

    real ***t1, ***t2;
    t1 = dimCube(LEV2,ROW2,COL2);
    t2 = dimCube(LEV2,ROW2,COL2);


    setFun(t1, xdim, ydim, zdim);

    ///////////////////// x ///////////////////////
    gettimeofday(&tp,NULL);
    elapsed_time_x = -(tp.tv_sec*1.0e6 + tp.tv_usec);
    #pragma omp parallel
    {
        for (int n=0; n<max_iterations; ++n) {
            secDer_x(&t2[0][0][0],&t1[0][0][0],xdim,ydim,zdim);
            #pragma omp barrier
        } // end for //
    } // end of parallel region //
    gettimeofday(&tp,NULL);
    elapsed_time_x += (tp.tv_sec*1.0e6 + tp.tv_usec);
    ///////////////////// x ///////////////////////

    ///////////////////// y ///////////////////////
    gettimeofday(&tp,NULL);
    elapsed_time_y = -(tp.tv_sec*1.0e6 + tp.tv_usec);
    #pragma omp parallel
    {
        for (int n=0; n<max_iterations; ++n) {
            secDer_y(&t2[0][0][0],&t1[0][0][0],xdim,ydim,zdim);
            #pragma omp barrier
        } // end for //
    } // end of parallel region //
    gettimeofday(&tp,NULL);
    elapsed_time_y += (tp.tv_sec*1.0e6 + tp.tv_usec);
    ///////////////////// y ///////////////////////


    ///////////////////// z ///////////////////////
    gettimeofday(&tp,NULL);
    elapsed_time_z = -(tp.tv_sec*1.0e6 + tp.tv_usec);
    #pragma omp parallel
    {
        for (int n=0; n<max_iterations; ++n) {
            secDer_z(&t2[0][0][0],&t1[0][0][0],xdim,ydim,zdim);
            #pragma omp barrier
        } // end for //
    } // end of parallel region //
    gettimeofday(&tp,NULL);
    elapsed_time_z += (tp.tv_sec*1.0e6 + tp.tv_usec);
    ///////////////////// z ///////////////////////

    #pragma omp parallel
    {
        #pragma omp master
        nthreads = omp_get_num_threads();
        // end of omp master
    } // end of parallel region //


    printf ("for %d threads it tooks %14.6e seconds to finish x, %14.6e seconds to finish y, %14.6e seconds to finish z\n",
    nthreads,elapsed_time_x*1.0e-6/max_iterations,elapsed_time_y*1.0e-6/max_iterations,elapsed_time_z*1.0e-6/max_iterations );

    freeCube(&t2);
    freeCube(&t1);

    if (sizeof(real) == 8) {
        printf("Double precision version\n");
    } else {
        printf("Single precision version\n");
    } // end if

    return 0;
} // end main() //

