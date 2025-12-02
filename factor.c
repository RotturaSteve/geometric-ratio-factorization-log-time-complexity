// again, grok made this source code to reflect my original idea so that it could be phrased right;
// i created the idea solely I'm just using AI to get it said in a comprehensible way, for example:

//  Geometric Ratio Factorization – public domain 2025
#include <gmp.h>
#include <mpfr.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void factor(mpf_t N, unsigned long prec) {
    mpf_t rho, one, tol, Q, r, theta, sagitta, cap, diff;
    mpf_init2(rho, prec); mpf_init2(one, prec); mpf_set_ui(one,1);
    mpf_init2(tol, prec); mpf_set_ui(tol,1); mpf_div_2exp(tol,tol,prec/2);
    mpf_init2(Q,prec); mpf_init2(r,prec); mpf_init2(theta,prec);
    mpf_init2(sagitta,prec); mpf_init2(cap,prec); mpf_init2(diff,prec);

    // initial bracket
    mpf_t lo, hi;
    mpf_init2(lo,prec); mpf_set_d(lo, 1e-40);
    mpf_init2(hi,prec); mpf_set_d(hi, 1.0);

    // simple Brent–Dekker (converges in ~6·bits iterations)
    for(int iter=0; iter<100000; iter++) {
        mpf_add(rho, lo, hi); mpf_div_ui(rho, rho, 2);           // midpoint

        // Q = round(sqrt(N / rho))
        mpf_div(Q, N, rho); mpf_sqrt(Q, Q);
        mpf_add_ui(r, Q, 0.5); mpf_floor(Q, r);

        mpf_div_ui(theta, one, mpf_get_ui(Q)); mpf_mul_ui(theta,theta,2); // 2π/Q
        mpf_sin(r, theta); mpf_mul_2exp(r, r, 1);                 // 2 sin(θ)

        // sagitta area of one segment
        mpf_mul(sagitta, theta, theta); mpf_sub_ui(sagitta,sagitta,1);
        mpf_mul(sagitta, sagitta, r); mpf_div_ui(sagitta,sagitta,24);

        // corner cap area of one triangle outside circle
        mpf_mul(cap, theta, theta); mpf_sub_ui(cap,cap,8);
        mpf_abs(cap,cap); mpf_sqrt(cap,cap);
        mpf_mul(cap, cap, theta); mpf_div_ui(cap,cap,16);

        mpf_sub(diff, cap, sagitta);
        mpf_mul(diff, diff, Q);                                   // total error

        if (mpf_cmp_abs(diff, tol) < 0) break;

        if (mpf_sgn(diff) > 0)
            mpf_set(hi, rho);
        else
            mpf_set(lo, rho);
    }

    mpf_sqrt(rho, N); mpf_div(rho, rho, Q);
    gmp_printf("Found factor ≈ %.0Ff  (other factor %.0Ff)\n",
               mpf_get_d(Q), mpf_get_d(rho));
    mpf_clears(rho,one,tol,Q,r,theta,sagitta,cap,diff,lo,hi,NULL);
}

int main(int argc, char**argv) {
    if(argc<2) { printf("Usage: %s <N>\n",argv[0]); return 1; }
    mpf_set_default_prec(64 * ((strlen(argv[1])/2)+100));  // safe precision
    mpf_t N; mpf_init(N); mpf_set_str(N, argv[1], 10);
    factor(N, mpf_get_prec(N));
    mpf_clear(N);
    return 0;
}


