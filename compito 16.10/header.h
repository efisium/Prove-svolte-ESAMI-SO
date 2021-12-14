#ifndef _HEAD_
#define _HEAD_

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <errno.h>
#include <pthread.h>

#define N_THREADS 5
#define LEN_VETT 5

typedef struct{
    int a;
    int b;
}elemento;

typedef struct{
    elemento vettore[LEN_VETT];
    int testa;
    int coda;
    int cont;

    pthread_mutex_t mutex;
    pthread_cond_t ok_prod_cv;
    pthread_cond_t ok_cons_cv;

    int prod;
    int cons;

}MonitorVettore;

typedef struct{
    elemento buffer;
    MonitorVettore *m;

    pthread_mutex_t mutex;
    pthread_cond_t ok_lett_cv;
    pthread_cond_t ok_scritt_cv;

    int lettori;
    int scrittori;
    int lett_wait;
    int scritt_wait;


}MonitorBuffer;

void genera(MonitorVettore *v, elemento e);
elemento preleva(MonitorVettore *v);
void aggiorna(MonitorBuffer *b, elemento e);
void consulta(MonitorBuffer *b);
void* startGeneratore(void *g);
void* startAggiornatore(void *g);
void* startDestinatario(void *g);
void inizializza(MonitorBuffer *b, MonitorVettore *v);
void rimuovi(MonitorBuffer *b, MonitorVettore *v);

#endif