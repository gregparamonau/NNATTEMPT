#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>

matrix * create_matrix_from_data(int m, int n, double * data) {
    matrix * out = (matrix *) malloc (sizeof(matrix));
    out -> m = m;
    out -> n = n;
    out -> data = (double *) malloc (n * m * sizeof(double));

    for (int x = 0; x<n * m; x++) out -> data[x] = data[x];

    return out;
}
matrix * create_matrix_random(int m, int n, double range) {
    matrix * out = (matrix *) malloc (sizeof(matrix));
    out -> m = m;
    out -> n = n;
    out -> data = (double *) malloc (m * n * sizeof(double));

    for (int x = 0; x<m * n; x++) out->data[x] = ((double)rand() / RAND_MAX * 2.0 - 1.0) * range;//(double)(rand() % (2 * range * 10 + 1) - (range * 10)) / 10.0;

    return out;
}

vector * create_vector_from_data(int n, double * data) {
    vector * out = (vector *) malloc (sizeof(vector));
    out -> n = n;
    out -> data = (double * ) malloc (n * sizeof(double));

    for (int x = 0; x<n; x++) out -> data[x] = data[x];
    
    return out;
}
vector * create_vector_random(int n, double range) {
    vector * out = (vector *) malloc (sizeof(vector));
    out -> n = n;
    out -> data = (double *) malloc (n * sizeof(double));

    for (int x = 0; x<n; x++) out -> data[x] = ((double)rand() / RAND_MAX * 2.0 - 1.0) * range;//(double)(rand() % (2 * range * 10 + 1) - (range * 10)) / 10.0;

    return out;
}

void set_vector(vector * in, double * data, int n) {
    if (n != in -> n) {
        printf("DIMENSIONS OF in: %d, AND data: %d DON'T MATCH: ", in -> n, n);
        return;
    }

    for (int x = 0; x<n; x++) in -> data[x] = data[x];
}

void mult_vec(vector * output, matrix * A, vector * a) {
    if (a -> n != A -> n || output -> n != A -> m) {
        printf("DIMENSIONS OF A: %d x %d, a: %d, AND output: %d DON'T MATCH", A -> m, A -> n, a-> n, output -> n);
        return;
    }

    for (int x = 0; x< A -> m; x++) {
        output -> data[x] = 0;
        for (int i = 0; i < a -> n; i++) {
            output -> data[x] += A -> data[A -> n * x + i] * a -> data[i];
        }
    }

}
void mult_mat(matrix * output, matrix * A, matrix * B) {
    if (A -> n != B -> m || output -> m != A -> m || output -> n != B -> n) {
        printf("DIMENSIONS OF A: %d x %d, B: %d x %d, AND OUTPUT: %d x %d DON'T MATCH", A -> m, A -> n, B -> m, B -> n, output -> m, output -> n);
        return;
    }

    //out [i][j] = sum {A[i][k] * B[k][j]}
    //loop over i, j, k
    for (int i = 0; i< A -> m; i++) {
        for (int j = 0; j < B -> n; j++) {
            output -> data[i * output -> n + j] = 0;
            for (int k = 0; k < A -> n; k++) {
                output -> data[i * output -> n + j] += A -> data[i * A -> n + k] * B -> data[k * B -> n + j];
            }
        }
    }
}

void add_vec(vector * output, vector * a, vector * b) {
    if (a -> n != b -> n || output -> n != a -> n) {
        printf("DIMENSIONS OF a: %d, b: %d, AND output: %d DON'T MATCH", a -> n, b -> n, output -> n);
        return;
    }

    for (int x = 0; x<output -> n; x++) output -> data[x] = a -> data[x] + b -> data[x];

}
void add_mat(matrix * output, matrix * A, matrix * B) {
    if (A -> n != B -> n || A -> m != B -> m || output -> m != A -> m || output -> n != A -> n) {
        printf("DIMENSIONS OF A: %d x %d, B: %d x %d, AND OUTPUT: %d x %d DON'T MATCH", A -> m, A -> n, B -> m, B -> n, output -> m, output -> n);
        return;
    }

    for (int x = 0; x<output -> m * output -> n; x++) output -> data[x] = A -> data[x] + B -> data[x];
}

void apply_act(vector * output, vector * a, double (*act)(double)) {
    if (output -> n != a -> n) {
        printf("DIMENSIONS OF a: %d, AND output: %d DON'T MATCH", a -> n, output -> n);
        return;
    }

    for (int x = 0; x<output -> n; x++) output -> data[x] = act(a -> data[x]);
}
int argmax(vector * in) {
    int ind = 0;
    double max = in -> data[0];
    for (int x = 1; x<in -> n; x++) {
        if (in -> data[x] > max) {
            max = in -> data[x];
            ind = x;
        }
    }
    return ind;
}


void print_mat(matrix * in) {
    for (int x = 0; x<in -> m; x++) {
        for (int y = 0; y < in -> n; y++) {
            printf("%f ", in -> data[x * in -> n + y]);
        }
        printf("\n");
    }
}
void print_vec(vector * in) {
    for (int x = 0; x<in -> n; x++) printf("%f ", in -> data[x]);
    printf("\n");
}

void free_matrix(matrix * in) {
    free(in -> data);
    free(in);
}
void free_vector(vector * in) {
    free (in -> data);
    free(in);
}