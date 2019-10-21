// Exam Number: Y3857872
/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

arrayList.c:
        - Source file defining the functions to create and and control array lists.
        - Implemented within 'graphNetwork.c' for storing connections between nodes.
        - (Code referenced and adapted from Andrew Turner - Reference [6])

        - Functions:
            - listConstructor
            - listDestructor
            - listAdd
            - listRemove
            - listSearch
            - getListEntity
            - listDisplay
            - listResize
            - listSize
            - listIsEmpty

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

// Include respective header file:
#include "list.h"


/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                                            LIST STRUCTURES
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

/*
    cityConnection Structure:
        - Internal custom entity structure.
        - Used to store the relative city connection information as individual entities within a list.

        city        - Pointer to a node in a given network.
        distance    - Integer distance between given node and the node within cityConnection structure.
*/
struct cityConnection
{
    struct Node* city;
    int distance;
};

/*
    List Structure:
        capacity        - Maximum length of the list.
        numEntities     - Current number of entities within a give list.
        entities        - Array of individual pointer entities, each of the cityConnection structure.
*/
struct List
{
    int capacity;
    int numEntities;
    struct cityConnection* entities;
};


/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                                        LIST CONSTRUCTORS/DESTRUCTORS
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

/*
    List Constructor:
        - Allocate memory and create a new list.
        - Returns pointer to new list.
*/
struct List* listConstructor()
{
    struct List* arrayList;

    // Reserve memory for the list and for the elements we want to store.
    arrayList = malloc(sizeof(struct List));
    arrayList->entities = malloc(LIST_CAPACITY * sizeof(struct cityConnection));

    // Initialise list values.
    arrayList->capacity = LIST_CAPACITY;
    arrayList->numEntities = 0;

    // Return new list.
    return arrayList;
}

/*
    List Destructor:
        - Free memory associated with a list.
*/
void listDestructor(struct List* list)
{
    free(list->entities);
    free(list);
}


/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                                            LIST SIZE FUNCTIONS
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

/*
    List Size:
        - Returns the integer size of a given list.
        - Used within nodeDegree() function from 'network.h' library.
*/
int listSize(struct List* list)
{
    return list->numEntities;
}

/*
    List Is Empty:
        - Checks if a given list is empty.
        - Uses internal listSize() function.
            '1' -> List is empty.
            '0' -> List contains at least one entity.
*/
int listIsEmpty(struct List* list)
{
    if (listSize(list) == 0)
        return 1;

    else return 0;
}

/*
    List Resize:
        - Function to increase the maximum capacity of a given list once the maximum has been reached.
        - Capacity is stack ever increased by a factor of two.
        - Used within listAdd() if DYNAMIC_RESIZE from 'network.h' is enabled.
        - Memory is reallocated.
*/
void listResize(struct List* list, int newSize)
{
    // The list is already big enough to store the number of elements requested, do nothing.
    if (newSize <= list->capacity)
        return;

    // Reallocate the entities array.
    list->entities = realloc(list->entities, newSize * sizeof(struct cityConnection));

    // Update list capacity.
    list->capacity = newSize;
}


/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                                            LIST SEARCH FUNCTIONS
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

/*
    List Search:
        - Searches for a node within a list using a given city name.
        - Returns integer value.
            'index' -> City name found, index within list returned.
            '-1'    -> Node not present in list.
*/
int listSearch(struct List* list, char cityName[127])
{
    int index;

    // Check each occupied element in the list in turn.
    for (index = 0; index < list->numEntities; index++)
    {
        struct Node* currentCity = list->entities[index].city;

        // If the current element matches the requested one return its index.
        if (strcmp(getNodeName(currentCity), cityName) == 0)
            return index;
    }

    // Entity not found, return '-1'.
    return -1;
}

/*
    Get List Entity:
        - Returns pointer to a node and its path distance a given list and index.
        - Function returns path distance to given address argument.
            'Node*' -> Node pointer within list at given index.
            'NULL'  -> Invalid index within list.
*/
struct Node* getListEntity(struct List* list, int index, int* pathDistance)
{
    // If the requested index is invalid then return NULL to indicate failure.
    if (index < 0 || index >= list->numEntities)
        return NULL;

    // Set distance to the requested argument address.
    *pathDistance = list->entities[index].distance;

    // Return node at given index.
    return list->entities[index].city;
}


/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                                        LIST ADD/REMOVE FUNCTIONS
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

/*
    List Add:
        - Adds a new entity to a list using a given node and distance.
        - If DYNAMIC_RESIZE from 'network.h' is enabled, list capacity will double once limit has been reached.
        - Returns indication if function was successful.
            '1' -> Entity added to list.
            '0' -> List is at capacity and DYNAMIC_RESIZE is disabled.
*/
int listAdd(struct List* list, struct Node* city, int distance)
{
    // Return '0' if list is full.
    if (list->numEntities >= list->capacity)
    {
        if (DYNAMIC_RESIZE == 0)
        {
            // List at capacity, return '0' and print failure.
            printf("Failure: '%s' connection list at capacity! - (listAdd - arrayList.c)\n", getNodeName(city));
            printf("\t^ (Enable DYNAMIC_RESIZE or increase ARRAY_LIST_CAPACITY in 'list.h')");
            return 0;
        }

            // Call list resize function to increase memory for more elements.
        else listResize(list, list->capacity * 2);
    }

    // Write in connected city name and distance in to next available slot in list.
    list->entities[list->numEntities].city = city;
    list->entities[list->numEntities].distance = distance;

    // Update entity count.
    list->numEntities++;

    // Indicate success.
    return 1;
}

/*
    List Remove:
        - Unties city node at a given index from the network.
        - Frees node from memory.
        - Returns indication if function was successful.
            '1' -> City found and removed from network.
            '0' -> City not present in network.
*/
int listRemove(struct List* list, int index)
{
    // If the index is invalid then do nothing.
    if (index < 0 || index >= list->numEntities)
        return 0;

    // Remove entity at given index and shift all entities in front backwards to keep array structure.
    int elementCount = list->numEntities - index;
    memmove(&list->entities[index], &list->entities[index+1], elementCount * sizeof(struct cityConnection));

    // Update number of elements in list.
    list->numEntities--;

    // Indicate success.
    return 1;
}


/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                                        LIST DISPLAY FUNCTIONS
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

/*
    List Display:
        - Prints all nodes in a given node's connection list as an array.
        - Additionally indicates if connection list is empty.
*/
void listDisplay(struct List* list)
{
    // Print if network is empty and immediately return.
    if (list->numEntities == 0)
    {
        printf("\tConnection List empty!\n");
        return;
    }

    // Start array.
    printf("\t[");

    // Scan through all indexes within the list.
    int index;
    for (index = 0; index < list->numEntities; index++)
    {
        if (index > 0)
            printf(",");

        struct Node* city = list->entities[index].city;

        // Print the current city.
        printf(" %s (%dkm)", getNodeName(city), list->entities[index].distance);
    }

    // Close array.
    printf(" ]\n");
}
