// Exam Number: Y3857872
/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

    graphNetwork.c:
        - Source file defining the functions to create and and control a network graph.
        - Includes Dijkstra's algorithm to find the shortest path between two cities within the network.
        - Network representation based on a doubly-linked list structure.
        - (Code referenced and adapted from Andrew Turner - Reference [5])

        - Functions:
            - nodeConstructor
            - nodeDestructor
            - networkConstructor
            - networkDestructor
            - nodeDegree
            - networkSize
            - networkIsEmpty
            - cityNameSearch
            - getNode
            - addCity
            - addEdge
            - removeCity
            - removeEdge
            - displayNetwork
            - displayConnections
            - dijkstra

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

// Include respective header file:
#include "network.h"


/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                                            NETWORK STRUCTURES
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

/*
    Node Structure:
        name                - Node name.
        connections         - Pointer to a list of weighted connected nodes.
        next                - Pointer to the next node in the network.
		prev                - Pointer to the previous node in the network.

        (For Dijkstra's algorithm)
        shortestDistance    - Relative integer distance travelled from a starting node.
        routeEdgeDistance   - Edge integer distance for node via the shortest path.
        isVisited           - Indication if node has been visited.
		back                - Pointer to the previous node via the shortest path to back propagate.
*/
struct Node
{
    char name[127];
    struct List* connections;

    // Neighbouring city nodes.
    struct Node* next;
    struct Node* prev;

    // For Dijkstra's algorithm.
    int shortestDistance;
    int routeEdgeDistance;
    int isVisited;
    struct Node* back;
};

/*
    Network Structure:
        size    - Number of nodes within network.
        head    - Pointer to the first node in the network.
		prev    - Pointer to the last node in the network.
*/
struct Network
{
    int size;

    struct Node* head;
    struct Node* tail;
};


/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                                        NETWORK CONSTRUCTORS/DESTRUCTORS
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

/*
    Node Constructor:
        - Allocate memory and create a new node with a provided city name.
        - Returns pointer to new node.
*/
struct Node* nodeConstructor(char cityName[127])
{
    struct Node* newCity;

    // Reserve memory for node.
    newCity = malloc(sizeof(struct Node));
    newCity->connections = listConstructor();

    // Write in city name.
    strcpy(newCity->name, cityName);

    // Initialise Dijkstra parameters.
    newCity->isVisited = 0;
    newCity->shortestDistance = INT_MAX;
    newCity->routeEdgeDistance = 0;
    newCity->back = NULL;

    // Initialise neighbouring nodes.
    newCity->next = NULL;
    newCity->prev = NULL;

    // Return new node.
    return newCity;
}

/*
    Node Destructor:
        - Free memory associated with a node.
*/
void nodeDestructor(struct Node* node)
{
    listDestructor(node->connections);
    free(node);
}

/*
    Network Constructor:
        - Allocate memory for a new network.
        - Returns pointer to new network.
*/
struct Network* networkConstructor(void)
{
    // Print network creation.
    printf("\n- Network Created! -\n");

    // Reserve memory for the network.
    struct Network* newNetwork = malloc(sizeof(struct Network));

    // Initialise network values.
    newNetwork->size = 0;
    newNetwork->head = NULL;
    newNetwork->tail = NULL;

    // Return new network.
    return newNetwork;
}

/*
    Network Destructor:
        - Free memory associated with a network.
        - Removes all nodes in network in the process.
*/
void networkDestructor(struct Network* cityNetwork)
{
    // Traverse entire network starting at the network head.
    struct Node* currentNode;
    struct Node* nextNode = cityNetwork->head;

    while (nextNode != NULL)
    {
        // Move along to next city node in network.
        currentNode = nextNode;
        nextNode = currentNode->next;

        // Destruct city node.
        nodeDestructor(currentNode);
    }

    //Set network size to '0' and address to 'NULL'.
    cityNetwork->size = 0;
    cityNetwork = NULL;

    // Free entire network after all nodes have been removed.
    free(cityNetwork);


    // Print network removal.
    printf("\n- Network Removed! -\n");
}


/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                                            NETWORK SIZE FUNCTIONS
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

