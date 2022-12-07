#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>
#include <math.h>
#include <omp.h>


void count_sort(const uint64_t n, const uint64_t in[], uint64_t out[]) {
    #ifdef PARALLELIZE_1
    #pragma omp parallel for
    #endif
    for (uint64_t i = 0; i < n; i++) {
        const uint64_t x = in[i];
        uint64_t count = 0;
        #ifdef PARALLELIZE_2
        #pragma omp parallel for reduction(+: count)
        #endif
        for (uint64_t j = 0; j < n; j++) {
            if (in[j] < x || (in[j] == x && j < i)) {
                count += 1;
            }
        }
        out[count] = x;
    }
}


int main() {
    uint64_t n;
    printf("n:\n");
    scanf("%"SCNd64, &n);
    
    uint64_t* in = malloc(n * sizeof(uint64_t));
    uint64_t* out = malloc(n * sizeof(uint64_t));

    // we want to generate the worst case, so using maximum range possible (maxint - 0)
    uint64_t step = UINT64_MAX / n;

    #pragma omp parallel for
    for (uint64_t i = 0; i < n; i++) {
        in[i] = (n - i - 1) * step;
    }

    double elapsed_clock = omp_get_wtime();
    count_sort(n, in, out);
    elapsed_clock = omp_get_wtime() - elapsed_clock;

    free(in);
    free(out);

    printf("%lf", elapsed_clock);

    return 0;
}
