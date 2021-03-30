/// @file sender_manager.c
/// @brief Contiene l'implementazione del sender_manager.

#include "err_exit.c"
#include "defines.h"
#include "shared_memory.h"
#include "semaphore.h"
#include "fifo.h"
#include "pipe.h"

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>

#define MAX_READ 100
int main(int argc, char * argv[]) {

	int f8;
	
	//creazione 3 processi S1,S2,S3		
	for(int child=0; child<3; ++child) {
		pid_t pid = fork();
	//controllo se errore nella creazione del figlio
		if(pid == -1)
			printf("Figlio %d non creato", child);
	}

	//apertura file F8	
	f8=open("OutputFiles/F8.csv", O_RDWR, S_IRWXU | S_IROTH | S_IXOTH);
	if(f8==-1)
		ErrExit("Errore apertura file");
		
	char buffer[]= "Id; PID\n S1; 123\n S2; 124\n S3; 125\n";
	
	//scrittura nel file F8 dei PID dei tre processi
	ssize_t f8Write = write(f8, buffer, sizeof(buffer));
	if(f8Write != sizeof(buffer))
		ErrExit("Errore scrittura file f8");
		
	close(f8);
	
	int f0 = open("argv[1]", O_RDONLY, S_IRWXU | S_IROTH);
	if(f0==-1)
		ErrExit("Errore apertura file");
	
	char buffer1[MAX_READ];
	ssize_t f0Read= read(f0, buffer1, MAX_READ);
	if (f0Read == -1)
		ErrExit("Errore lettura file");
	
		
		
    return 0;
}
