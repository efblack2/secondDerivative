#include "real.h"

void setFun(real ***a, int xdim, int ydim, int start, int end)
{
    int k = 0;
    for(int l = start; l < end; ++l) {
        for(int r = 1; r < ydim ; ++r) {
            for(int c = 1; c < xdim; ++c) {
                a[l][r][c] = (real) ++k;
            } // end for //
        } // end for //
    } // end for //
} // end of setFun() //
