#ifndef NN_H
#define NN_H

#include "layer.h"
#include "matrix.h"

typedef struct NN {
    vector * input;
    vector * output;

    vector * prob;
    layer * head;
    layer * last;

    int n_inputs;
    int n_outputs;
    int n_layers;
} NN;

//create & delete
void create_nn(NN ** nn, int n_lay, int * nodes_layer, double (*act)(double), double (*act_prime)(double)); //create NN
void free_nn (NN * nn); //free everything about NN

//forward
void calculate_outputs(NN * nn, double * in); //calculates outputs, and saves them to nn -> outputs
void softmax(NN * nn); //takes nn -> outputs, and puts them through softmax. saves them to same spot

//learn
void learn(NN * nn, const char * training_file, double alpha); //learn a set of data
void back_prop(NN * nn, layer * last, vector * onehot, double alpha); //backpropagate, w alpha = learn rate

//test
double test(NN * nn, const char * test_file); //test & return accuracy (0 : 1)

void print_output(NN * nn);// print output probabilities
void print_nn(NN * nn);

//host of activation functions;
double ReLU(double in); //ReLU function
double dReLU(double in); //derivative of ReLU

double Lin(double in);
double dLin(double in);

double SoftPlus(double in);
double dSoftPlus(double in);

double t(double in);
double dt(double in);


#endif