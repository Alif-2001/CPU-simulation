#include "thread.h"
#include "parser.h"

/*
 *      Title: simcpu.c
 *      Author: Alif Merchant
 *      Date: Thursday, March 5, 2021  
 */

void parseComands(char **argv, int argc, int* d, int* v, int* r, int* quant);
void freeProcessList(Process **PL, int size);

int main(int argc, char **argv) {
    
    int d = 0, v = 0, r = 0, quant = 0;
    parseComands(argv, argc, &d, &v , &r, &quant);

    if(r && quant <= 0){
        printf("Invalid Time Quantum, Exiting program.\n");
        return 0;
    }

    if(v){
        d = 1;
    }

    FILE *fp = stdin;
    Process **PL = NULL;
    int size = readFile(fp, &PL);

    if(r && quant > 0){
        printf("Running RR\n");
        rr(PL,size, quant, d ,v);
    }else{
        printf("Running FCFS\n");
        fcfs(PL,size, d, v);
    }

    freeProcessList(PL, size);
    //Make sure to free everything

    return 0;
}

/* 
 *      parse comand line arguments. (ex: -d -v -r 50)   
 */
void parseComands(char **argv, int argc, int* d, int* v, int* r, int* quant){
    int i = 1;
    while(i < argc){
        int j = 1;
        while(argv[i][j] != '\0'){
            switch (argv[i][j]){
                case 'd':
                    *d = 1;
                break;
                case 'v':
                    *v = 1;
                break;
                case 'r':
                    *r = 1;
                    if(argv[i+1]){
                        *quant = atoi(argv[argc-1]);
                    }else{
                        printf("Missing Time Quantum\n");
                    }
                break;
            
                default:
                break;
            }
            j++;
        }
        i++;
    }
}

/* 
 *      free all the burst, thread and process items inside the 
 *      process list   
 */
void freeProcessList(Process **PL, int size){
    for(int i = 0; i < size; i++){
        Process * process = PL[i];
        for(int j = 0; j < process->threads; j++){
            Thread * thread = process->threadList[j];
            for(int k = 0; k < thread->CPUs; k++){
                CPU * cpu = thread->CPUList[k];
                free(cpu);
            }
            free(thread->CPUList);
            free(thread);
        }
        free(process->threadList);
        free(process);
    }
    free(PL);
}
