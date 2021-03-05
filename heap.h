#ifndef _HEAP_H_
#define _HEAP_H_

#include "thread.h"

void insertToHeap(Event **a, Event *data, int *n, int size);
Event *extract_min(Event **a, int *n);
Event *get_min(Event **a);

void insertToHeap_verb(Verbose **a, Verbose *data, int *n, int size);
Verbose *extract_min_verb(Verbose **a, int *n);


#endif
