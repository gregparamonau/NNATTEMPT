#ifndef MATRIX_H
#define MATRIX_H

typedef struct matrix{
    int m, n;
    double * data;
} matrix;

typedef struct vector{
    int n;
    double * data;
} vector;

matrix * create_matrix_from_data(int m, int n, double * data);
matrix * create_matrix_random(int m, int n, double range);

vector * create_vector_from_data(int n, double * data);
vector * create_vector_random(int n, double range);

void set_vector(vector * in, double * data, int n);

void mult_vec(vector * output, matrix * A, vector * a);
void mult_mat(matrix * output, matrix * A, matrix * B);

void add_vec(vector * output, vector * a, vector * b);
void add_mat(matrix * output, matrix * A, matrix * B);

void apply_act(vector * output, vector * a, double (*act)(double));
int argmax(vector * in);

void print_mat(matrix * in);
void print_vec(vector * in);

void free_matrix(matrix * in);
void free_vector(vector * in);


#endif