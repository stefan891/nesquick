#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
int contaCifre(pid_t); //conta le cifre del pid di ogni figlio
void stampaCifre(pid_t, int); //stampo le cifre sul file F9.csv
void stampaCifraUno(); //Stampa la cifra 1 che indice il primo figlio
void stampaCifraDue(); //Stampa la cifra 2 che indica il secondo figlio
void stampaCifraTre(); //Stampa la cifra 3 che indica il terzo figlio
void creoF4();
void creoF5();
void creoF6();


//Variabili globali
int fd, fd4, fd5, fd6;
ssize_t numWrite;
char intestazione[56] = "Id | Id Sender | Id Receiver | Time arrival | Time dept.";
int numeroPid[3];
int child;
int num;
int main(){

	pid_t pid[3];
	char buffer[17] = "Receiver ID | PID";
	char buf[19] = "\n-----------------\n";
	char barraVerticale[1] = "|";
	int cifre; //salvo il numero delle cifre del PID
	printf("Sono il processo padre(Receiver manager) PID: %d\n", getpid());
	
	/*Creazione del file F9.csv per lettura/scrittura
	  File permission permissions read+write only for owner */
	fd = open("F9.csv", O_RDWR | O_CREAT | O_EXCL | O_TRUNC, S_IRUSR | S_IWUSR);
	if(fd == -1){
		printf("Errore nella creazione del file F9.csv");
	}
	/*Stampo l'intestazione del file F9.csv*/
	numWrite = write(fd, buffer, sizeof(buffer));
	if (numWrite != sizeof(buffer))
		printf("Errore nella scrittura su file F9.csv");
	/*Stampo i trattini*/
	numWrite = write(fd, buf, sizeof(buf));
	if (numWrite != sizeof(buf))
		printf("Errore nella scrittura su file F9.csv");
	
	//Creazione dei figli R1, R2, R3
	for(child = 0; child < 3; child++)
	{
		pid[child] = fork();
		if(pid < 0){
			printf("Errore");
		}
		if(pid[child] == 0){
			printf("Child %d PID: %d PPID: %d Sleep: %d\n", child, getpid(), getppid(), child + 1);
			numeroPid[child] = getpid();
			printf("CHILD: %d PID: %d\n", child, numeroPid[child]);
			if(child == 0){
				//Creo il file F6
				creoF6();
			}
			else if(child == 1){
				//Creo il file F5
				creoF5();
			}
			else{
				//Creo il file F4
				creoF4();
			}
			sleep(child+1);
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
	

	return 0;
}
int contaCifre(pid_t a){

	int count = 0;

	if(a == 0){
		return 1;
	}	
	else{

		while(a != 0){
			a = a / 10;
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

	numWrite = write(fd, caratteri, sizeof(caratteri));
	if(numWrite != sizeof(caratteri)){
		printf("Errore nella scrittura su file F9.csv");
	}
}
void stampaCifraUno(){
	char uno[14] = "      1     | ";
	numWrite = write(fd, uno, sizeof(uno));
	if(numWrite != sizeof(uno)){
		printf("Errore nella scrittura su file F9.csv");
	}
	
}
void stampaCifraDue(){
	char due[15] = "\n      2     | ";
	numWrite = write(fd, due, sizeof(due));
	if(numWrite != sizeof(due)){
		printf("Errore nella scrittura su file F9.csv");
	}
}
void stampaCifraTre(){
	char tre[15] = "\n      3     | ";
numWrite = write(fd, tre, sizeof(tre));
	if(numWrite != sizeof(tre)){
		printf("Errore nella scrittura su file F9.csv");
	}
	
}
void creoF4(){

	fd4 = open("F4.csv", O_RDWR | O_CREAT | O_EXCL | O_TRUNC, S_IRUSR | S_IWUSR);
	if(fd4 == -1){
		printf("Errore nella creazione del file F4.csv");
	}
	numWrite = write(fd4, intestazione, sizeof(intestazione));
	if(numWrite != sizeof(intestazione)){
		printf("Errore nella scrittura su file F4.csv");
	}
}
void creoF5(){

	fd5 = open("F5.csv", O_RDWR | O_CREAT | O_EXCL | O_TRUNC, S_IRUSR | S_IWUSR);
	if(fd5 == -1){
		printf("Errore nella creazione del file F5.csv");
	}
	numWrite = write(fd5, intestazione, sizeof(intestazione));
	if(numWrite != sizeof(intestazione)){
		printf("Errore nella scrittura su file F5.csv");
	}
}
void creoF6(){

	fd6 = open("F6.csv", O_RDWR | O_CREAT | O_EXCL | O_TRUNC, S_IRUSR | S_IWUSR);
	if(fd6 == -1){
		printf("Errore nella creazione del file F6.csv");
	}
	numWrite = write(fd6, intestazione, sizeof(intestazione));
	if(numWrite != sizeof(intestazione)){
		printf("Errore nella scrittura su file F6.csv");
	}
}