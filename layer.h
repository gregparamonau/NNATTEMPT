#ifndef LAYER_H
#define LAYER_H

#include "matrix.h"

typedef struct layer {
    struct layer * prev;
    struct layer * next;
    double (*activation)(double in); //activation function
    double (*activation_prime)(double in); //derivative of activation function
    matrix * weights;
    vector * biases;
    vector * input;
    vector * output;

    int n_inputs, n_outputs;
} layer;

void initialise_layer(layer ** new_layer, layer * prev_layer, int num_inputs, int num_outputs, double (*act)(double), double (*act_prime)(double)); //random weights & biases
void progress_layer(layer * in); //save calculated outputs into the layers output

void free_layer(layer * in); //free everything about the layer;


#endif