#include <stdio.h>
#include <stdlib.h>

#define M 30
#define N 40

double r() {
    return rand() * 1.0 / RAND_MAX;
}


void fill(char g[][N], double p ) {
    int j, k;

    for (j = 0; j < M; j++) {
        for (k = 0; k < N; k++) {
            if (r() < p) {
                g[j][k] = 'X'; // on
            } else {
                g[j][k] = ' ';
            }
        }
    }
}

void show(char g[][N] );

void main() {
    char grid[M][N];
    fill(grid, 0.60);
    show(grid);
    printf("Done\n");
}


void show(char g[][N]) {
    int j, k;
    for (j = 0; j < M; j++) {
        for (k = 0; k < N; k++) {
            printf("%c", g[j][k]);
        }
        printf("\n");
    }

}
