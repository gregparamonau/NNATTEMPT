#include "layer.h"
#include <stdlib.h>
#include <math.h>

void initialise_layer(layer ** new_layer, layer * prev_layer, int num_inputs, int num_outputs, double (*act)(double), double (*act_prime)(double)) {
    * new_layer = (layer *) malloc (sizeof(layer));
    (* new_layer) -> n_inputs = num_inputs;
    (* new_layer) -> n_outputs = num_outputs;
    (* new_layer) -> activation = act;
    (* new_layer) -> activation_prime = act_prime;
    (* new_layer) -> prev = prev_layer;

    (* new_layer) -> input = create_vector_random(num_inputs, 0);
    (* new_layer) -> output = create_vector_random(num_outputs, 0);
    (* new_layer) -> next = NULL;

    if (num_outputs != -1) {
        (* new_layer) -> weights = create_matrix_random(num_outputs, num_inputs, sqrt(1.0 / num_inputs));
        (* new_layer) -> biases = create_vector_random(num_outputs, sqrt(1.0 / num_inputs));
    }

    //if (prev_layer) prev_layer -> next = *new_layer;
    // O = W(I) + B
}
void progress_layer(layer * in) {
    //O = WI + B

    mult_vec(in -> output, in -> weights, in -> input); //WI
    add_vec(in -> output, in -> output, in -> biases); //B

    if (!in -> next) return;

    //apply activation, and store result in next 
    apply_act(in -> next -> input, in -> output, in -> activation);
}

void free_layer(layer * in) {
    free_matrix(in -> weights);
    free_vector(in -> biases);
    free_vector(in -> input);
    free_vector(in -> output);

    free(in);
}