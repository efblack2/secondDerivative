// prototypes
#include "real.h"

void secDer_x(real *restrict dev,real *fun, int xdim, int ydim,int start, int end);
void secDer_y(real *restrict dev,real *fun, int xdim, int ydim,int start, int end);
void secDer_z(real *restrict dev,real *fun, int xdim, int ydim,int start, int end);
void setFun(real ***a, int xdim, int ydim, int start, int end);
//void secDev(real ***t1,real ***t2, int xdim, int ydim, int zdim);

// end  of prototypes

