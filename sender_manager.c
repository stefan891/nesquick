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
#include <sys/msg.h>
#include <string.h>

#define MESS 50
#define N 5
#define M 3

int fd8, fd0, fd1, fd2, fd3;
ssize_t numWrite, numRead;
int PID;
int child;
off_t current;

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
		
	numWrite= write(fd8, car, sizeof(car));
	if(numWrite != sizeof(car))
		ErrExit("Errore scrittura sul file F8");
}
							
int main(int argc, char * argv[]) {

	int nCifre;
	pid_t pid;
	char buffer_S[] = "Id | Message | Id Sender | Id Receiver | TimeArrival | TimeDeparture";
	int lenght_id=0;
	int lenght_mess=0;
	int lenght_idsender=0;
	int lenght_idreceiver=0;
	int lenght_s1=0;
	int lenght_s2=0;
	int lenght_s3=0;
	int lenght_type=0;
	int i, k;
	int capo=0;
	//apertura file F8	
	fd8 = open("OutputFiles/F8.csv", O_RDWR | O_EXCL | O_TRUNC, S_IRWXU | S_IROTH);
	if(fd8 == -1)
		ErrExit("Errore apertura file F8");
	
	//scrittura nel file F8 del titolo	
	char buf_titolo[] = "ID | PID";
	numWrite = write(fd8, buf_titolo, sizeof(buf_titolo));
	if(numWrite != sizeof(buf_titolo))
		ErrExit("Errore scrittura sul file F8");
							
	//creazione 3 processi figli S1,S2,S3		
	for(child = 1; child < 4; child++) {
		pid = fork();

		if(pid == -1) {  	//errore nella creazione del figlio
			ErrExit("Figlio non creato");
		}
		else if(pid == 0){ 
			printf("Child %d PID: %d PPID: %d Sleep: %d\n", child, getpid(), getppid(), child);
			//esecuzione S1
			if(child == 1) {  
				//apertura file F0 e F1
				fd0 = open("InputFiles/F0.csv", O_RDWR | O_EXCL, S_IRUSR | S_IWUSR);
				if(fd0 == -1)
					ErrExit("Errore apertura file F0");
				
				fd1 = open("OutputFiles/F1.csv", O_RDWR | O_EXCL | O_TRUNC, S_IRUSR | S_IWUSR);
				if(fd1 == -1)
					ErrExit("Errore apertura file F0");
				
				struct Message{
					char Id[M];
					char Message[MESS];
					char IdSender[M];
					char IdReceiver[M];
					char DelS1[N];
					char DelS2[N];
					char DelS3[N];
					char Type[N];
				};
				
				
				//scrittura nel file F1 dell'intestazione
				numWrite = write(fd1, buffer_S, sizeof(buffer_S));
				if(numWrite != sizeof(buffer_S))
					ErrExit("Errore scrittura sul file F1");
					
				//trovo la grandezza in byte di F0
				off_t current1 = lseek(fd0, -1, SEEK_END);
				
				//setto puntatore dopo l'intestazione
				int puntatore=53;
				char buff_f0[current1-puntatore];
				
				//conto il numero di Messaggi dentro F0
				current = lseek(fd0, puntatore, SEEK_SET);
				numRead = read(fd0, buff_f0, sizeof(buff_f0));
				if(numRead == -1)
					ErrExit("Errore lettura file F0");
				for(i=0; i<current1-puntatore; i++) {
					if(buff_f0[i]=='\n')
						capo++;
				}
				
				struct Message Mess[capo];
				
				for(k=0; k<capo; k++){
					//lettura id da F0
					current = lseek(fd0, puntatore, SEEK_SET);
					numRead = read(fd0, buff_f0, sizeof(buff_f0));
					if(numRead == -1)
						ErrExit("Errore lettura file F0");
					
					for(i=0; buff_f0[i]!= ';'; i++);
					buff_f0[i+1]='\0';
					lenght_id= i+1;
					strcpy(Mess[k].Id, buff_f0); 
					char buff_id[lenght_id];
					strcpy(buff_id, Mess[k].Id);
					
					//scrittura nel file F1 dell'ID
					numWrite = write(fd1, buff_id, sizeof(buff_id));
					if(numWrite != sizeof(buff_id))
						ErrExit("Errore scrittura sul file F1");
						
					puntatore = puntatore+lenght_id;
						
					//lettura Messaggio
					current = lseek(fd0, puntatore, SEEK_SET);
					numRead = read(fd0, buff_f0, sizeof(buff_f0));
					if(numRead == -1)
						ErrExit("Errore lettura file F0");
					
					for(i=0; buff_f0[i]!= ';'; i++);
					buff_f0[i+1]='\0';
					lenght_mess=i+1;
					strcpy(Mess[k].Message, buff_f0);	
					char buff_mess[lenght_mess];
					strcpy(buff_mess, Mess[k].Message);
					
					//scrittura nel file F1 del messaggio
					numWrite = write(fd1, buff_mess, sizeof(buff_mess));
					if(numWrite != sizeof(buff_mess))
						ErrExit("Errore scrittura sul file F1");
					
					puntatore=puntatore+lenght_mess;
					
					//lettura IdSender
					current = lseek(fd0, puntatore, SEEK_SET);
					numRead = read(fd0, buff_f0, sizeof(buff_f0));
					if(numRead == -1)
						ErrExit("Errore lettura file F0");
					
					for(i=0; buff_f0[i]!= ';'; i++);
					buff_f0[i+1]='\0';
					lenght_idsender=i+1;
					strcpy(Mess[k].IdSender, buff_f0);
					
					char buff_sender[lenght_idsender];
					strcpy(buff_sender, Mess[k].IdSender);
					
					//scrittura nel file F1 dell'idSender
					numWrite = write(fd1, buff_sender, sizeof(buff_sender));
					if(numWrite != sizeof(buff_sender))
						ErrExit("Errore scrittura sul file F1");
						
					puntatore=puntatore+lenght_idsender;
					
					//lettura IdReceiver
					current = lseek(fd0, puntatore, SEEK_SET);
					numRead = read(fd0, buff_f0, sizeof(buff_f0));
					if(numRead == -1)
						ErrExit("Errore lettura file F0");
					
					for(i=0; buff_f0[i]!= ';'; i++);
					buff_f0[i+1]='\0';
					lenght_idreceiver=i+1;
					strcpy(Mess[k].IdReceiver, buff_f0);
					char buff_receiver[lenght_idreceiver];
					strcpy(buff_receiver, Mess[k].IdReceiver);
					
					//scrittura nel file F1 dell'idReceiver
					numWrite = write(fd1, buff_receiver, sizeof(buff_receiver));
					if(numWrite != sizeof(buff_receiver))
						ErrExit("Errore scrittura sul file F1");
					puntatore= puntatore+lenght_idreceiver;
					
					//lettura DelS1
					current = lseek(fd0, puntatore, SEEK_SET);
					numRead = read(fd0, buff_f0, sizeof(buff_f0));
					if(numRead == -1)
						ErrExit("Errore lettura file F0");
					
					for(i=0; buff_f0[i]!= ';'; i++);
					buff_f0[i+1]='\0';
					lenght_s1=i+1;
					strcpy(Mess[k].DelS1, buff_f0);
					puntatore = puntatore+lenght_s1;
					
					//lettura DelS2
					current = lseek(fd0, puntatore, SEEK_SET);
					numRead = read(fd0, buff_f0, sizeof(buff_f0));
					if(numRead == -1)
						ErrExit("Errore lettura file F0");
					
					for(i=0; buff_f0[i]!= ';'; i++);
					buff_f0[i+1]='\0';
					lenght_s2=i+1;
					strcpy(Mess[k].DelS2, buff_f0);
					puntatore = puntatore+lenght_s2;
					
					//lettura DelS3
					current = lseek(fd0, puntatore, SEEK_SET);
					numRead = read(fd0, buff_f0, sizeof(buff_f0));
					if(numRead == -1)
						ErrExit("Errore lettura file F0");
					
					for(i=0; buff_f0[i]!= ';'; i++);
					buff_f0[i+1]='\0';
					lenght_s3=i+1;
					strcpy(Mess[k].DelS3, buff_f0);
					puntatore = puntatore+lenght_s3;
					
					//lettura Type
					current = lseek(fd0, puntatore, SEEK_SET);
					numRead = read(fd0, buff_f0, sizeof(buff_f0));
					if(numRead == -1)
						ErrExit("Errore lettura file F0");
					
					for(i=0; buff_f0[i]!= '\n'; i++);
					buff_f0[i]='\0';
					buff_f0[i+1]='\n';
					lenght_type=i;
					strcpy(Mess[k].Type, buff_f0);
					
					puntatore=puntatore+lenght_type;
					
				}

			}
			//esecuzione S2
			else if(child == 2) {  
				//scrittura nel file F2 dell'intestazione
				fd2 = open("OutputFiles/F2.csv", O_RDWR | O_EXCL | O_CREAT| O_TRUNC, S_IRUSR | S_IWUSR);
				if(fd2 == -1)
					ErrExit("Errore apertura file F2");
				
				numWrite = write(fd2, buffer_S, sizeof(buffer_S));
				if(numWrite != sizeof(buffer_S))
					ErrExit("Errore scrittura sul file F2");
				
				
			}
			//esecuzione S3
			else {
				//scrittura nel file F3 dell'intestazione
				fd3 = open("OutputFiles/F3.csv", O_RDWR | O_EXCL | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
				if(fd3 == -1)
					ErrExit("Errore apertura file F3");
				numWrite = write(fd3, buffer_S, sizeof(buffer_S));
				if(numWrite != sizeof(buffer_S))
					ErrExit("Errore scrittura sul file F3");
				
			}
			sleep(child);
			exit(0);
		}
		else {
			//aspetto la terminazione dei figli e torno al padre
			if((pid = wait(NULL)) != -1) {
				nCifre = numCifre(pid);
				if(child == 1) {
					char bufferS1[]= "\nS1; ";
					numWrite = write(fd8, bufferS1, sizeof(bufferS1));
					if(numWrite != sizeof(bufferS1))
						ErrExit("Errore scrittura sul file F8");
					int2char(pid, nCifre);			
				}
				else if(child == 2) {
						char bufferS2[] = "\nS2; ";
						numWrite = write(fd8, bufferS2, sizeof(bufferS2));
						if(numWrite != sizeof(bufferS2))
							ErrExit("Errore scrittura sul file F8");
						int2char(pid, nCifre);	
					}
					else {
						char bufferS3[] = "\nS3; ";
						numWrite = write(fd8, bufferS3, sizeof(bufferS3));
						if(numWrite != sizeof(bufferS3))
							ErrExit("Errore scrittura sul file F8");
						int2char(pid, nCifre);
					}									
			}
		}
	}
	close(fd2);
	close(fd3);
	close(fd8);
	exit(0);
    return 0;
}
