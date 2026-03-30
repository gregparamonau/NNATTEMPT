#include "NN.h"
#include "layer.h"
#include "matrix.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

//function to write the predicted output to a file:

void write_prediction_to_file(NN * nn, const char * testing_file, const char * write_file, const char * write_file_proper) {
    FILE * read = fopen(testing_file, "r");
    FILE * write = fopen(write_file, "w");
    FILE * write_proper = fopen(write_file_proper, "w");

    //get how many data points, inputs, outputs
    if (!read) {
        printf("FILE NOT FOUND!\n");
        return;
    }

    char line [1000];
    char * tok;
    const char * delim = " \t\n";

    fgets(line, 1000, read);

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

    for (int x = 0; x<N; x++) {
        //initialize the line reader
        fgets(line, 1000, read);
        strtok(line, delim);

        //read input vector
        for (int i = 0; i< I; i++) {
            tok = strtok(NULL, delim);
            if (!tok) {
                printf("TESTING FILE FUCKED LINE: %d. ERROR LINE 243\n", x);
                return;
            }
            in -> data [i] = atof(tok);
        }

        //read correct output vector
        for (int o = 0; o < O; o++) {
            tok = strtok(NULL, delim);
            if (!tok) {
                printf("TESTING FILE FUCKED LINE: %d. ERROR LINE 253\n", x);
                return;
            }
            out -> data[o] = atof(tok);
        }

        calculate_outputs(nn, in -> data); //calcualte outputs
        for (int y = 0; y < in -> n; y++) {
            fprintf(write, "%f, ", in -> data [y]);
            fprintf(write_proper, "%f, ", in -> data [y]);
        }
        fprintf(write, "%f\n", nn -> prob -> data[0]);
        fprintf(write_proper, "%f\n", out -> data[0]);

    }
    free_vector(in);
    free_vector(out);

    fclose(read);
    fclose(write);
}


//gcc -g /Users/gregoryparamonau/Desktop/VSCode/NNATTEMPT/nn_main.c /Users/gregoryparamonau/Desktop/VSCode/NNATTEMPT/NN.c /Users/gregoryparamonau/Desktop/VSCode/NNATTEMPT/layer.c /Users/gregoryparamonau/Desktop/VSCode/NNATTEMPT/matrix.c -o nn_main
//gcc -g /Users/gregoryparamonau/Desktop/VSCode/NNATTEMPT/nn_main.c -o nn_main
//gcc -g *.c -o nn_main
int main() {
    srand(time(NULL));
    NN * nn;

    int n_in, n_out, n_lay, hidden_lay, update, iterations;
    double alpha;
    printf("Enter # Inputs");
    scanf("%d", &n_in);

    if (n_in != -1) {
         printf("Enter # Outputs");
        scanf("%d", &n_out);

        printf("Enter # Layers [including input & output]");
        scanf("%d", &n_lay);

        printf("Enter # of Nodes / Hidden Layer");
        scanf("%d", &hidden_lay);

        printf("Enter # of Epochs");
        scanf("%d", &iterations);

        alpha = 0.0001;

        printf("Update Along Way?");
        scanf("%d", &update);
    }
    else {
        n_out = 2;
        n_lay = 4;
        hidden_lay = 4;
        alpha = 0.001;
        update = 1;
    }

    //create array
    int * layers = (int *) malloc (n_lay * sizeof(int));
    for (int x = 1; x< n_lay - 1; x++) layers[x] = hidden_lay;
    layers[0] = n_in;
    layers[n_lay - 1] = n_out;

    const char * training_file = "/Users/gregoryparamonau/Desktop/VSCode/NNATTEMPT/training.txt";
    const char * testing_file = "/Users/gregoryparamonau/Desktop/VSCode/NNATTEMPT/testing.txt";
    const char * write_file = "/Users/gregoryparamonau/Desktop/VSCode/NNATTEMPT/output.txt";
    const char * write_file_proper = "/Users/gregoryparamonau/Desktop/VSCode/NNATTEMPT/output_prop.txt";

    int epochs[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};
    int counter = 0;
    //let's test accuracies as a function of epochs (complete runs through dataset)

    create_nn(& nn, n_lay, layers, ReLU, dReLU);
    print_nn(nn);

    //test epochs up to 1000, increments of 1 for 0-10, increments of 10 for 10-100, and increments of 100 for 100-1000

    for (int x = 0; x<iterations + 1; x++) {
        if ((x == epochs [counter] || x % 10 == 0) && update) {
            counter ++;
            printf("%d, %f\n", x, test(nn, testing_file));
            //print_nn(nn);
        }
        learn(nn, training_file, alpha);
        //print_nn(nn);
    }

    //print_nn(nn);

    write_prediction_to_file(nn, testing_file, write_file, write_file_proper);

    return 0;
}