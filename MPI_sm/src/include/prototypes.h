// prototypes
#include <mpi.h> 
#include "real.h"

void secDev(real ***restrict dev,real ***restrict fun, int xdim, int ydim,int start, int end );
void setFun(real ***a, int xdim, int ydim, int zdim);
//void secDev(real ***t1,real ***t2, int xdim, int ydim, int zdim);

// end  of prototypes

