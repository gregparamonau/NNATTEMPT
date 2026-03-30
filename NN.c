#include "NN.h"
#include "layer.h"
#include "matrix.h"
#include "math.h"

#include <stdlib.h>
#include "math.h"
#include <stdio.h>
#include <string.h>

//create & delete
void create_nn(NN ** nn, int n_lay, int * nodes_layer, double (*act)(double), double (*act_prime)(double)) {
    (* nn) = (NN *) malloc (sizeof(NN));

    //params
    (* nn) -> n_inputs = nodes_layer[0];
    (* nn) -> n_layers = n_lay;
    (* nn) -> n_outputs = nodes_layer[n_lay - 1];

    initialise_layer(&((* nn) -> head), NULL, nodes_layer[0], nodes_layer[1], act, act_prime);
    layer * layer_ptr = (*nn) -> head;

    (* nn) -> input = (* nn) -> head -> input;

    //hidden layers
    for (int x = 1; x<n_lay - 2; x++) {
        initialise_layer(&(layer_ptr -> next), layer_ptr, nodes_layer[x], nodes_layer[x + 1], act, act_prime);
        layer_ptr = layer_ptr -> next;
    }
    initialise_layer(&(layer_ptr -> next), layer_ptr, nodes_layer[n_lay - 2], nodes_layer[n_lay - 1], Lin, dLin);
    layer_ptr = layer_ptr -> next;

    (* nn) -> last = layer_ptr;
    (* nn) -> output = layer_ptr -> output;
    (* nn) -> prob = create_vector_random((* nn) -> n_outputs, 0); //initialize to 0

}
void free_nn (NN * nn) {
    layer * lay = nn -> head;
    for (int x = 0; x<nn -> n_layers; x++) {
        layer * temp = lay;
        lay = lay -> next;

        free_layer(temp);
    }
    free(nn -> prob);

    free(nn);
}

//forward
void calculate_outputs(NN * nn, double * in) {
    set_vector(nn -> input, in, nn -> n_inputs);

    layer * ptr = nn -> head;
    //loop over hidden layers
    while (ptr) {
        progress_layer(ptr);
        ptr = ptr -> next;
    }

    //final layer needs to be handled separately
    //ie generate prob vector for nn using this 
    softmax(nn);

}

void softmax(NN * nn) {
    double max = nn -> output -> data[0];
    for (int i = 1; i < nn -> output -> n; i++)
        if (nn -> output -> data[i] > max) max = nn -> output -> data[i];

    double sum = 0;
    for (int i = 0; i < nn -> output -> n; i++) {
        nn -> prob -> data[i] = exp(nn -> output -> data[i] - max);
        sum += nn -> prob -> data[i];
    }

    for (int i = 0; i < nn -> output -> n; i++) {
        nn -> prob -> data[i] /= sum;
    }
}

//learn
void learn(NN * nn, const char * training_file, double alpha) {
    //file structured in the following way
    //first line: [N: number of data points], [I: number of inputs], [O: number of outputs] //, [T: fraction of points reserved for training]
    //every following line: [Random character], [first I numbers: data point inputs], [next O numbers: correct data point outputs]

    FILE * fp = fopen(training_file, "r");
    if (!fp) {
        printf("FILE NOT FOUND!\n");
        return;
    }

    char line [1000];
    char * tok;
    const char * delim = " \t\n";

    fgets(line, 1000, fp);

    //char * token = strtok(line, delim);
    int N, I, O;
    tok = strtok(line, delim);
    if (tok) sscanf(tok, "%d", &N); else printf("TOKEN NOT FOUND 101\n"); //find number of data points in dataset

    tok = strtok(NULL, delim);
    if (tok) sscanf(tok, "%d", &I); else printf("TOKEN NOT FOUND 104\n");

    tok = strtok(NULL, delim);
    if (tok) sscanf(tok, "%d", &O); else printf("TOKEN NOT FOUND 107\n");

    vector * in = create_vector_random(I, 0);
    vector * out = create_vector_random(O, 0);

    for (int x = 0; x<N; x++) {
        //initialize the line reader
        fgets(line, 1000, fp);
        strtok(line, delim);

        //read input vector
        for (int i = 0; i< I; i++) {
            tok = strtok(NULL, delim);
            if (!tok) {
                printf("TRAINING FILE FUCKED UP: 121\n");
                return;
            }
            in -> data [i] = atof(tok);
        }

        //read correct output vector
        for (int o = 0; o < O; o++) {
            tok = strtok(NULL, delim);
            if (!tok) {
                printf("TRAINING FILE FUCKED UP: 131\n");
                return;
            }
            out -> data [o] = atof(tok);
        }

        calculate_outputs(nn, in -> data); //calcualte outputs
        back_prop(nn, nn -> last, out, alpha); //backprpagate and update the weights & biases
    }

    free_vector(in);
    free_vector(out);

}

