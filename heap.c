#include "heap.h"

/*
 *      Title: heap.c
 *      Author: Alif Merchant
 *      Date: Thursday, March 5, 2021
 *      Reference: https://algorithmtutor.com/Data-Structures/Tree/Binary-Heaps/ (max-heap, which I changed into a min-heap)
 */


int parent(int i) {
    return (i - 1) / 2;
}

/* 
 *      return the index of the left child 
 */
int left_child(int i) {
    return 2*i + 1;
}

/* 
 *      return the index of the right child
 */ 
int right_child(int i) {
    return 2*i + 2;
}

/* 
 *      Swap 2 Event pointers
 */
void swap(Event *x, Event *y) {
    Event temp = *x;
    *x = *y;
    *y = temp;
}

/* 
 *     insert an Event item at the appropriate position in the heap
 */
void insertToHeap(Event **a, Event *data, int *n, int size) {
    if (*n >= size) {
        printf("The heap is full. Cannot insert\n");
        return;
    }
    // first insert the time at the last position of the array 
    // and move it up
    a[*n] = data;
    *n = *n + 1;

    // move up until the heap property satisfies
    int i = *n - 1;
    while (i != 0 && a[parent(i)]->thread->arrivalTime > a[i]->thread->arrivalTime) {
        swap(a[parent(i)], a[i]);
        i = parent(i);
    }
}

/* 
 *      moves the item at position i of array a 
 *      into its appropriate position
 */

void min_heapify(Event ** a, int i, int n){
    // find left child node
    int left = left_child(i);

    // find right child node
    int right = right_child(i);

    // find the smallest among 3 nodes
    int smallest = i;

    // check if the left node is smaller than the current node
    if (left < n && a[left]->thread->arrivalTime < a[smallest]->thread->arrivalTime) {
        smallest = left;
    }

    // check if the right node is smaller than the current node
    if (right < n && a[right]->thread->arrivalTime < a[smallest]->thread->arrivalTime) {
        smallest = right;
    }

    // swap the smallest node with the current node 
    // and repeat this process until the current node is smaller than 
    // the right and the left node
    if (smallest != i) {
        swap(a[i],a[smallest]);
        min_heapify(a, smallest, n);
    }

}

/* 
 *      returns the smallest item of the heap. (the item at the top)   
 */
Event *get_min(Event **a) {
    return a[0];
}

/* 
 *      returns the smallest item of the heap and deletes it.   
 */
Event *extract_min(Event **a, int *n) {
    Event *min_item = a[0];

    // replace the first item with the last item
    a[0] = a[*n - 1];
    *n = *n - 1;

    // maintain the heap property by heapifying the 
    // first item
    min_heapify(a, 0, *n);
    return min_item;
}


/*  Verbose heap functions  */


/* 
 *      Swap 2 Verbose pointers
 */
void swap_verb(Verbose *x, Verbose *y) {
    Verbose temp = *x;
    *x = *y;
    *y = temp;
}

/* 
 *     insert a Verbose item at the appropriate position in the heap
 */
void insertToHeap_verb(Verbose **a, Verbose *data, int *n, int size) {
    if (*n >= size) {
        printf("The heap is full. Cannot insert\n");
        return;
    }
    // first insert the time at the last position of the array 
    // and move it up
    a[*n] = data;
    *n = *n + 1;

    // move up until the heap property satisfies
    int i = *n - 1;
    while (i != 0 && a[parent(i)]->time > a[i]->time) {
        swap_verb(a[parent(i)], a[i]);
        i = parent(i);
    }
}

/* 
 *      moves the item at position i of array a 
 *      into its appropriate position
 */

void min_heapify_verb(Verbose ** a, int i, int n){
    // find left child node
    int left = left_child(i);

    // find right child node
    int right = right_child(i);

    // find the smallest among 3 nodes
    int smallest = i;

    // check if the left node is smaller than the current node
    if (left < n && a[left]->time < a[smallest]->time) {
        smallest = left;
    }

    // check if the right node is smaller than the current node
    if (right < n && a[right]->time < a[smallest]->time) {
        smallest = right;
    }

    // swap the smallest node with the current node 
    // and repeat this process until the current node is smaller than 
    // the right and the left node
    if (smallest != i) {
        swap_verb(a[i],a[smallest]);
        min_heapify_verb(a, smallest, n);
    }

}

/* 
 *      returns the smallest item of the heap. (the item at the top)   
 */
Verbose *get_min_verb(Verbose **a) {
    return a[0];
}

/* 
 *      returns the smallest item of the heap and deletes it.   
 */
Verbose *extract_min_verb(Verbose **a, int *n) {
    Verbose *min_item = a[0];

    // replace the first item with the last item
    a[0] = a[*n - 1];
    *n = *n - 1;

    // maintain the heap property by heapifying the 
    // first item
    min_heapify_verb(a, 0, *n);
    return min_item;
}

