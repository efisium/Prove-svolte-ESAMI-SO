#ifndef _HEAD_
#define _HEAD_

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <errno.h>
#include <pthread.h>

#define NUM_THREADS_PROD 4
#define NUM_THREADS_CONS  2

typedef struct{
    int indirizzo;
    int dato;
}Buffer;

typedef struct{
    Buffer vettori[10];

    //Variabili SINC - MUTEX
    pthread_mutex_t mutex;

    pthread_cond_t ok_prod_cv;
    pthread_cond_t ok_cons_cv;

    int ok_produzione;
    int ok_consumo;
}GestioneIO;

void Inizializza(GestioneIO* g);
void Produci(GestioneIO* g, Buffer* b);
void Consuma(GestioneIO* g, Buffer* b);

#endif