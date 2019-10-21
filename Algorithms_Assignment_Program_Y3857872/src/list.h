// Exam Number: Y3857872
/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

    list.h:
        - Header file for the 'arrayList.c' source file.
        - Defines the public functions to create and control an array list
            from within the main and other source files.
        - Contains the 'List' structure needed within the network source file.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED


/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                                        LIST LIBRARIES/DEFINITIONS
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

// User Created Network Library:
#include "network.h"

/*
    ARRAY_LIST_CAPACITY:
        - Maximum integer capacity for all lists (maximum number of connections a node can have).
        - If 'DYNAMIC_RESIZE' is enabled the capacity will double when the limit is reached.
*/
#define LIST_CAPACITY 16


/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                                        PUBLIC LIST STRUCTURES
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

/*
    Node Structure:
        - Public structure defined in 'graphNetwork.c' source file.
*/
struct Node;

/*
    List Structure:
        - Public structure defined in 'arrayList.c' source file.
*/
struct List;


/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                                    PUBLIC LIST FUNCTION DECLARATIONS
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

// Creates a list.
struct List* listConstructor();

// Frees memory associated with a given list.
void listDestructor(struct List* list);

// Returns the size of a given list.
int listSize(struct List* list);

// Adds a vertex/node into a list with a given relative distance.
int listAdd(struct List* list, struct Node* city, int distance);

// Removes an present vertex/node from a list with a given index.
int listRemove(struct List* list, int index);

// Searches a list for a given city name and returns its index if found.
int listSearch(struct List* list, char cityName[127]);

// Returns pointer to a node and distance a given list and index.
struct Node* getListEntity(struct List* list, int index, int* pathDistance);

// Displays the current nodes and respective distances within a list.
void listDisplay(struct List* list);

#endif // LIST_H_INCLUDED
