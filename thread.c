#include "thread.h"

#define PQSIZE 10000
#define VERBSIZE 10000

/*
 *      Title: thread.c
 *      Author: Alif Merchant
 *      Date: Thursday, March 5, 2021
 *      
 */


/*
 *      This function creates the initial priority queue 
 *      by inserting events that contain the thread and the
 *      process they belong to. It also initializes the time, 
 *      the state, and the burst to 0. 
 */
int createPQ(Process **PL, Event **PQueue, int size){
    int i, j, n = 0;
    for(i = 0; i < size; i++){
        for(j = 0; j < PL[i]->threads; j++){
            Event *event = malloc(sizeof(Event));
            event->process = PL[i];
            event->thread = PL[i]->threadList[j];
            event->time = 0;
            event->state = 0;
            event->burst = 0;
            insertToHeap(PQueue, event, &n, PQSIZE);
            //n++;
        }
    }
    return n;
}

/*
 *      This function prints all the details of a Result
 *      structure. It prints the total time, the average turnaround
 *      and the CPU utilization.
 */
void printResult(Result * result){
    printf("Total Time Required = %d units\n", result->TotalTime);
    printf("Average Turnaround Time is %.2f units\n", result->avgTurnaround);
    printf("CPU utilization is %.2f %%\n", result->CPUtil);
}

/*
 *      This function prints details about each thread that was 
 *      passed in. It prints details such as which process it
 *      belongs to, its arrival time, service time, total I/O time,
 *      turnaround time, and the finish time.
 */
void printDetails(Process **PL, int size){
    for(int i = 0; i < size; i++){
        for(int j = 0; j < PL[i]->threads; j++){
            Thread * ref = PL[i]->threadList[j];
            printf("Thread %2d of Process %2d\n", j+1, i+1);
            printf("    arrival time: %d\n", ref->finishTime - ref->turnAround);
            int serviceTime = 0;
            int IOTime = 0;
            for(int k = 0; k < ref->CPUs; k++){
                CPU * ref2 = ref->CPUList[k];
                serviceTime += ref2->CPUTimeOgCopy;
                IOTime += ref2->ioTime;
            }
            printf("    service time: %3d units, I/O Time: %3d units, turnaround time: %4d units, finish time: %4d units\n",
                        serviceTime, IOTime, ref->turnAround, ref->finishTime);
        }
    }
}

/*
 *      This function returns the average turnaround time 
 *      for all the threads in the list of processes.
 */
double getAvgTurn(Process **PL, int size){
    int avg = 0;
    int count = 0;
    for(int i = 0; i < size; i++){
        int high = 0;
        int low = 100000;
        for(int j = 0; j < PL[i]->threads; j++){
            Thread * ref = PL[i]->threadList[j];
            if ( high < ref->finishTime){
                high = ref->finishTime;
            }
            if(low > ref->finishTime - ref->turnAround){
                low = ref->finishTime - ref->turnAround;
            }
        }
        avg += high - low;
        count++;
    }
    return (double)avg/count;
}

/*
 *      This function prints a verbose phrase using the inputed 
 *      Verbose structure. The states work as follows; 0 for new,
 *      1 for Ready, 2 for Running, 3 for blocked, and 4 for terminated. 
 */
void printVerbose(Verbose * verbose){
    char name[100], name2[100];
    switch (verbose->state1) {
        case 0:
            strcpy(name,"New");
            break;

        case 1:
            strcpy(name,"Ready");
            break;
        
        case 2:
            strcpy(name,"Running");
            break;
        
        case 3:
            strcpy(name,"Blocked");
            break;

        case 4:
            strcpy(name,"Terminated");
            break;
        
        default:
            break;
    }

    switch (verbose->state2) {
        case 0:
            strcpy(name2,"New");
            break;

        case 1:
            strcpy(name2,"Ready");
            break;
        
        case 2:
            strcpy(name2,"Running");
            break;
        
        case 3:
            strcpy(name2,"Blocked");
            break;

        case 4:
            strcpy(name2,"Terminated");
            break;
        
        default:
            break;
    }

    printf("At time %4d: Thread %2d of process %2d moves from %7s to %10s\n",
            verbose->time, verbose->thread->id, verbose->process->id, name, name2);
}

/*
 *      This function takes in a list of verbose. Since the list
 *      is a heap, it prints them out in order of time.
 */
void printAllVerbose(Verbose ** VBList, int* VBSize){
    Verbose * verbose;
    while(*VBSize > 0){
        verbose = extract_min_verb(VBList, VBSize);
        printVerbose(verbose);
        free(verbose);
    }
}

/*
 *      This function simply frees the verbose list.
 */
void freeAllVerbose(Verbose ** VBList, int* VBSize){
    Verbose * verbose;
    while(*VBSize > 0){
        verbose = extract_min_verb(VBList, VBSize);
        free(verbose);
    }
}

/*
 *      This function frees the list of completed Event 
 *      structures.
 */
void freeCompletedEvents(Event ** EL, int size){
    for(int i = 0; i<size ; i++){
        free(EL[i]);
    }
    free(EL);
}

