// Exam Number: Y3857872
/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

    network.h:
        - Header file for the 'graphNetwork.c' source file.
        - Defines the public functions to create and control a weighted graph network from within 'main.c'.
        - Contains the 'Network' and 'Node' structures needed within all source files.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED


/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                                        NETWORK LIBRARIES/DEFINITIONS
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

// Global Libraries:
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

// Global time library and variables used to calculate function speed:
#include <time.h>

// User created libraries:
#include "list.h"
#include "stack.h"

/*
    DYNAMIC_RESIZE:
        - Enables/Disables the element of resizing structures such as
            stacks and lists once they reach capacity.
                '1' -> Enabled.
                '0' -> Disabled.
*/
#define DYNAMIC_RESIZE 1


/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                                        PUBLIC NETWORK STRUCTURES
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

/*
    Node Structure:
        - Public structure defined in 'graphNetwork.c' source file.
*/
struct Node;

/*
    Network Structure:
        - Public structure defined in 'graphNetwork.h' source file.
*/
struct Network;

/*
    Timer Structure Typedef:
        - Custom time variables to calculate time complexity across source files.
*/
typedef struct
{
    double timeStart;
    double timeEnd;
    double timeInSeconds;
} timer;


/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                                    PUBLIC NETWORK FUNCTION DECLARATIONS
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

// Creates a network.
struct Network* networkConstructor();

// Frees memory associated with a given network.
void networkDestructor(struct Network* cityNetwork);

// Adds a node/city into a network with a given city name.
int addCity(struct Network* cityNetwork, char cityName[127]);

// Adds a weighted edge/path between two cities present within a network.
int addPath(struct Network* cityNetwork, char city1Name[127], char city2Name[127], int distance);

// Removes a present node/city from a network with a given city name.
int removeCity(struct Network* cityNetwork, char cityName[127]);

// Removes a present weighted edge/path between two cities in a network.
int removePath(struct Network* cityNetwork, char city1Name[127], char city2Name[127]);

// Displays the current nodes within a network.
void displayNetwork(struct Network* cityNetwork);

// Displays a given city name's current connection list within a network.
void displayConnections(struct Network* cityNetwork, char cityName[127]);

// Returns name of given node.
char* getNodeName(struct Node* node);

// Applies Dijkstra's algorithm between two cities present within a network.
int dijkstra(FILE* dijkstraresults_file, double *timeInSeconds, struct Network* cityNetwork, char cityStartName[127], char cityEndName[127]);

#endif // NETWORK_H_INCLUDED
