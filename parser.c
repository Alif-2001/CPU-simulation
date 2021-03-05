#include "parser.h"

/*
 *      Title: parser.c
 *      Author: Alif Merchant
 *      Date: Thursday, March 5, 2021
 *      
 */


/*
 *      This function reads each burst's detail
 *      from the file.
 */

void readCPU(FILE *fp, int count, CPU **CL){
    int n = 0, CPUTime, IOTime;
    while( n < count){
        CPU * cpu = malloc(sizeof(CPU));
        if(n == count-1){
            if(fscanf(fp, "%d %d", &n, &CPUTime) != EOF){
                cpu->id = n;
                cpu->CPUTime = CPUTime;
                cpu->CPUTimeOgCopy = CPUTime;
                cpu->ioTime = 0;
                //printf("        CPU n: %d %d\n", cpu->id, cpu->CPUTime);
                CL[n-1] = cpu;
            }
        }else{
            if(fscanf(fp, "%d %d %d", &n, &CPUTime, &IOTime) != EOF){
                cpu->id = n;
                cpu->CPUTime = CPUTime;
                cpu->CPUTimeOgCopy = CPUTime;
                cpu->ioTime = IOTime;
                //printf("        CPU n: %d %d %d\n", cpu->id, cpu->CPUTime, cpu->ioTime);
                CL[n-1] = cpu;
            }
        }
    }
}

/*
 *      This function reads each thread's detail
 *      from the file.
 */

void readThread(FILE *fp, int count, Thread **TL){
    int n = 0, CPUs, arrival;
    while(n < count){
        Thread * thread = malloc(sizeof(Thread));
        if(fscanf(fp, "%d %d %d", &n, &arrival, &CPUs) != EOF){
            thread->id = n;
            thread->arrivalTime = arrival;
            thread->CPUs = CPUs;
            thread->finishTime = 0;
            thread->turnAround = 0;
            thread->CPUList = malloc(sizeof(CPU *)* CPUs);
            /* printf("    Thread n: %d\n", thread->id);
            printf("    arrival n CPUs: %d\n", thread->arrivalTime);
            printf("    Thread n CPUs: %d\n", thread->CPUs); */
            readCPU(fp, CPUs, thread->CPUList);
            TL[n-1] = thread;
        }
    }
}

/*
 *      This function reads each process's detail
 *      from the file
 */

void readProcess(FILE *fp, int count, Process **PL, int tSwitch, int pSwitch){
    int n = 0, threads;
    while(n < count){
        Process * process = malloc(sizeof(Process));
        if(fscanf(fp, "%d %d", &n, &threads) != EOF){
            process->id = n;
            process->threadOH = tSwitch;
            process->processOH = pSwitch;
            process->threads = threads;
            process->threadList = malloc(sizeof(Thread*)*threads);
            /* printf("Process n: %d\n", process->id);
            printf("Process n threads: %d\n", process->threads); */
            readThread(fp, threads, process->threadList);
            PL[n-1] = process;
        }
    }
}

/*
 *      This function creates space for the Process
 *      List and then reads threads, and each bursts 
 *      from the file
 */

int readFile(FILE *fp, Process ***PL){
    int size, tSwitch, pSwitch;
    if(fp != NULL){
        if(fscanf(fp,"%d", &size)!=EOF){
            *PL = malloc(sizeof(Process *) *size);
        }
        if(fscanf(fp, "%d %d", &tSwitch, &pSwitch) != EOF){
            readProcess(fp, size, *PL, tSwitch, pSwitch);
        }
    }

    fclose(fp);
    return size;
}

