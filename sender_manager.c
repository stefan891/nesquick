/// @file sender_manager.c
/// @brief Contiene l'implementazione del sender_manager.

#include "err_exit.h"
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define COSTANTE 53

//funzioni 

int contaCifre(pid_t);
void stampaCifre(pid_t, int);
void stampaCifraUno();
void stampaCifraDue();
void stampaCifraTre();
void creaF1();
void creaF2();
void creaF3();
void stampaPID(pid_t);

//variabili globali

int fd0, fd1, fd2, fd3, fd8;
ssize_t numWrite, numRead;

char intestazione[48] = {"Id;Id Sender;Id Receiver;Time Arrival;Time dept."};
char intestazioneFileTrafficInfo[56] = {"Id;Message;Id Sender;Id Receiver;Time Arrival;Time dept."};
int child;
int num;


int main(void) {

	pid_t pid[3];
	int cifre;
	int i = 0;
	int lunghezzaMessage = 0;
	int lunghezzaDelS1 = 0;
	int lunghezzaDelS2 = 0;
	int lunghezzaDelS3 = 0;
	int lunghezzaType = 0;
	char buffer[14] = "Sender ID;PID\n";
	printf("Sono il processo padre(Sender manager)n PID: %d\n", getpid());

	//apro il file F8
	fd8 = open("OutputFile/F8.csv", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IROTH);
	if(fd8 == -1){
		ErrExit("open");
	}
	//stampa intestazione file F8
	numWrite = write(fd8, buffer, sizeof(buffer));
	if(numWrite != sizeof(buffer)){
		ErrExit("write");
	}

	//Creazione dei processi figli S1, S2, S3

	for(child = 0; child < 3; child++){

		pid[child] = fork();

		if(pid[child] == -1){
			ErrExit("fork");
		}
		else if(pid[child] == 0){

			printf("Child %d PID: %d PPID: %d\n", child + 1, getpid(), getppid());
			if(child == 0){

				//Definisco la struttura
				typedef struct{
					char Id[3];
					char Message[50];
					char IdSender[3];
					char IdReceiver[3];
					char DelS1[4];
					char DelS2[4];
					char DelS3[4];
					char Type[5];
				}messaggio_t;
				messaggio_t Msg;


				//Apro il file F0.csv
				//intestazione file F0.csv --> 53 caratteri
				fd0 = open("InputFile/F0.csv",  O_RDWR | O_EXCL, S_IRUSR | S_IWUSR);
				if(fd0 == -1){
					ErrExit("open");
				}
				//creo il file F1
				creaF1();
				//Posiziono il puntatore alla fine dell'intestazione
				off_t puntatore = lseek(fd0, COSTANTE, SEEK_SET);
				//Leggo ID del primo messaggio
				char arrayId[3];

				ssize_t numReadId = read(fd0, arrayId, sizeof(arrayId));
				if(numReadId != sizeof(arrayId)){
					ErrExit("read");
				}
				arrayId[numReadId] = '\0';
				//printf("arrayId: %s\n", arrayId);
				strcpy(Msg.Id, arrayId);
				printf("Msg.Id: %s\n", Msg.Id);
				//numReadId indica la grandezza del primo array della struttura
				//Leggo il messaggio
				char arrayMessage[50 + 1];
				puntatore = lseek(fd0, COSTANTE + 3, SEEK_SET);
				ssize_t numReadMessage = read(fd0, arrayMessage, sizeof(arrayMessage));
				if(numReadMessage != sizeof(arrayMessage)){
					ErrExit("read");
				}
				arrayMessage[numReadMessage] = '\0';
				//la variabile i contiene la lunghezza del messaggio
				for(i = 0; arrayMessage[i] != ';'; i++);
				arrayMessage[i] = ';';
				arrayMessage[i+1] = '\0';
				//Salvo la lunghezza del messaggio
				lunghezzaMessage = i + 1;
				char message[lunghezzaMessage];
				for(i = 0; i < lunghezzaMessage + 1; i++){
					message[i] = arrayMessage[i];
				}
				strcpy(Msg.Message, arrayMessage);
				printf("Msg.Message: %s\n", Msg.Message);
				//Trovo l'id sender
				char idSender[3];
				puntatore = lseek(fd0, COSTANTE + numReadId + lunghezzaMessage, SEEK_SET);
				ssize_t numReadIdSender = read(fd0, idSender, sizeof(idSender));
				if(numReadIdSender != sizeof(idSender)){
					ErrExit("read");
				}
				printf("idSender[0] = %c", )
				//numReadIdSender è la lunghezza dell'array
				idSender[numReadIdSender] = '\0';
				printf("\nIdSender: %s\n", idSender);					strcpy(Msg.IdSender, idSender);
				printf("Msg.IdSender: %s\n", Msg.IdSender);
				//Trovo l'id Receiver
				char idReceiver[3];
				puntatore = lseek(fd0, COSTANTE + numReadId + lunghezzaMessage + numReadIdSender + 3, SEEK_SET);
				ssize_t numReadIdReceiver = read(fd0, idReceiver, sizeof(idReceiver));
				if(numReadIdReceiver != sizeof(idReceiver)){
					ErrExit("read");
				}
				//numReadIdReceiver contine la lunghezza dell'array;
				idReceiver[numReadIdReceiver] = '\0';
				strcpy(Msg.IdReceiver, idReceiver);
				printf("Msg.IdReceiver: %s\n", Msg.IdReceiver);

				//Salvo i ritardi DelS1, DelS2, DelS2
				char supportoDelS1[4];
				char supportoDelS2[4];
				char supportoDelS3[4];
				char supportoType[5];
				
				puntatore = lseek(fd0, COSTANTE  + numReadId + lunghezzaMessage + numReadIdSender + numReadIdReceiver , SEEK_SET);

				ssize_t numReadDelS1 = read(fd0, supportoDelS1, sizeof(supportoDelS1));
				if(numReadDelS1 != sizeof(supportoDelS1)){
					ErrExit("read");
				}
				supportoDelS1[numReadDelS1] = '\0';
				//printf("SupportoDelS1: %s\n", supportoDelS1);
				for(i = 0; supportoDelS1[i] != ';'; i++);
				supportoDelS1[i] = '\0';
				//Salvo la lunghezza del messaggio
				lunghezzaDelS1 = i;
				//Dichiaro l'array dove salvo il delS1
				//lunghezzaDelS1 contiene la grandezza corretta dell'array
				//printf("Lungheza DelS1: %d\n", lunghezzaDelS1);
				char DelS1[lunghezzaDelS1];
				for(i = 0; i < lunghezzaDelS1; i++){
					DelS1[i] = supportoDelS1[i];
				}
				//printf("DelS1: %s\n", DelS1);
				//DelS1 contiene il ritardo
				strcpy(Msg.DelS1, DelS1);
				printf("Msg.DelS1: %s\n", Msg.DelS1);

				//Salvo il ritardo DelS2
				puntatore = lseek(fd0, COSTANTE  + numReadId + lunghezzaMessage + numReadIdSender + numReadIdReceiver + lunghezzaDelS1 , SEEK_SET);

				ssize_t numReadDelS2 = read(fd0, supportoDelS2, sizeof(supportoDelS2));
				if(numReadDelS2 != sizeof(supportoDelS2)){
					ErrExit("read");
				}
				supportoDelS1[numReadDelS2] = '\0';
				//printf("SupportoDelS2: %s\n", supportoDelS2);
				for(i = 0; supportoDelS2[i] != ';'; i++);
				supportoDelS2[i] = '\0';
				//Salvo la lunghezza del messaggio
				lunghezzaDelS2 = i;
				//Dichiaro l'array dove salvo il delS1
				//lunghezzaDelS2 contiene la grandezza corretta dell'array
				//printf("Lungheza DelS2: %d\n", lunghezzaDelS2);
				char DelS2[lunghezzaDelS2];
				for(i = 0; i < lunghezzaDelS2; i++){
					DelS2[i] = supportoDelS2[i];
				}
				strcpy(Msg.DelS2, DelS2);
				printf("Msg.DelS2: %s\n", Msg.DelS2);
				//printf("DelS2: %s\n", DelS2);
				//delS2 contiene il ritardo

				//DelS3
				puntatore = lseek(fd0, COSTANTE  + numReadId + lunghezzaMessage + numReadIdSender + numReadIdReceiver + lunghezzaDelS1 + lunghezzaDelS2, SEEK_SET);

				ssize_t numReadDelS3 = read(fd0, supportoDelS3, sizeof(supportoDelS3));
				if(numReadDelS3 != sizeof(supportoDelS3)){
					ErrExit("read");
				}
				supportoDelS1[numReadDelS3] = '\0';
				//printf("supportoDelS3: %s\n", supportoDelS3);
				for(i = 0; supportoDelS3[i] != ';'; i++);
				supportoDelS3[i] = '\0';
				//Salvo la lunghezza del messaggio
				lunghezzaDelS3 = i;
				//Dichiaro l'array dove salvo il delS1
				//lunghezzaDelS1 contiene la grandezza corretta dell'array
				char DelS3[lunghezzaDelS3];
				//printf("Lunghezza DelS3: %d\n", lunghezzaDelS3);
				for(i = 0; i < lunghezzaDelS3; i++){
					DelS3[i] = supportoDelS3[i];
				}
				strcpy(Msg.DelS3, DelS3);
				printf("Msg.DelS3: %s\n", Msg.DelS3);
				//printf("DelS3: %s\n", DelS3);
				//DelS3 contiene il ritardo

				//Trovo il type
				
				puntatore = lseek(fd0, COSTANTE + lunghezzaMessage + numReadIdSender + numReadIdReceiver + lunghezzaDelS1 + lunghezzaDelS2 + lunghezzaDelS3 + 12, SEEK_SET);
				ssize_t numReadType = read(fd0, supportoType, sizeof(supportoType));
				if(numReadType != sizeof(supportoType)){
					ErrExit("read");
				}
				supportoType[numReadType] = '\0';
				for(i = 0; supportoType[i] != '\n'; i++);
				//lunghezza type
				lunghezzaType = i + 1;

				char type[lunghezzaType];
				for(i = 0; i < lunghezzaType; i++){
					type[i] = supportoType[i];
				}
				strcpy(Msg.Type, type);
				printf("Msg.Type: %s\n", type);


				




				//Stampa su file

				//stampo l'id
				numWrite = write(fd1, arrayId, sizeof(arrayId));
				if(numWrite != sizeof(arrayId)){
					ErrExit("write");
				}
				//stampo il messaggio
				numWrite = write(fd1, Msg.Message, sizeof(Msg.Message));
				if(numWrite != sizeof(Msg.Message)){
					ErrExit("write");
				}
				//stampo l'id sender
				numWrite = write(fd1, idSender, sizeof(idSender));
				if(numWrite != sizeof(idSender)){
					ErrExit("write");
				}
				//stampo l'id Receiver
				numWrite = write(fd1, idReceiver, sizeof(idReceiver));
				if(numWrite != sizeof(idReceiver)){
					ErrExit("write");
				}
				//stampo il delS1
				numWrite = write(fd1, DelS1, sizeof(DelS1));
				if(numWrite != sizeof(DelS1)){
					ErrExit("write");
				}
				//stampo il delS2
				numWrite = write(fd1,DelS2, sizeof(DelS2));
				if(numWrite != sizeof(DelS2)){
					ErrExit("write");
				}
				//stampo il DelS3
				numWrite = write(fd1, Msg.DelS3, sizeof(Msg.DelS3));
				if(numWrite != sizeof(Msg.DelS3)){
					ErrExit("write");
				}
				//Stampo il type
				numWrite = write(fd1, type, sizeof(type));
				if(numWrite != sizeof(type)){
					ErrExit("write");
				}

			}
			else if(child == 1){
				creaF2();
			}
			else{
				creaF3();
			}
			exit(0);
		}
		else{

			if((pid[child] = wait(NULL)) != -1){

				cifre = contaCifre(pid[child]);
				if(child == 0){

					stampaCifraUno();
					stampaCifre(pid[child], cifre);

				}
				else if(child == 1){
					
					stampaCifraDue();			
					stampaCifre(pid[child], cifre);
					
				}
				else{

					stampaCifraTre();
					stampaCifre(pid[child], cifre);
				}
			}
		}
	}


	close(fd0);
	close(fd1);
	close(fd2);
	close(fd3);
	close(fd8);

	return 0;
}

