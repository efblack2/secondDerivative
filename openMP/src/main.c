// size of plate
#define ROWS       640
#define COLUMNS    640
#define LEVELS     640

#define ROW2       (ROWS+2)
#define COL2       (COLUMNS+2)
#define LEV2       (LEVELS+2)

#include <stdio.h>
#include <stdlib.h>
/*
#include <ctype.h>
#include <math.h>
*/
#include <sys/time.h>

#include <omp.h> 
#include "real.h"
#include "dimCube.h"
#include "prototypes.h"


int main(int argc, char *argv[])
{
    int nthreads=0;
    double elapsed_time;
    struct timeval tp;

    int max_iterations;                                  // number of iterations

    if (argc > 1 ) {
        max_iterations=atoi(argv[1]);
    } else {
        printf("Maximum iterations [100-4000]?\n");
        if ( !scanf("%d", &max_iterations)  || max_iterations < 0 ) {
            printf("wrong input value\nBye...\n");
            exit(0);
        } // end if //       
    } // endif //
    printf("Ruuning %d iterations \n",max_iterations);

    if (sizeof(real) == 8) {
        printf("Double precision version\n");
    } else {
        printf("Single precision version\n");
    } // end if



// Arrays and other variables //

    const int xdim=ROW2-1;
    const int ydim=COL2-1;
    const int zdim=LEV2-1;
    
    real ***t1, ***t2;
    t1 = dimCube(LEV2,ROW2,COL2);
    t2 = dimCube(LEV2,ROW2,COL2);


    setFun(t1, xdim, ydim, zdim);
    
    gettimeofday(&tp,NULL);
    elapsed_time = -(tp.tv_sec*1.0e6 + tp.tv_usec);  

     
    #pragma omp parallel 
    {
        for (int n=1 ; n<=max_iterations; ++n) {
            secDev(t2,t1,xdim,ydim,zdim);
        }	// end of time loop n = 1,...,nstep //
        
        #pragma omp single nowait
        nthreads = omp_get_num_threads();
        // end of omp single
    } // end of parallel region //
    
    
    gettimeofday(&tp,NULL);
    elapsed_time += (tp.tv_sec*1.0e6 + tp.tv_usec);
    printf ("\n\nIt tooks %14.6e seconds for %d threads to finish\n", elapsed_time*1.0e-6, nthreads);

    if (sizeof(real) == 8) {
        printf("Double precision version\n");
    } else {
        printf("Single precision version\n");
    } // end if

    
    freeCube(&t2);
    freeCube(&t1);

    return 0;
} // end main() //

