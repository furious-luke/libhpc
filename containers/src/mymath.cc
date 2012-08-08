#include <math.h>
#include "libhpc/debug/debug.hh"
#include "num.hh"
#include "mymath.hh"

int powi(int x, int e) {
    int r = 1;
    ASSERT(x >= 0);
    while(e) {
	r *= x;
	e--;
    }
    return r;
}

void tri_bary(const double** crds, const double* pnt, double* bc) {
    double a, b, c, d, e, f;

    a = crds[0][0] - crds[2][0];
    b = crds[1][0] - crds[2][0];
    c = crds[2][0] - pnt[0];
    d = crds[0][1] - crds[2][1];
    e = crds[1][1] - crds[2][1];
    f = crds[2][1] - pnt[1];

    bc[0] = (b*f - c*e)/(a*e - b*d);
    bc[1] = (a*f - c*d)/(b*d - a*e);
    bc[2] = 1.0 - bc[0] - bc[1];
}