int contaCifre(pid_t a){

	int count  = 0;

	if(a == 0){
		return 1;
	}
	else{
		while(a != 0){
			a = a/10;
			count++;
		}
	}
	return count;
}
void stampaCifre(pid_t pid, int cifre){

	char caratteri[cifre];
	int tmp;

	while(pid != 0){

		tmp = pid % 10;
		cifre--;
		caratteri[cifre] = (char) tmp + 48;
		pid = pid / 10;

	}
	//stampo sul file F8.csv il pid
	numWrite = write(fd8, caratteri, sizeof(caratteri));
	if(numWrite != sizeof(caratteri)){
		ErrExit("write");
	}
}
void stampaCifraUno(){

	char uno[2] = {"1;"};
	
	numWrite = write(fd8, uno, sizeof(uno));
	if(numWrite != sizeof(uno)){
		ErrExit("write");
	}
}
void stampaCifraDue(){

	char due[3] = {"\n2;"};
	
	numWrite = write(fd8, due, sizeof(due));
	if(numWrite != sizeof(due)){
		ErrExit("write");
	}
}
void stampaCifraTre(){

	char tre[3] = {"\n3;"};
	
	numWrite = write(fd8, tre, sizeof(tre));
	if(numWrite != sizeof(tre)){
		ErrExit("write");
	}
}
void creaF1(){


	fd1 = open("F1.csv", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IROTH);
	if(fd1 == -1){
		ErrExit("open");
	}
	numWrite = write(fd1, intestazioneFileTrafficInfo, sizeof(intestazioneFileTrafficInfo));
	if(numWrite != sizeof(intestazioneFileTrafficInfo)){
		ErrExit("write");
	}

}
void creaF2(){

	fd2 = open("F2.csv", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IROTH);
	if(fd2 == -1){
		ErrExit("open");
	}
	numWrite = write(fd2, intestazioneFileTrafficInfo, sizeof(intestazioneFileTrafficInfo));
	if(numWrite != sizeof(intestazioneFileTrafficInfo)){
		ErrExit("write");
	}
}
void creaF3(){

	fd3 = open("F3.csv", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IROTH);
	if(fd3 == -1){
		ErrExit("open");
	}
	numWrite = write(fd3, intestazioneFileTrafficInfo, sizeof(intestazioneFileTrafficInfo));
	if(numWrite != sizeof(intestazioneFileTrafficInfo)){
		ErrExit("write");
	}
}