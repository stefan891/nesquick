/// @file semaphore.c
/// @brief Contiene l'implementazione delle funzioni
///         specifiche per la gestione dei SEMAFORI.

#include "err_exit.h"
#include "semaphore.h"
#include <sys/sem.h>

union semun {
    int val;
    struct semid_ds * buf;
    unsigned short * array;
};     


void semOp (int semid, unsigned short sem_num, short sem_op) {
    struct sembuf sop= {.sem_num = sem_num, .sem_op = sem_op, .sem_flg = 0};

    if (semop(semid, &sop, 1) == -1)
        errExit("semop failed");
}

int CreateSemSet(){
    

}