/*
    Node Degree:
        - Uses listSize() function from 'list.h' library.
        - Returns the integer size of a given nodes connection list,
            or a given node's degree.
*/
int nodeDegree(struct Node* cityNode)
{
    return listSize(cityNode->connections);
}

/*
    Network Size:
        - Returns the size of a given network (number of nodes).
*/
int networkSize(struct Network* cityNetwork)
{
    return cityNetwork->size;
}

/*
    Network Is Empty:
        - Checks if a given network is empty.
            '1' -> Network is empty.
            '0' -> Network contains at least one node.
*/
int networkIsEmpty(struct Network* cityNetwork)
{
    if (cityNetwork->size == 0)
        return 1;

    else return 0;
}


/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                                            NETWORK SEARCH FUNCTIONS
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

/*
    City Name Search:
        - Searches for a node within a network using a given city name.
        - Used to prevent duplicate cities being added to the network.
        - Returns integer value.
            'index' -> City name found, index within network returned.
            '-1'    -> Node not present in network.
*/
int cityNameSearch(struct Network* cityNetwork, char citySearch[127])
{
    // Start at the first node in the network.
    struct Node* currentCity = cityNetwork->head;

    int index = 0;

    // Don't search if network is empty (return '-1').
    while (cityNetwork->size > 0)
    {
        // Check its name against cities already added and return its index if found.
        if (strcmp(currentCity->name, citySearch) == 0)
            return index;

        // If we reach the end of the list, break and return failure '-1'.
        if (currentCity->next == NULL)
            break;

        // If current city name does not match, increase index to move to the next node.
        currentCity = currentCity->next;
        index++;
    }

    return -1;
}

/*
    Get Node:
        - Returns pointer to a node a given network and index.
        - Flags an error if index given is invalid.
            'Node*' -> Node pointer within network at given index.
            'NULL'  -> Invalid index within network.
*/
struct Node* getNode(struct Network* cityNetwork, int cityIndex)
{
    // Print failure if it is invalid - return 'NULL'.
    if (cityIndex < 0 || cityIndex >= cityNetwork->size)
    {
        printf("Failure: %d = Invalid index! - (getNode - graphNetwork.c)\n", cityIndex);
        return NULL;
    }

    // Start at the head and move forwards.
    struct Node* currentNode = cityNetwork->head;

    // Move through index items.
    int index;
    for (index = 0; index < cityIndex; index++)
    {
        // Make sure our links are not corrupt ...
        if (currentNode->next == NULL)
            return NULL;

        // ... otherwise move on
        currentNode = currentNode->next;
    }

    // Return node at given index.
    return currentNode;
}


/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                                        NETWORK ADD/REMOVE FUNCTIONS
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

/*
    Add City:
        - Adds a new vertex/node to a network using the 'Node' structure and a given city name.
        - Additionally checks to see if city has already been added using previous citySearch() function.
        - Returns indication if function was successful.
            '1' -> City added to network
            '0' -> City already present in network and rejected.
*/
int addCity(struct Network* cityNetwork, char cityName[127])
{
    // Check if city has already been added, returns '0' if present.
    if (cityNameSearch(cityNetwork, cityName) >= 0)
    {
        // *** Uncomment 'printf' line below if you would like to display rejected duplicate cities ***

        //printf("Failure: %s already present within network. - (addCity - graphNetwork.c)\n", cityName);
        return 0;
    }

    // City wasn't found in the network, continue code.

    // Construct a new city node.
    struct Node* newCity = nodeConstructor(cityName);

    // The node that will come before this node is currently the list tail
    // Note that this also works if the list is empty
    newCity->prev = cityNetwork->tail;

    // And we will not have a next element
    newCity->next = NULL;

    // If the list is not empty ...
    if (cityNetwork->tail != NULL)
    {
        // ... tell the current tail that it now has a next element ...
        cityNetwork->tail->next = newCity;
    }

    // ... and update the list to make the new node the tail.
    cityNetwork->tail = newCity;

    // If list is empty ...
    if (cityNetwork->head == NULL)
    {
        // ... set node as the head as well.
        cityNetwork->head = newCity;
    }

    // Increase the network size by 1.
    cityNetwork->size++;

    // Print and indicate success (return '1').
    printf("Success: %s added to network.\n", cityName);
    return 1;
}

