#ifndef _THREAD_H_
#define _THREAD_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct CPU{
    int id;         
    int ioTime;
    int CPUTime;
    int CPUTimeOgCopy;      //helps in getting the Total Service Time of the CPU
} CPU;

typedef struct Thread{
    int id;
    int arrivalTime;
    int CPUs;
    int turnAround;
    int finishTime;
    CPU **CPUList;
} Thread;

typedef struct Process{
    int id;
    int threadOH;
    int processOH;
    int threads;
    Thread **threadList;
} Process;

typedef struct Event{
    Thread * thread;
    Process * process;
    int state;
    int time;
    int burst;
} Event;

typedef struct Result{
    int TotalTime;
    double avgTurnaround;
    double CPUtil;
} Result;

typedef struct Verbose{
    int time;
    Thread *thread;
    Process *process;
    int state1;
    int state2;
} Verbose;

#include "heap.h"

void fcfs(Process **PL, int size, int d, int v);
void rr(Process **PL, int size, int quant, int d, int v);


#endif
