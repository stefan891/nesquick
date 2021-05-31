#include "err_exit.h"

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>

#define LENGTH punct-52
#define COUNT count[y]

typedef struct 
{
    char id [2];
    char mass [51];
    char id_sender [3];
    char id_rec [3];
    char S1 [5];
    char S2 [5];
    char S3 [5];
    char type [6];
} message_t;


int main(void) {

  //char buffer[]={'1',';', 'C', 'i', 'a', 'o', ';', 'S', '3', ';', 'R', '2',';' ,'2', ';', '5', ';', '3', ';', 'Q', '\0'};

    int y, x;           //contatori della posizione del buffer
    int count[7] = {0}; //contatore per riempire i vari campi del messaggio
    int fd0, fd1;
    //elem *lista;      //dichairazione della lista dei messaggi
    message_t MSG;      //dichiarazione di quale messaggio stiamo parlando


	fd0 = open("F0.csv", O_RDWR | O_EXCL, S_IRUSR | S_IWUSR);

	if(fd0 == -1)
	    ErrExit("Errore apertura file F0");

    fd1 = open("F1.csv", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IROTH);
	if(fd1 == -1)
		ErrExit("Errore apertura file F0");

    off_t current = lseek(fd0, -1, SEEK_END);

    char buffer[current];
    lseek(fd0, 54, SEEK_SET);
    ssize_t numRead = read(fd0, buffer, sizeof(buffer));

    for(x=0; buffer[x] != '\0' ; x++){

        //se arrivo in una posizione del buffer in cui il carattere è ; allora 
        if(buffer[x] == ';' || buffer[x] == '\n'){
            if(buffer[x] == ';')
                y++;
            
            else if(buffer[x] == '\n')
                y=0;
            
            x++;
            COUNT = 0;
        }

        switch(y){
            
            case 0:
                MSG.id[COUNT] = buffer[x];
                COUNT++;
                break;
            
            case 1:
                MSG.mass[COUNT]= buffer[x];
                COUNT++;
                break;
            case 2: 
                MSG.id_sender[COUNT] = buffer[x];
                MSG.mass[count[1]+1] = '\0';
                COUNT++;
                break;
            case 3:
                MSG.id_rec[COUNT] = buffer[x];
                MSG.id_sender[count[2]+1] = '\0';
                COUNT++;
                break;
            
            case 4:
                MSG.S1[COUNT] = buffer[x];
                MSG.id_rec[count[3]+1] = '\0';
                COUNT ++;
                break;
            case 5:
                 MSG.S2[COUNT] = buffer[x];
                 MSG.S1[count[4]+1] = '\0';
                 COUNT++;
                 break;
            
            case 6: 
                MSG.S3[COUNT] = buffer[x];
                MSG.S2[count[5]+1] = '\0';
                COUNT++;
                break;
            
            case 7:
                MSG.type[COUNT] = buffer[x];
                MSG.S3[count[6]+1] = '\0';
                COUNT++;
                break;       
                
        }
    }

    //return lista;
    printf("%s\n", MSG.mass);
    return 0;
    
}
  