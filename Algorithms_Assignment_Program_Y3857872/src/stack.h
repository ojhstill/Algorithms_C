// Exam Number: Y3857872
/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

    stack.h:
        - Header file for the 'priorityStack.c' source file.
        - Defines the public functions to create and control a priority stack
            from within the main and other source files.
        - Contains the 'Stack' structure needed within the network source file.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED


/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                                        STACK LIBRARIES/DEFINITIONS
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

// Global Libraries:
#include <math.h>

// User Created Network Library:
#include "network.h"

/*
    STACK_CAPACITY:
        - Maximum integer capacity for all stacks (maximum number of connections a node can have).
        - If 'DYNAMIC_RESIZE' is enabled the capacity will double when the limit is reached.
*/
#define STACK_CAPACITY 32


/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                                        PUBLIC STACK STRUCTURES
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

/*
    Node Structure:
        - Public structure defined in 'graphNetwork.c' source file.
*/
struct Node;

/*
    Item Structure:
        - Public structure defined in 'priorityStack.c' source file.
*/
struct Item;

/*
    Stack Structure:
        - Public structure defined in 'priorityStack.c' source file.
*/
struct Stack;


/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                                    PUBLIC STACK FUNCTION DECLARATIONS
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

// Create a priority stack.
struct Stack* stackConstructor();

// Free memory associated with a given priority stack.
void stackDestructor(struct Stack* stack);

// Indicates if a given stack is empty.
int stackIsEmpty(struct Stack* stack);

// Adds an item into a stack with given relative distances.
int stackPush(struct Stack* stack, struct Node* city, int shortestDistance, int edgeDistance);

// Removes an item from the top of the stack.
struct Node* stackPop(struct Stack* stack, int* edgeDistance);

// Searches a stack for a given city name and returns its index if found.
int stackSearch(struct Stack* stack, char cityName[127]);

// Bubble sorts the priority stack into descending order.
void stackBubbleSort(struct Stack* stack);

// Heap sorts the priority stack into descending order.
void stackHeapSort(struct Stack* stack);

// Indicates if a given stack is sorted.
int stackIsSorted(struct Stack* stack);

#endif // STACK_H_INCLUDED
