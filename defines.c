/// @file defines.c
/// @brief Contiene l'implementazione delle funzioni
///         specifiche del progetto.

#include "defines.h"
#define COUNT count[y] 


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

    //return lista;
    stampa(MSG.mass);
    
}

void stampa(char mess[]){

    int i;
    printf("%s\n", mess);

}
elem *inserisci_in_coda(elem *lista, message_t mess){

    elem *prec;
    elem *tmp;
    tmp = (elem*)malloc(sizeof(elem));

    if(tmp != NULL){
        tmp -> next = NULL;
        tmp -> Msg = mess;

        if(lista == NULL)
            lista = tmp;
        
        else{
            for( prec=lista; prec->next!=NULL; prec= prec->next);
            prec->next = tmp;
        }
    }else
        printf("memoria esaurita");
    
    return lista;
}