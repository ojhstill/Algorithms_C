// Exam Number: Y3857872
/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

    priorityStack.c:
        - Source file defining the functions to create and and control priority stacks.
        - Implemented within 'graphNetwork.c' for Dijkstra's function as a priority queue to
            return the lowest relative distances.
        - Stack can use 'bubble sort' or 'heap sort' algorithms to order the array.
                                            ^ (default is set to heap sort)
        - (Code referenced and adapted from Andrew Turner - Reference [7])

        - Functions:
            - stackConstructor
            - stackDestructor
            - stackSize
            - stackIsEmpty
            - stackResize
            - stackSearch
            - stackPush
            - stackPop
            - stackIsSorted
            - stackUpdate
            - swap
            - stackBubbleSort
            - heapify
            - buildMaxHeap
            - stackHeapSort

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

// Include respective header file:
#include "stack.h"


/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                                                    STACK STRUCTURES
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

/*
	Dijkstra Structure:
        - Internal custom structure.
        - Used to store the relative information for Dijkstra's algorithm as an item within a priority stack.

		city                - Pointer to a node in a given network.
		shortestDistance    - Relative shortest distance to the respective node.
		edgeDistance        - Edge distance to node on the respective node path.
*/
struct Item
{
    struct Node* city;
    int shortestDistance;
    int edgeDistance;
};

/*
	Stack Structure:
		length          - Maximum length of the stack.
		top             - Position of the next empty index in the stack
		cities          - Array of individual pointer entities, each of the Dijkstra structure.
		isSorted        - Indication if priority stack has been sorted.
*/
struct Stack
{
    int length;
    int top;
    struct Item* item;
    int isSorted;
};


/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                                            STACK CONSTRUCTORS/DESTRUCTORS
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

/*
    Stack Constructor:
        - Allocate memory for and create a new stack.
        - Returns pointer to new stack.
*/
struct Stack* stackConstructor()
{
    struct Stack* stack;

    // Reserve memory for the stack and for the items we want to store.
    stack = malloc(sizeof(struct Stack));
    stack->item =  malloc(STACK_CAPACITY * sizeof(struct Item));

    // Initialise stack values.
    stack->length = STACK_CAPACITY;
    stack->top = 0;
    stack->isSorted = 0;

    // Return new stack.
    return stack;
}

/*
    Stack Destructor:
        - Free memory associated with a stack.
*/
void stackDestructor(struct Stack* stack)
{
    free(stack->item);
    free(stack);
}


/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                                                STACK SIZE FUNCTIONS
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

/*
    Stack Size:
        - Returns the current number of items in a given stack.
*/
int stackSize(struct Stack* stack)
{
    return stack->top;
}

/*
    List Is Empty:
        - Checks if a given stack is empty.
        - Uses internal stackSize() function.
            '1' -> Stack is empty.
            '0' -> Stack contains at least one item.
*/
int stackIsEmpty(struct Stack* stack)
{
    if (stackSize(stack) == 0)
        return 1;

    else return 0;
}

/*
    Stack Resize:
        - Function to increase the maximum capacity of a given stack once the maximum has been reached.
        - Capacity is only ever increased by a factor of two.
        - Used within stackPush() if DYNAMIC_RESIZE from 'network.h' is enabled.
        - Memory is reallocated.
*/
void stackResize(struct Stack* stack, int newSize)
{
    // The list is already big enough to store the number of elements requested, do nothing.
    if (newSize <= stack->length)
        return;

    // Otherwise reallocate the entities array
    stack = realloc(stack, sizeof(struct Stack));
    stack->item = realloc(stack->item, newSize * sizeof(struct Item));

    // And update our capacity
    stack->length = newSize;
}


/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                                                STACK SEARCH FUNCTIONS
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

/*
    List Search:
        - Searches for a node within a list using a given city name.
        - Returns integer value.
            'index' -> City name found, index within list returned.
            '-1'    -> Node not present in list.
*/
int stackSearch(struct Stack* stack, char cityName[127])
{
    int index;

    // Check each occupied element in the list in turn
    for (index = 0; index < stack->top; index++)
    {
        struct Node* currentCity = stack->item[index].city;

        // If the current element matches the requested one
        if (strcmp(getNodeName(currentCity), cityName) == 0)
        {
            // Return its index
            return index;
        }
    }

    // If we didn't execute the return in the loop above then the
    // entity was not found. In this case we return -1.
    return -1;
}


/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                                            STACK ADD/REMOVE FUNCTIONS
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

/*
    Stack Push:
        - Adds a new item to a stack using a given node and distances.
        - If DYNAMIC_RESIZE from 'network.h' is enabled, stack length will double once limit has been reached.
        - Returns indication if function was successful.
            '1' -> Item added to stack.
            '0' -> Stack is at capacity and DYNAMIC_RESIZE is disabled.
*/
int stackPush(struct Stack* stack, struct Node* city, int shortestDistance, int edgeDistance)
{
    // Return '0' if stack is full. / Resize if stack is full.
    if (stack->top >= stack->length)
    {
        if (DYNAMIC_RESIZE == 0)
        {
            // List at capacity, return '0' and print failure.
            printf("Failure: Priority stack at capacity! - (stackPush - priorityStack.c)\n");
            printf("\t^ (Enable DYNAMIC_RESIZE or increase PRIORITY_STACK_CAPACITY in 'stack.h')");
            return 0;
        }

            // Call stack resize function to increase memory for more elements.
        else stackResize(stack, stack->length * 2);
    }

    // Write in connected city name and distance in to next available slot in stack.
    stack->item[stack->top].city = city;
    stack->item[stack->top].shortestDistance = shortestDistance;
    stack->item[stack->top].edgeDistance = edgeDistance;

    // Assume and set stack as unsorted.
    stack->isSorted = 0;

    // Update item count.
    stack->top++;

    // Indicate success.
    return 1;
}

