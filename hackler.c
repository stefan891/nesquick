/// @file client.c
/// @brief Contiene l'implementazione del client.

#include "defines.h"
#include <string.h>


int main(int argc, char * argv[]) {

    int file = open(argv[1], O_RDONLY);
    
    if (file == -1){
        printf("File %s does not exist\n", argv[1]);
        continue;
    }


    
    return 0;
}
