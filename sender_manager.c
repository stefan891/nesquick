/// @file sender_manager.c
/// @brief Contiene l'implementazione del sender_manager.

#include "err_exit.h"
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
#include <stdio.h>

int fd;
ssize_t numWrite;
int PID;
int child;

int numCifre(pid_t pid) {
	int cifre=0;
	
	if(pid==0)
		return 1;
	else {
		while(pid!=0) {
			pid=pid/10;
			cifre++;
		}
	}
	return cifre;
}

void int2char(pid_t pid, int nCifre) {
	char car[nCifre];
	int temp;
	
	while(pid!=0) {
		temp=pid%10;
		nCifre--;
		car[nCifre] = (char) temp+48;
		pid=pid/10;
	}
		
	numWrite= write(fd, car, sizeof(car));
	if(numWrite != sizeof(car))
		ErrExit("Errore scrittura sul file F8");
}

int main(int argc, char * argv[]) {

	int nCifre;
	pid_t pid;
	
	//apertura file F8	
	fd = open("OutputFiles/F8.csv", O_RDWR | O_EXCL | O_TRUNC, S_IRUSR | S_IWUSR);
	if(fd == -1)
		ErrExit("Errore apertura file F8");
	
	//scrittura nel file F8 del titolo	
	char buf_titolo[] = "ID; PID";
	numWrite = write(fd, buf_titolo, sizeof(buf_titolo));
	if(numWrite != sizeof(buf_titolo))
		ErrExit("Errore scrittura sul file F8");
							
	//creazione 3 processi figli S1,S2,S3		
	for(child = 1; child < 4; child++) {
		pid = fork();

		if(pid == -1)  	//errore nella creazione del figlio
			ErrExit("Figlio non creato");
			
		if(pid == 0){
			printf("Child %d PID: %d PPID: %d Sleep: %d\n", child, getpid(), getppid(), child);
			if(child == 1) {
				//lettura file F0
				//scrittura file F1
			}
			else if(child == 2) {
				//scrittura file F2
			
			}
			else {
				//scrittura file F3
			}
			sleep(child);
			exit(0);
		}
		else {
			//aspetto la terminazione dei figli e torno al padre
			if((pid = wait(NULL)) != -1) {
				nCifre = numCifre(pid);
				if(child == 0) {
					char bufferS1[]= "\nS1; ";
					numWrite = write(fd, bufferS1, sizeof(bufferS1));
					if(numWrite != sizeof(bufferS1))
						ErrExit("Errore scrittura sul file F8");
					int2char(pid, nCifre);			
				}
				else if(child == 1) {
						char bufferS2[] = "\nS2; ";
						numWrite = write(fd, bufferS2, sizeof(bufferS2));
						if(numWrite != sizeof(bufferS2))
							ErrExit("Errore scrittura sul file F8");
						int2char(pid, nCifre);	
					}
					else {
						char bufferS3[] = "\nS3; ";
						numWrite = write(fd, bufferS3, sizeof(bufferS3));
						if(numWrite != sizeof(bufferS3))
							ErrExit("Errore scrittura sul file F8");
						int2char(pid, nCifre);
					}									
			}
		}
	}
	
    return 0;
}