/*
 *      This function changes the state of the current event. It also
 *      creates a new Verbose variable and inserts it to the list 
 *      of verbose structures
 */
void changeState(Event * event, int state, int time, Verbose **VList, int *VSize){
    
    Verbose * verbose = malloc(sizeof(Verbose));
    verbose->time = time;
    verbose->thread = event->thread;
    verbose->process = event->process;
    verbose->state1 = event->state;
    verbose->state2 = state;
    insertToHeap_verb(VList, verbose, VSize, VERBSIZE);

    event->state = state;
}


/*
 *      This function simulates the first come first serve
 *      CPU scheduling.
 */

void fcfs(Process **PL, int size, int d, int v){
    Verbose **VBList = malloc(sizeof(Verbose *)* VERBSIZE);     // Create an array of Verbose
    int VBSize = 0;

    Event **PQueue = malloc(sizeof(Event *) * PQSIZE);          // Create the priority queue, made of Event structures
    Event **completed = malloc(sizeof(Event *) * PQSIZE);       // Create an array of completed Events
    int completedSize = 0;

    Result *result = malloc(sizeof(Result));                    // Create a Result pointer, which will be printed at the end
    int PQSize = createPQ(PL, PQueue, size);                    // Initialize the Priority Queue

    int time = 0;
    int timeInCPU = 0;
    Event *current = NULL;
    Event *previous = NULL;
    Event *store = NULL;
    int slice = 0;
    int IOtime = 0;
    
    while(PQSize != 0){
        previous = current;                         
        current = get_min(PQueue);                  // Get an Event from the top of the priority min-heap (smallest arrival time)

        changeState(current, 1, current->thread->arrivalTime, VBList, &VBSize);     // Change state to Ready

        if(previous != NULL){                       // Check if we had a process before the current one
            if(previous->process != current->process){     //  Compare previous to current process
                time += current->process->processOH;     // If the processes are different, add the process overhead to time
            }else if(previous == current){
                time = previous->thread->arrivalTime;    // If the processes and threads ar the same, the time will be the same as the thread's arrival time
            }else{
                time += current->process->threadOH;      // If just the thread are different, add the thread overhead to time
            }
            current->time = time;
    
           
            timeInCPU += current->thread->CPUList[current->burst]->CPUTime;     // Increase the total amount of CPU time by the burst's CPU time

            time += current->thread->CPUList[current->burst]->CPUTime;     // Increase the amount of time by the burst's CPU time

            current->thread->turnAround += time - current->thread->arrivalTime + current->thread->CPUList[current->burst]->ioTime;      // Increase the turnaround time by subtracting arrival time + the burst's I/O time form current time
            IOtime = current->thread->CPUList[current->burst]->ioTime;      // Store the I/O time 
            current->burst++;      // Move to the next burst
        }else{                       // This section is similar to the one right above. It is meant to work just for the first event.
            current->time = time;

            timeInCPU += current->thread->CPUList[current->burst]->CPUTime; 

            time += current->thread->CPUList[current->burst]->CPUTime;

            current->thread->turnAround += time - current->thread->arrivalTime + current->thread->CPUList[current->burst]->ioTime; 
            IOtime = current->thread->CPUList[current->burst]->ioTime;
            current->burst++;

        }
        store = extract_min(PQueue,&PQSize);        // Remove the event on the top of the priority queue
        changeState(current, 2, current->time, VBList, &VBSize);    // Change the state to Running
        current->time = time;

        if(current->burst == current->thread->CPUs){    // If we finished running the last burst, we terminate the Thread
            current->thread->finishTime = time;
            changeState(current, 4, current->thread->finishTime, VBList, &VBSize);      // Change state to Terminated
            completed[completedSize++] = current;                                       // Save the event in the list of completed Events (so we can free them later)
        }else if(current->burst < current->thread->CPUs){      // If we have more bursts left
            changeState(current, 3, time, VBList, &VBSize);     // Change the state to blocked
            current->thread->arrivalTime = current->time + IOtime;      // Set the next arrival time (in this case, current->time = previous time + CPUTime)
            insertToHeap(PQueue,current,&PQSize,PQSIZE);        // Insert the Event back into the Priority Queue
        }

    }
    if(v){                                          // Print verbose if the v flag is set to 1
        printAllVerbose(VBList, &VBSize);
    }else{                                          // Or just free everything
        freeAllVerbose(VBList, &VBSize);
    }
    result->TotalTime = time;                       // Set the default results and print them
    result->CPUtil = 100 * (double)timeInCPU/time;
    result->avgTurnaround = getAvgTurn(PL, size);
    printResult(result);

    if(d){                                          // Print details if the d flag is set to 1
        printDetails(PL, size);
    }

    freeCompletedEvents(completed, completedSize);  // free all the completed events
    free(PQueue);                                   // free the priority queue
    free(VBList);                                   // free the verbose list
    free(result);                                   // free the result structure
}

/*
 *      This function simulates the round robin
 *      CPU scheduling.
 */

