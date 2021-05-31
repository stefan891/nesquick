#include <unistd.h> 
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>

#define MACCHINE 10
#define POSTIPISTA 5
#define MAX_WAIT 10

sem_t semaforo;

// Restituisce il valore del semaforo
int valoreSemaforo(void) {
    int valore = -1;
    sem_getvalue(&semaforo, &valore);
    return valore;
}

void *macchina(void *arg) {
    // Controlliamo il semaforo:
    // se è verde passiamo e ne decrementiamo il valore,
    // se è rosso attendiamo che diventi verde.
    printf("Macchina %d al semaforo, valore: %d\n", (int) arg, valoreSemaforo());
    sem_wait(&semaforo);
    printf("Macchina %d passata al semaforo, valore: %d\n", (int) arg, valoreSemaforo());
    
    // La macchina fa il proprio giro in una quantità di tempo casuale.
    sleep((rand() % MAX_WAIT)+1);

    // Esce dalla pista, incrementa il semaforo e sblocca
    // un altro thread, se ve ne sono in attesa.
    sem_post(&semaforo);
    printf("Macchina %d uscita dal circuito, valore: %d\n", (int) arg, valoreSemaforo());
    return NULL;
}

int main(int argc, char *argv[]) {
    int c1;
    pthread_t macchine[MACCHINE];
    
    // Inizializziamo il semaforo sui posti disponibili
    // in pista
    sem_init(&semaforo, 0, POSTIPISTA);
    
    // Facciamo entrare una macchina ogni secondo
    for(c1=0;c1<MACCHINE;c1++) {
        pthread_create(&macchine[c1], NULL, macchina, (void*) c1);
        sleep(1);
    }

    // Attendiamo la conclusione di tutti i thread
    for(c1=0;c1<MACCHINE;c1++)
        pthread_join(macchine[c1], NULL);

    return 0;
}
