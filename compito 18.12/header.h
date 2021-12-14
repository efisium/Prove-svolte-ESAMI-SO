#include <pthread.h>

#ifndef HEADER_H
#define HEADER_H

#define DIM_BUFFER 5
#define RISULTATO_TYPE 1
#define RQS_TO_SND 2
#define OK_TO_SND 3
/* TBD: Definire le macro per la comunicazione su coda sincrona */

extern int coda_RTS;
extern int coda_OTS;
extern int coda_risultati;

//typedef char mess_t[3];
typedef struct {
    
    // pool di buffer condivisi
    int operandi[DIM_BUFFER];
    int testa;
    int coda;
    int cont;
    
    /* TBD: Aggiungere le variabili necessarie per la sincronizzazione */
    pthread_mutex_t mutex;
    pthread_cond_t ok_prod_cv;
    pthread_cond_t ok_cons_cv;

    int prod;
    int cons;
} MonitorOperandi;

typedef struct {
    /* TBD: Definire la struttura messaggio */
    long type;
    int mess;
} Risultato;

void inserisci_operando(MonitorOperandi * mo, int operando);
int *preleva_operando(MonitorOperandi * mo);

void inserisci_risultato(Risultato *m , int queue);
int preleva_risultato(Risultato *m, int queue, int tipomess);

void * genera_operandi(void *);
void * calcola(void *);
void preleva_risultati(int);

void initServiceQueues();
void removeServiceQueues();
    
#endif

