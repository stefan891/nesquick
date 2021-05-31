/// @file shared_memory.c
/// @brief Contiene l'implementazione delle funzioni
///         specifiche per la gestione della MEMORIA CONDIVISA.

#include "err_exit.h"
#include "shared_memory.h"
#include "defines.h"
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>

int main (){

    int shmid;          //id della shared mamory
    
    //creazione della chiave da associare alla shared memory

    //creazione della shared mamory, se già esistente con questa chiave allora torna errore
    shmid = shmget( IPC_PRIVATE, MAX_LENGTH_SH, S_IRUSR | S_IWUSR);

    

    


}
