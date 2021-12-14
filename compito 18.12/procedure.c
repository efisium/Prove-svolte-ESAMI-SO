#include "header.h"
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <string.h>

int coda_RTS;
int coda_OTS;
//int coda_risultati;

void initServiceQueues(){

    /* TBD: Inizializzazione code di servizio */
    
    coda_RTS = msgget(IPC_PRIVATE,IPC_CREAT|0664);
    printf("Coda di servizio coda_RTS create con desc: %d\n", coda_RTS);
    
    coda_OTS = msgget(IPC_PRIVATE,IPC_CREAT|0664);
    printf("Coda di servizio coda_OTS create con desc: %d\n", coda_OTS);
}

void removeServiceQueues(){
    
    /* TBD: rimozione code di servizio */
    msgctl(coda_RTS,IPC_RMID,0);
    msgctl(coda_OTS,IPC_RMID,0);
}

// Send Sincrona
void inserisci_risultato (Risultato *m , int queue){
    
    /* TBD: Implementare il protocollo per la send sincrona e inviare il messaggio
     * per l'inserimento del risultato
     */
    Risultato rqs_to_snd;
    Risultato ok_to_snd;
    rqs_to_snd.type = RQS_TO_SND;
    msgsnd(coda_RTS,&rqs_to_snd,sizeof(Risultato)-sizeof(long),0);
    msgrcv(coda_OTS,&ok_to_snd,sizeof(Risultato)-sizeof(long),OK_TO_SND,0);
    msgsnd(queue,m,sizeof(Risultato)-sizeof(long),0);
}

// Receive Bloccante
int preleva_risultato(Risultato *m, int queue, int tipomess){
    
    /* TBD: Implementare il protocollo per la send sincrona e ricevere il messaggio
     * per il prelevamento del risultato
     */
    Risultato rqs_to_snd;
    Risultato ok_to_snd;
    ok_to_snd.type = OK_TO_SND;
    msgrcv(coda_RTS,&rqs_to_snd,sizeof(Risultato)-sizeof(long),RQS_TO_SND,0);
    msgsnd(coda_OTS,&ok_to_snd,sizeof(Risultato)-sizeof(long),0);
    msgrcv(queue,m,sizeof(Risultato)-sizeof(long),tipomess,0);
    return (m->mess);
}

void inserisci_operando(MonitorOperandi * m, int operando){

    /* TBD: Implementare la sincronizzazione richiesta tramite monitor
     * gestendo opportunamente la produzione degli operandi
     */
    pthread_mutex_lock(&m->mutex);
    /*while(m->prod == 1 || m->cons == 1){
        pthread_cond_wait(&m->ok_prod_cv,&m->mutex);
    }*/
    if(m->cont == DIM_BUFFER){
        pthread_cond_wait(&m->ok_prod_cv,&m->mutex);
    }
    //(m->prod)++;
    
    //pthread_mutex_unlock(&m->mutex);

    m->operandi[m->testa] = operando;
    m->testa = (m->testa + 1) % DIM_BUFFER;
    (m->cont)++;

    /*pthread_mutex_lock(&m->mutex);
    (m->prod)--;*/
    pthread_cond_signal(&m->ok_cons_cv);
    pthread_mutex_unlock(&m->mutex);
}

int * preleva_operando(MonitorOperandi * m){

        static int operando[2];

        /* TBD: Implementare la sincronizzazione richiesta tramite monitor
         * gestendo opportunamente la consumazione degli operandi
         */
            pthread_mutex_lock(&m->mutex);
            /*while(m->prod == 1 || m->cons == 1){
                pthread_cond_wait(&m->ok_cons_cv,&m->mutex);
            }*/
            for(int i=0;i<2;i++){
                if(m->cont == 0){
                    //pthread_cond_signal(&m->ok_prod_cv);
                    //if(m->cont == 1) (m->cont)--;
                    pthread_cond_wait(&m->ok_cons_cv,&m->mutex);
                }
                /*if(m->cont == 0) (m->cont)++;
                pthread_mutex_unlock(&m->mutex);*/

                operando[i] = m->operandi[m->coda];
                m->coda = (m->coda + 1) % DIM_BUFFER;
                (m->cont)--;
            }
            /*pthread_mutex_lock(&m->mutex);
            (m->cons)--;*/
            pthread_cond_signal(&m->ok_prod_cv);
            pthread_mutex_unlock(&m->mutex);
            
            return operando;
}

void * genera_operandi(void *m){

        int i, op;

        MonitorOperandi * mon = (MonitorOperandi*)m;
    
        for (i=0; i<6; i++){
        
                // call inserisci_operando generato da 1 a 10
                op = 1 + rand() % 10;
                printf("[thread genera_operandi] Operando generato: %d\n", op);
                inserisci_operando(mon, op);
                 
        }
        pthread_exit(0);
}

void preleva_risultati(int coda_risultati){

        int i, risultato, less=0, great=0;
    
        Risultato m;
    
        for (i=0; i<6; i++){
        
            
                //risultato = preleva_risultato(mon);
                risultato = preleva_risultato(&m, coda_risultati, RISULTATO_TYPE);
                printf("[processo preleva_risultati] Risultato prelevato: %d coda: %d\n", risultato, coda_risultati);
            
                if (risultato<=25)
                    less++;
                else
                    great++;
                 
        }
    
        printf("[processo preleva_risultati] STATS: Conteggio minore uguale: risultato: %d\n", less);
        printf("[processo preleva_risultati] STATS: Conteggio maggiore: %d\n", great);
        exit(0);
}


void * calcola(void *mon){

        int i, calc;
        int *op;

        MonitorOperandi * mon_op = (MonitorOperandi*)mon;
    
        Risultato m; //messaggio da inviare
    
        for (i=0; i<2; i++){
        
                // preleva operando
                op = preleva_operando(mon_op);
                int op1 = *op;
                int op2 = *(op+1);
                printf("[thread calcola] Preleva op1: %d op2: %d\n", op1, op2);

                // calcola
                calc = (op1*op1) + (op2*op2);

                printf("[thread calcola] Calcola somma dei quadrati (%d x %d) + (%d x %d) = %d\n", op1, op1, op2, op2, calc);
                // inserisci risultato
            
                printf("[thread calcola] Inserisci risultato: %d tramite send sincrona su coda: %d\n", calc, coda_risultati);
                sleep(1);
            
                /* TBD: Impostare il messaggio da inviare */
                m.type = RISULTATO_TYPE;
                m.mess = calc;
                //printf("VALORE CALC: %s\n",m.mess);
                //m.mess = (char)calc;
                inserisci_risultato(&m , coda_risultati);
        }
    
        pthread_exit(0);
}