/*
    Add Path:
        - Adds a weighted connection between two given city nodes.
        - Attaches each city nodes to each other's connection list along with distance, connecting the two cities.
        - Flags an error if either or both cities aren't present within the network already.
        - Returns indication if function was successful.
            '1' -> Both cities are present in network and edge was successfully added.
            '0' -> City/Cities not present in network or distance is invalid, or other additional error flags.
*/
int addPath(struct Network* cityNetwork, char city1Name[127], char city2Name[127], int distance)
{
    // Find city indexes.
    int city1Index = cityNameSearch(cityNetwork, city1Name);
    int city2Index = cityNameSearch(cityNetwork, city2Name);

    // Error flagging system:

    // Check if both city names are present in network and that distance is valid.
    int edgeErrorCode = 0;

    if (city1Index < 0 && city2Index < 0)
        edgeErrorCode = 1;

    else if (city1Index < 0)
        edgeErrorCode = 2;

    else if (city2Index < 0)
        edgeErrorCode = 3;

    else if (cityNetwork->size <= 1)
        edgeErrorCode = 4;

    else if (distance <= 0)
        edgeErrorCode = 5;

    // Prints error information to the user.
    switch(edgeErrorCode) {
        case 1 :
            printf("Failure: Cities '%s' and '%s' not found in network! - (addPath - graphNetwork.c)\n", city1Name, city2Name);
            return 0;
        case 2 :
            printf("Failure: City '%s' not found in network! - (addPath - graphNetwork.c)\n", city1Name);
            return 0;
        case 3 :
            printf("Failure: City '%s' not found in network! - (addPath - graphNetwork.c)\n", city2Name);
            return 0;
        case 4 :
            printf("Failure: Network must contain at least 2 cities to create an edge! - (addPath - graphNetwork.c)\n");
            return 0;
        case 5 :
            printf("Failure: Distance must be greater than 0km! - (addPath - graphNetwork.c)\n");
            return 0;
        default :
            // No errors found, continue code.
            break;
    }

    // Define and get both city nodes.
    struct Node* city1Node = getNode(cityNetwork, city1Index);
    struct Node* city2Node = getNode(cityNetwork, city2Index);

    // Add both cities to each others connection lists with the distance defined.
    listAdd(city1Node->connections, city2Node, distance);
    listAdd(city2Node->connections, city1Node, distance);

    // Print and indicate success.
    printf("Success: (%s -> %s) of distance %dkm added to network.\n", city1Name, city2Name, distance);
    return 1;
}

/*
    Remove City:
        - Removes entity through a given name from a list.
        - Frees node from memory once untied from list.
        - Returns indication if function was successful.
            '1' -> Entity found and removed from list.
            '0' -> Entity not present in list.
*/
int removeCity(struct Network* cityNetwork, char cityName[127])
{
    // Find the requested node.
    int cityIndex = cityNameSearch(cityNetwork, cityName);

    // If the requested node is invalid then return failure.
    if (cityIndex == -1)
    {
        printf("Failure: %s not present within network. - (removeCity - graphNetwork.c)\n", cityName);
        return 0;
    }

    // Node is present within network:

    struct Node* currentCity = getNode(cityNetwork, cityIndex);
    int* pathDistance = &cityIndex;

    // Remove all edges attached to node to remove.
    for (cityIndex = 0; cityIndex < nodeDegree(currentCity); cityIndex++)
    {
        // Get connectedCity on respective currentCity's connection list ...
        struct Node* connectedCity = getListEntity(currentCity->connections, cityIndex, pathDistance);

        // ... and remove currentCity from connectedCity's connection list.
        int listIndex = listSearch(connectedCity->connections, currentCity->name);
        listRemove(connectedCity->connections, listIndex);
    }

    // If we have a previous item make it point to this node's next item ...
    if (currentCity->prev != NULL)
        currentCity->prev->next = currentCity->next;

    // ... and for the next node's previous item
    if (currentCity->next != NULL)
        currentCity->next->prev = currentCity->prev;

    // If entity was the first item, make the head of the network the current node's next item.
    if (currentCity == cityNetwork->head)
        cityNetwork->head = currentCity->next;

    // And similar for the last item.
    if (currentCity == cityNetwork->tail)
        cityNetwork->tail = currentCity->prev;

    // Free memory associated with removed node.
    nodeDestructor(currentCity);

    // Update the element count.
    cityNetwork->size--;

    // Print and indicate success (return '1').
    printf("Success: %s removed from network.\n", cityName);
    return 1;
}

