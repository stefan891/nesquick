/// @file defines.h
/// @brief Contiene la definizioni di variabili
///         e funzioni specifiche del progetto.

#pragma once

#include <stdio.h>
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
#include <string.h>
#include <time.h>
#include "err_exit.h"
#include "semaphore.h"

#define F8 "OutputFiles/F8.csv"
#define F9 "OutputFiles/F9.csv"
#define F2 "OutputFiles/F2.csv"
#define F3 "OutputFiles/F3.csv"
#define F1 "OutputFiles/F1.csv"
#define F4 "OutputFiles/F4.csv"
#define F5 "OutputFiles/F5.csv"
#define F6 "OutputFiles/F6.csv"
#define F7out "OutputFiles/F7_out.csv"

#define MAX_LENGTH_SH 150

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

struct _elem{
    message_t Msg;
    struct _elem *next;
} ;
typedef struct _elem elem;

void SenderRead(char BUFFER_SZ[]);
void stampa(char []);
elem *inserisci_in_coda(elem *lista, message_t mess)