void back_prop(NN * nn, layer * last, vector * onehot, double alpha) {
    //process:
    //preliminary, final layer is slightly different
        //* compute L_j = dL/dof_j (final layer outputs) = pj - yj (softmax - one-hot)
        //* 
    //* compute next layer update vector
        //* L_j = sum{k}{dL/dfk (saved previous layer input update [k]) * dfk/dok (relu prime [ok current layer output [k]]) * dok/dij (W_kj)}
    //* compute & update weights matrix W = W - alpha DW
        //* DW_k row common multiple: dL/dfk (saved previous layer input update [k]) * dfk/dok (relu prime [ok (current layer output[k])])
        //* DW_kj = DW_k * ij [input of current layer [j]]
        //* DW = [dL/dok] * [i]^T
    //* compute & update biases vector B = B - alpha DB
        //* B_k = dl/dfk (saved previous later input update [k]) * dfk/dok (relu prime [ok (current layer output [k])]) * (dok / dB_k [1])
    
    //step 1: final layer dL/dof_j
    vector * Lj = create_vector_random(nn -> n_outputs, 0);
    for (int x = 0; x < Lj -> n; x++) Lj -> data[x] = nn -> prob -> data[x] - onehot -> data[x];

    layer * ptr = last;

    //step 2: while loop (until prev == NULL)
    do {

        //calculate weights biases update vectors
        vector * newLj = create_vector_random(ptr -> n_inputs, 0);
        matrix * DW = create_matrix_random(ptr -> weights -> m, ptr -> weights -> n, 0);
        vector * DB = create_vector_random(ptr -> n_outputs, 0);

        for (int k = 0; k < ptr -> weights -> m; k++) {
            double dldok = Lj -> data[k] * ptr -> activation_prime(ptr -> output -> data[k]);
            DB -> data[k] += - alpha * dldok;
            for (int j = 0; j < ptr -> weights -> n; j++) {
                newLj -> data[j] += dldok * ptr -> weights -> data[k * ptr -> weights -> n + j];
                DW -> data [k * DW -> n + j] = - alpha * dldok * ptr -> input -> data[j];
            }
        }

        //update weights & biases
        add_mat(ptr -> weights, ptr -> weights, DW);
        add_vec(ptr -> biases, ptr -> biases, DB);

        free_matrix(DW);
        free_vector(DB);

        free_vector(Lj);
        Lj = newLj;

        ptr = ptr -> prev;
        //calculate biases vector
    } while (ptr);

    free_vector(Lj);
}

double test(NN * nn, const char * test_file) {
    //file formatted in the same way that the training file is formatted
    FILE * fp = fopen(test_file, "r");
    if (!fp) {
        printf("FILE NOT FOUND!\n");
        return -1;
    }

    char line [1000];
    char * tok;
    const char * delim = " \t\n";

    fgets(line, 1000, fp);

    //char * token = strtok(line, delim);
    int N, I, O;
    
    tok = strtok(line, delim);
    if (tok) sscanf(tok, "%d", &N); else printf("TOKEN NOT FOUND 221\n");

    tok = strtok(NULL, delim);
    if (tok) sscanf(tok, "%d", &I); else printf("TOKEN NOT FOUND 224\n");

    tok = strtok(NULL, delim);
    if (tok) sscanf(tok, "%d", &O); else printf("TOKEN NOT FOUND 227\n");
    
    vector * in = create_vector_random(I, 0);
    vector * out = create_vector_random(O, 0);

    int sum = 0;

    for (int x = 0; x<N; x++) {
        //initialize the line reader
        fgets(line, 1000, fp);
        strtok(line, delim);

        //read input vector
        for (int i = 0; i< I; i++) {
            tok = strtok(NULL, delim);
            if (!tok) {
                printf("TESTING FILE FUCKED LINE: %d. ERROR LINE 243\n", x);
                return -1;
            }
            in -> data [i] = atof(tok);
        }

        //read correct output vector
        for (int o = 0; o < O; o++) {
            tok = strtok(NULL, delim);
            if (!tok) {
                printf("TESTING FILE FUCKED LINE: %d. ERROR LINE 253\n", x);
                return -1;
            }
            out -> data[o] = atof(tok);
        }

        calculate_outputs(nn, in -> data); //calcualte outputs
        sum += (argmax(out) == argmax(nn -> prob));
    }
    free_vector(in);
    free_vector(out);

    return (double) sum / N;
}

void print_output(NN * nn) {
    for (int x = 0; x< nn -> n_outputs; x++) {
        printf("output %d: %f\n", x, nn -> prob -> data[x]);
    }
}
void print_nn(NN * nn) {
    layer * ptr = nn -> head;

    while (ptr) {
        print_mat(ptr -> weights);
        printf("\n");
        print_vec(ptr -> biases);
        printf("\n\n");
        ptr = ptr -> next;
    }
}

//host of activation functions;
//leaky ReLU
double ReLU(double in) {
    return tanh(in);/*
    if (in >= 0) return in;
    return 0;*/
}

double dReLU(double in) {
    double a = tanh(in);
    return 1 - a * a;
   /*if (in >= 0) return 1;
    return 0;*/
}

double Lin(double in) {
    return in;
}
double dLin(double in) {
    return 1;
}

double SoftPlus(double in) {
    return log(1 + exp(in));
}
double dSoftPlus(double in) {
    return 1.0 / (1 + exp(-in));
}

double t(double in) {
    return exp(in) - 1;
}
double dt(double in) {
    return exp(in);
}