/*
    Remove Edge:
        - Unties city node at a given index from the network.
        - Frees node from memory.
        - Returns indication if function was successful.
            '1' -> Edge found and removed from network.
            '0' -> Cities / Edge not present in network, or other additional error flags.
*/
int removePath(struct Network* cityNetwork, char city1Name[127], char city2Name[127])
{
    // Find city indexes.
    int city1Index = cityNameSearch(cityNetwork, city1Name);
    int city2Index = cityNameSearch(cityNetwork, city2Name);

    // Error flagging system:

    // Check if both city names are present in network and that distance is valid.
    int pathErrorCode = 0;

    if (city1Index < 0 && city2Index < 0)
        pathErrorCode = 1;

    else if (city1Index < 0)
        pathErrorCode = 2;

    else if (city2Index < 0)
        pathErrorCode = 3;

    else if (cityNetwork->size <= 1)
        pathErrorCode = 4;

    // Prints error information to the user.
    switch(pathErrorCode) {
        case 1 :
            printf("Failure: Cities '%s' and '%s' not found in network! - (removePath - graphNetwork.c)\n", city1Name, city2Name);
            return 0;
        case 2 :
            printf("Failure: City '%s' not found in network! - (removePath - graphNetwork.c)\n", city1Name);
            return 0;
        case 3 :
            printf("Failure: City '%s' not found in network! - (removePath - graphNetwork.c)\n", city2Name);
            return 0;
        case 4 :
            printf("Failure: Network must contain at least 2 cities to remove an edge! - (removePath - graphNetwork.c)\n");
            return 0;
        default :
            // No errors found, continue code.
            break;
    }

    // Define and get both city nodes.
    struct Node* city1Node = getNode(cityNetwork, city1Index);
    struct Node* city2Node = getNode(cityNetwork, city2Index);

    // Search for city name in each others respective connections list.
    int list1Index = listSearch(city1Node->connections, city2Node->name);
    int list2Index = listSearch(city2Node->connections, city1Node->name);

    // Flag error and return '0' is either city isn't connected to the other.
    if (list1Index == -1 || list2Index == -1)
    {
        printf("Failure: Either city is not connected to the other respective city! - (removePath - graphNetwork.c)\n");
        return 0;
    }

    // Both cities are present in both the network and in each other's connections list, continue code.

    // Remove both cities to each others connection lists with the distance defined.
    listRemove(city1Node->connections, city2Index);
    listRemove(city2Node->connections, city1Index);

    // Print and indicate success.
    printf("Success: Edge (%s -> %s) removed from network.\n", city1Name, city2Name);
    return 1;
}


/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                                            NETWORK DISPLAY FUNCTIONS
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

/*
    Display Network:
        - Prints all nodes in a given network as an array.
        - Additionally indicates if network is empty.
*/
void displayNetwork(struct Network* cityNetwork)
{
    // Print if network is empty and immediately return.
    if (cityNetwork->size == 0 || cityNetwork == NULL)
    {
        printf("\n\tNetwork empty!\n");
        return;
    }

    printf("\n\tNetwork: [");

    // Start at the head of the network list ...
    struct Node* currentCity = cityNetwork->head;

    // ... and print each element in turn until NULL.
    while (currentCity != NULL)
    {
        if (currentCity != cityNetwork->head)
            printf(",");

        // Print the current city name.
        printf(" %s", currentCity->name);

        // Move to the next node.
        currentCity = currentCity->next;
    }

    printf(" ]\n");
}

/*
    Display Connections:
        - Prints all connected nodes in a given node's connection list as an array.
        - Additionally indicates if the network or connection list is empty.
*/
void displayConnections(struct Network* cityNetwork, char cityName[127])
{
    // Print if network is empty and immediately return.
    if (cityNetwork->size == 0) {
        printf("\nNetwork empty!\n");
        return;
    }

    // Find the requested node.
    int cityIndex = cityNameSearch(cityNetwork, cityName);

    // If the requested node is invalid then return failure.
    if (cityIndex == -1) {
        printf("Failure: %s not present within network. - (displayConnections - graphNetwork.c)\n", cityName);
        return;
    }

    // Node is present within network:
    struct Node* currentCity = getNode(cityNetwork, cityIndex);

    printf("'%s' Connections: \n", currentCity->name);

    // Read all cities and paths attached to node to print.
    listDisplay(currentCity->connections);
}