void rr(Process **PL, int size, int quant, int d, int v){
    Verbose **VBList = malloc(sizeof(Verbose *)* VERBSIZE);     // Create an array of Verbose
    int VBSize = 0;
    Event **PQueue = malloc(sizeof(Event *) * PQSIZE);          // Create the priority queue, made of Event structures
    Event **completed = malloc(sizeof(Event *) * PQSIZE);       // Create an array of completed Events
    int completedSize = 0;
    Result *result = malloc(sizeof(Result));                    // Create a Result pointer, which will be printed at the end
    int PQSize = createPQ(PL, PQueue, size);                    // Initialize the Priority Queue

    int time = 0;
    int timeInCPU = 0;
    Event *current = NULL;
    Event *previous = NULL;
    Event *store = NULL;
    int slice = 0;
    int IOtime = 0;
    
    while(PQSize != 0){
        previous = current;
        current = get_min(PQueue);                  // Get an Event from the top of the priority min-heap (smallest arrival time)

        changeState(current, 1, current->thread->arrivalTime, VBList, &VBSize);     // Change state to Ready

        if(previous != NULL){                       // Check if we had a process before the current one
            if(previous->process != current->process){     //  Compare previous to current process
                time += current->process->processOH;     // If the processes are different, add the process overhead to time
            }else if(previous == current){
                time = previous->thread->arrivalTime;    // If the processes and threads ar the same, the time will be the same as the thread's arrival time
            }else{
                time += current->process->threadOH;      // If just the thread are different, add the thread overhead to time
            }
            current->time = time;

            if(current->thread->CPUList[current->burst]->CPUTime > quant){      // If the current bursts CPUt time is bigger than the time quantum
                current->thread->CPUList[current->burst]->CPUTime -= quant;     // decrease burst's CPUTime by quantum
                //current->thread->arrivalTime += current->time + quant;
                
                timeInCPU += quant;     // Increase the total amount of CPU time by quantum
                time += quant;          // Increase the amount of time by quantum
                current->thread->turnAround += time - current->thread->arrivalTime;      // Increase the turnaround time by subtracting arrival time form current time
                IOtime = 0;             // Set I/O time to 0, since we r not doing IO
                
            }else{
                timeInCPU += current->thread->CPUList[current->burst]->CPUTime;     // Increase the total amount of CPU time by the burst's CPU time

                time += current->thread->CPUList[current->burst]->CPUTime;     // Increase the amount of time by the burst's CPU time

                current->thread->turnAround += time - current->thread->arrivalTime + current->thread->CPUList[current->burst]->ioTime;      // Increase the turnaround time by subtracting arrival time + the burst's I/O time form current time
                IOtime = current->thread->CPUList[current->burst]->ioTime;      // Store the I/O time 
                current->burst++;      // Move to the next burst
            }
        }else{                       // This section is similar to the one right above. It is meant to work just for the first event.
            current->time = time;
            if(current->thread->CPUList[current->burst]->CPUTime > quant){
                current->thread->CPUList[current->burst]->CPUTime -= quant;
                //current->thread->arrivalTime += current->time + quant;
                timeInCPU += quant; 
                time += quant;
                current->thread->turnAround += time - current->thread->arrivalTime;
                IOtime = 0;
            }else{
                timeInCPU += current->thread->CPUList[current->burst]->CPUTime; 

                time += current->thread->CPUList[current->burst]->CPUTime;

                current->thread->turnAround += time - current->thread->arrivalTime + current->thread->CPUList[current->burst]->ioTime; 
                IOtime = current->thread->CPUList[current->burst]->ioTime;
                current->burst++;
            }  
        }
        store = extract_min(PQueue,&PQSize);        // Remove the event on the top of the priority queue
        changeState(current, 2, current->time, VBList, &VBSize);    // Change the state to Running
        current->time = time;

        if(current->burst == current->thread->CPUs){    // If we finished running the last burst, we terminate the Thread
            current->thread->finishTime = time;
            changeState(current, 4, current->thread->finishTime, VBList, &VBSize);      // Change state to Terminated
            completed[completedSize++] = current;                                       // Save the event in the list of completed Events (so we can free them later)
        }else if(current->burst < current->thread->CPUs){      // If we have more bursts left
            changeState(current, 3, time, VBList, &VBSize);     // Change the state to blocked
            current->thread->arrivalTime = current->time + IOtime;      // Set the next arrival time (in this case, current->time = previous time + CPUTime)
            insertToHeap(PQueue,current,&PQSize,PQSIZE);        // Insert the Event back into the Priority Queue
        }

    }
    if(v){                                          // Print verbose if the v flag is set to 1
        printAllVerbose(VBList, &VBSize);
    }else{                                          // Or just free everything
        freeAllVerbose(VBList, &VBSize);
    }

    result->TotalTime = time;                       // Set the default results and print them
    result->CPUtil = 100 * (double)timeInCPU/time;
    result->avgTurnaround = getAvgTurn(PL, size);
    printResult(result);

    if(d){                                          // Print details if the d flag is set to 1
        printDetails(PL, size);
    }

    freeCompletedEvents(completed, completedSize);  // free all the completed events
    free(PQueue);                                   // free the priority queue
    free(VBList);                                   // free the verbose list
    free(result);                                   // free the result structure
}
