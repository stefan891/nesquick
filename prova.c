#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/dir.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <string.h>
#include "defines.h"

#define LENGTH puct-52
#define COUNT count[y]

void stampa(char []);
void SenderReader(char []);

typedef struct 
{
    char id [1];
    char mass [50];
    char id_sender [2];
    char id_rec [2];
    char S1 [4];
    char S2 [4];
    char S3 [4];
    char type [5];
} message_t;


int main(int argc, char *argv[]){

    // For each file provided as input argument
    for (int i = 1; i < argc; ++i) {
        // open the file in read only mode
        int fd0 = open( "F0.csv", O_RDONLY | O_EXCL, S_IRUSR | S_IWUSR); 
        // check error of open system call
        if (fd0 == -1) {
            printf("File %s does not exist\n", fd0);
            errExit("open");
            continue;
        }

    off_t puct = lseek(fd0, -1, SEEK_END);
    char buffer[LENGTH];
    off_t current = lseek(fd0, 53, SEEK_SET);
    ssize_t bR = 0;

    bR = read(fd0, buffer, LENGTH ); 



    close(fd0);

    return 0;


};

void SenderRead(char BUFFER_SZ[]){

    int y, x;           //contatori della posizione del buffer
    int count[7] = {0}; //contatore per riempire i vari campi del messaggio
    //elem *lista;      //dichairazione della lista dei messaggi
    message_t MSG;      //dichiarazione di quale messaggio stiamo parlando


   
    for(x=0; BUFFER_SZ[x] != '\0' ; x++){

        //se arrivo in una posizione del buffer in cui il carattere è ; allora 
        if(BUFFER_SZ[x] == ';' || BUFFER_SZ[x] == '\n'){
            if(BUFFER_SZ[x] == ';')
                y++;
            
            else if(BUFFER_SZ[x] == '\n')
                y=0;
            
            x++;
            COUNT = 0;
        }
        
        if(y==2)
            MSG.mass[count[1]+1] = '\0';
        switch(y){
            
            case 0:
                MSG.id[COUNT] = BUFFER_SZ[x];
                COUNT++;
                break;
            
            case 1:
                MSG.mass[COUNT]= BUFFER_SZ[x];
                COUNT++;
                break;
            case 2: 
                MSG.id_sender[COUNT] = BUFFER_SZ[x];
                COUNT++;
                break;
            case 3:
                MSG.id_rec[COUNT] = BUFFER_SZ[x];
                COUNT++;
                break;
            
            case 4:
                MSG.S1[COUNT] = BUFFER_SZ[x];
                COUNT ++;
                break;
            case 5:
                 MSG.S2[COUNT] = BUFFER_SZ[x];
                 COUNT++;
                 break;
            
            case 6: 
                MSG.S3[COUNT] = BUFFER_SZ[x];
                COUNT++;
                break;
            
            case 7:
                MSG.type[COUNT] = BUFFER_SZ[x];
                COUNT++;
                break;       
                
        }
    }

    printf("%s", MSG.mass);
}