/*
    Stack Pop:
        - Removes the highest priority city from a given stack.
        - Function returns shortest distance and edge distance to given address arguments.
        - Returns highest priority city if function was successful.
            'Node*' -> Node pointer of highest priority city within stack.
            'NULL'  -> Stack is empty.
*/
struct Node* stackPop(struct Stack* stack, int* edgeDistance)
{
    // If the stack is empty then do nothing.
    if (stackIsEmpty(stack))
        return NULL;

    // Update number of items in stack.
    stack->top--;

    // Set distances to the requested argument address.
    //*shortestDistance = stack->item[stack->top].shortestDistance;
    *edgeDistance = stack->item[stack->top].edgeDistance;

    // Return popped city from stack.
    return stack->item[stack->top].city;
}


/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                                            STACK SORTING FUNCTIONS
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

/*
    Stack Is Sorted:
        - Checks if a given stack is sorted.
            '1' -> Stack is sorted.
            '0' -> Stack is unordered.
*/
int stackIsSorted(struct Stack* stack)
{
    return stack->isSorted;
}

/*
    Swap:
        - Swaps two stack items stored in two provided addresses.
        - Used within the relative sorting function.
*/
void swap(struct Item* itemL, struct Item* itemR)
{
    struct Item temp = *itemL;
    *itemL = *itemR;
    *itemR = temp;
}

/*
    Stack Bubble Sort:
        - Sorts an unordered stack into descending order.
        - Smaller item shortDistance values are given higher priority when removed from stack.
        - Uses the 'bubble sort' algorithm and the swap() function to order the array.
        - If stack is already sorted, function will not change anything.
        - (Code referenced and adapted from Andrew Turner - Reference [9])
*/
void stackBubbleSort(struct Stack* stack)
{
    // If stack is already sorted, do nothing.
    if (stackIsSorted(stack) == 1)
        return;

    int index = 0;

    // Ensure that we enter the while loop below.
    int sorted = 0;

    while (sorted == 0)
    {
        // Assume array is sorted.
        sorted = 1;

        // Check each pair of elements in turn.
        for (index = 1; index < stack->top; index++)
        {
            // If we find one out of order ...
            if (stack->item[index - 1].shortestDistance < stack->item[index].shortestDistance)
            {
                // ... mark our array as not sorted ...
                sorted = 0;

                // ... and put the elements in the correct position.
                swap(&stack->item[index - 1], &stack->item[index]);
            }
        }
    }

    stack->isSorted = 1;
}

/*
    Heapify:
        - Orders the max heap from a current and maximum index within a stack.
        - Uses the swap() function to order items if two elements are found out of place.
        - (Code referenced and adapted from Vaidehi Joshi - Reference [10])
*/
void heapify(struct Stack* stack, int index, int max)
{
    int nextIndex, leftChildIndex, rightChildIndex;

    //
    while(index < max)
    {
        //stackDisplay(stack);

        // Set indexes within heap.
        nextIndex = index;
        leftChildIndex = (2 * index) + 1;
        rightChildIndex = leftChildIndex + 1;

        // Set items to compare within heap.
        struct Item* currentItem = &stack->item[index];
        struct Item* leftChild = &stack->item[leftChildIndex];
        struct Item* rightChild = &stack->item[rightChildIndex];

        if (leftChildIndex < max && leftChild->shortestDistance < currentItem->shortestDistance)
        {
            swap(currentItem, leftChild);
            nextIndex = leftChildIndex;
        }

        if (rightChildIndex < max && rightChild->shortestDistance < currentItem->shortestDistance)
        {
            swap(currentItem, rightChild);
            nextIndex = rightChildIndex;
        }

        // if nextIndex has not changed, current index and children are sorted.
        if (nextIndex == index)
            return;

        // Move to next index.
        index = nextIndex;
    }
}

/*
    Build Max Heap:
        - Takes a stack and builds a max heap binary tree using an array representation.
        - Uses the heapify() function to order the array as items are passed in.
        - (Code referenced and adapted from Vaidehi Joshi - Reference [10])
*/
void buildMaxHeap(struct Stack* stack)
{
    int i = stack->top / 2 - 1;

    // Set j to the lower bound integer value.
    int j = (int) floor(i);

    // Build a max heap out of all stack items passed in.
    while (j >= 0)
    {
        heapify(stack, j, stack->top - 1);
        j--;
    }
}

/*
    Stack Heap Sort:
        - Sorts an unordered stack into descending order.
        - Smaller item shortDistance values are given higher priority when removed from stack.
        - Uses the 'heap sort' algorithm and the heapify() function to order the array.
        - If stack is already sorted, function will not change anything.
        - (Code referenced and adapted from Vaidehi Joshi - Reference [10])
*/
void stackHeapSort(struct Stack* stack)
{
    // If stack is already sorted, do nothing.
    if (stackIsSorted(stack) == 1)
        return;

    // Build a max heap.
    buildMaxHeap(stack);

    // Find last item index in stack.
    int lastElementIndex = stack->top - 1;

    // Set room item in stack for heap sort.
    struct Item* root = &stack->item[0];

    // Continue heap sorting until we have just one element left in the array.
    while(lastElementIndex > 0)
    {
        // Set element to sort in stack.
        struct Item* lastElement = &stack->item[lastElementIndex];

        // Swap the root with the last item.
        swap(root, lastElement);

        // Sort the max heap
        heapify(stack, 0, lastElementIndex);

        // Go to the next index.
        lastElementIndex--;
    }

    // Set stack to sorted.
    stack->isSorted = 1;
}
