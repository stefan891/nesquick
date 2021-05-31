/// @file client.c
/// @brief Contiene l'implementazione del client.

#include 
#include "defines.h"


#define N 20
#define BUFFER_SZ 100

typedef struct{
    int Id;
    int Delay;
    char Target[N];
    char Action[N];
} ;

char buffer[BUFFER_SZ + 1];

int main(int argc, char * argv[]) {

        char *F7;

    F7 = argv[1];

    if(argc < 1){
    exit(1);
  }

    int fileS = open(fileSource, O_RDONLY);
    
    if (fileS == -1){
        errExit("open failed");
    }

    int fileD = -1;
    
    if (access(fileDestination, F_OK)==)



   /* ssize_t bR = 0;

    do{
        bR = read(file, buffer, BUFFER_SZ);

        if(bR > 0){

            buffer[bR] = '\0';
            printf("%s", buffer);

        }
    }while (bR > 0);

    close(file);*/




    
    return 0;
}
