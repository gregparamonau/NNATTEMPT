Short project that I worked on over the weekend. 
This is a neural network program for which parameters can be specified, and for which any dataset can be learned
by providing it with the correctly structured text file of both training and test data. functionality has been tested 
primarily on 2D datasets, and classifying whether a point is within a shape or not. This has been tested on shapes
of hearts, 67, and my name Greg. The final model was trained on a dataset of 140000 points, and tested on a dataset of
34000 points. It had 2 inputs (x and y), and 2 outputs (in or out), with 14 hidden layers of 8 neurons each. This was
ran through the dataset 750 times (750 epochs), and achieved a final accuracy of ~97.9% on whether a point was in / out
the boundary of my name.

this was primarily written in C since I am currently taking a C programming course and felt like I needed the practice.
This comes with the advantage that C is hundreds of times faster than python, and thus the whole training process took
~20 minutes. the dataset was generated in python though, since handling files in C is a hassle, and I felt like
efficiency wasn't really a necessity here.

Overal very cool. might look into training on more complex datasets like MNIST or smth idk.