/*
    Get Node Name:
        - Returns pointer of the name of the given node.
*/
char* getNodeName(struct Node* node)
{
    return node->name;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                                        NETWORK DIJKSTRA'S ALGORITHM
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

/*
    Dijkstra:
        - Performs Dijkstra's algorithm from two given city names.
        - Prints relevant information and the results of the algorithm, such as the path and the minimal distance,
            to the terminal and to a 'dijkstraresults.txt' text file.
        - Flags an error if either or both cities aren't present within the network already.
        - Additionally calculates algorithm duration time (speed), result is printed at the end.
        - Returns indication if function was successful.
            '1' -> Network scanned - Path found or cities are unreachable.
            '0' -> City names invalid / Network is too small, or other additional error flags.
        - (Code referenced from Vaidehi Joshi - Reference [8])
*/
int dijkstra(FILE* dijkstraresults_file, double *algorithmTime, struct Network* cityNetwork, char cityStartName[127], char cityEndName[127])
{
    // Find city indexes.
    int cityStartIndex = cityNameSearch(cityNetwork, cityStartName);
    int cityEndIndex = cityNameSearch(cityNetwork, cityEndName);

    // Error flagging system:

    // Error checking system if cities are in the specified network.
    int dijkstraErrorCode = 0;

    if (cityStartIndex < 0 && cityEndIndex < 0)
        dijkstraErrorCode = 1;

    else if (cityStartIndex < 0)
        dijkstraErrorCode = 2;

    else if (cityEndIndex < 0)
        dijkstraErrorCode = 3;

    else if (cityNetwork->size <= 1)
        dijkstraErrorCode = 4;

    // Print respective error case and indicate failure ('0').
    switch(dijkstraErrorCode) {
        case 1 :
            printf("Failure: Cities '%s' and '%s' not found in network! - (dijkstra - graphNetwork.c)\n",
                    cityStartName, cityEndName);
            return 0;
        case 2 :
            printf("Failure: City '%s' not found in network! - (dijkstra - graphNetwork.c)\n", cityStartName);
            return 0;
        case 3 :
            printf("Failure: City '%s' not found in network! - (dijkstra - graphNetwork.c)\n", cityEndName);
            return 0;
        case 4 :
            printf("Failure: Network must contain at least 2 cities! - (dijkstra - graphNetwork.c)\n");
            return 0;
        default :
            // No errors were found.
            printf("\n- DIJKSTRA'S ALGORITHM -\n");
            fprintf(dijkstraresults_file, "\n- DIJKSTRA'S ALGORITHM -\n");
            break;
    }

    // Set up network:

        printf("Shortest path between '%s' and '%s'.\n", cityStartName, cityEndName);
        fprintf(dijkstraresults_file, "Shortest path between '%s' and '%s'.\n", cityStartName, cityEndName);

        // Initialise respective start and end city nodes.
        struct Node* startCity = getNode(cityNetwork, cityStartIndex);
        struct Node* endCity = getNode(cityNetwork, cityEndIndex);

        // Create a priority stack array.
        struct Stack* priorityStack = stackConstructor();

        // Reset all nodes in network to initial Dijkstra state.
        struct Node* currentCity = cityNetwork->head;

        // Edge distance between current and connected cities.
        int edgeDistance;

        // Initialise Network.
        while (1)
        {
            currentCity->isVisited = 0;
            currentCity->shortestDistance = INT_MAX;
            currentCity->routeEdgeDistance = 0;
            currentCity->back = NULL;
            if (currentCity->next == NULL)
                break;
            else
                currentCity = currentCity->next;
        }

        // Initialise starting position.
        startCity->shortestDistance = 0;
        startCity->routeEdgeDistance = 0;
        currentCity = startCity;

    // Start Algorithm:
    timer stopwatch;
    stopwatch.timeStart = (double) clock();    // Start clock.

    do { // While endCity has not been visited.

        int connectionIndex; // Index within the currentCity's connection list.

        for (connectionIndex = 0; connectionIndex < nodeDegree(currentCity); connectionIndex++)
        {
            // Read each connected city on respective currentCity's connection list.
            struct Node* connectedCity = getListEntity(currentCity->connections, connectionIndex, &edgeDistance);

            // If node has not yet been visited, ...
            if (connectedCity->isVisited == 0)
            {
                // ... check if the connectedCity's shortest distance is shorter via the currentCity than it's
                //      shortest distance, ...
                if (connectedCity->shortestDistance > currentCity->shortestDistance + edgeDistance)
                {
                    // ... and update its shortest distance and its route if true.
                    connectedCity->shortestDistance = currentCity->shortestDistance + edgeDistance;
                    connectedCity->routeEdgeDistance = edgeDistance;
                    connectedCity->back = currentCity;

                }

                // Check if connected city is already in the priority stack, ...
                if (stackSearch(priorityStack, connectedCity->name) == -1)
                {
                    // ... if it isn't, enqueue current city node to priority stack with relative distance travelled.
                    stackPush(priorityStack, connectedCity, connectedCity->shortestDistance, edgeDistance);
                }
            }
        }

        // Current city has now been visited and is now fully explored.
        currentCity->isVisited = 1;

        // Sort the priority stack.
        stackHeapSort(priorityStack);

        // Set next current city to the city with shortest total distance within the stack.
        currentCity = stackPop(priorityStack, &edgeDistance);

        // If there are no more nodes to explore and endCity has still not been found, cities are disconnected.
        if (currentCity == NULL)
            break;

    } while (endCity->isVisited != 1);

    // Stop Algorithm:
    stopwatch.timeEnd = (double) clock();      // Stop clock.

    printf("PATH RESULTS:\n");
    fprintf(dijkstraresults_file, "PATH RESULTS:\n");

    // PATH NOT FOUND / UNREACHABLE:

        if (endCity->isVisited == 0)
        {
            printf("\tPath not found! - Cities are unreachable.\nALGORITHM COMPLETED\n");
            fprintf(dijkstraresults_file, "\tPath not found! - Cities are unreachable.\nALGORITHM COMPLETED\n");
            return 1;
        }

    // PATH FOUND:

        // Start at end city.
        currentCity = endCity;

        printf("\tPath: [ ");
        fprintf(dijkstraresults_file, "\tPath: [ ");

        // Reverse order of Dijkstra path by back propagating from the end city and pushing to a stack.
        struct Stack* dijkstraPath = stackConstructor();

        while (currentCity != NULL)
        {
            stackPush(dijkstraPath, currentCity, currentCity->shortestDistance, edgeDistance);
            currentCity = currentCity->back;
        }

        // Print Results.
        while (stackIsEmpty(dijkstraPath) == 0)
        {
            // Pop next city from dijkstraPath stack.
            currentCity = stackPop(dijkstraPath, &edgeDistance);

            // Print edge distance between cities.
            if (currentCity != startCity)
            {
                printf(" -(%dkm)-> ", currentCity->routeEdgeDistance);
                fprintf(dijkstraresults_file, " -(%dkm)-> ", currentCity->routeEdgeDistance);
            }

            // Print current city name.
            printf("%s", currentCity->name);
            fprintf(dijkstraresults_file, "%s", currentCity->name);
        }

        printf(" ]\n\tThe distance of this path is %dkm.\n", endCity->shortestDistance);
        fprintf(dijkstraresults_file, " ]\n\tThe distance of this path is %dkm.\n", endCity->shortestDistance);

    // Free stack memories associated with function.
    stackDestructor(priorityStack);
    stackDestructor(dijkstraPath);

    // Calcuate function time duration.
    stopwatch.timeInSeconds = (stopwatch.timeEnd - stopwatch.timeStart) / CLOCKS_PER_SEC;

    // Store computation time in 'algorithmTime' pointer argument.
    *algorithmTime = stopwatch.timeInSeconds;

    printf("ALGORITHM COMPLETE - (%fs)\n", stopwatch.timeInSeconds);
    fprintf(dijkstraresults_file, "ALGORITHM COMPLETE - (%fs)\n", stopwatch.timeInSeconds);

    return 1;
}
