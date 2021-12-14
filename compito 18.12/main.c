#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "header.h"

int coda_risultati;

#define NUM_THREADS_OPERANDI    2
#define NUM_THREADS_CALCOLO     3

int main(){
        pid_t pid;

        pthread_attr_t attr;
        pthread_t threads_operandi[NUM_THREADS_OPERANDI];
        pthread_t threads_risultati;
        pthread_t threads_calcolo[NUM_THREADS_CALCOLO];

        srand(time(NULL)*getpid());

        // Creazione coda risultati
        coda_risultati = msgget(IPC_PRIVATE,IPC_CREAT|0664);
        printf("Coda coda_risultati create con desc: %d\n", coda_risultati);
    
        initServiceQueues();
            
        //creazione di una istanza di struttura monitor per gli operandi
        // MonitorOperandi sarà usato dai thread generazione operandi
        MonitorOperandi * pc_op = malloc(sizeof(MonitorOperandi));
    
        //inizializzazione mutex e condition
        /* TBD */
        pthread_mutex_init(&pc_op->mutex,NULL);
        pthread_cond_init(&pc_op->ok_cons_cv,NULL);
        pthread_cond_init(&pc_op->ok_prod_cv,NULL);
    
        //inizializzazione delle variabili di stato
        pc_op->cons = 0;
        pc_op->prod = 0;
        pc_op->testa = 0;
        pc_op->coda = 0;
        pc_op->cont = 0;
        /* TBD */
    
        //impostazione dei thread come joinable
        /* TBD */
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
        // genero processo prelievo risultati
        pid = fork();
        
        /* TBD */
        if(pid == 0){
            //PROCESSO PRELEVATORE
            preleva_risultati(coda_risultati);
        }
        // genero thread generazione operandi
        /* TBD */
        for(int i=0;i<NUM_THREADS_OPERANDI;i++){
            pthread_create(&threads_operandi[i],&attr,genera_operandi,(void*)pc_op);
        }
        // genero thread di calcolo
        /* TBD */
        for(int i=0;i<NUM_THREADS_CALCOLO;i++){
            pthread_create(&threads_calcolo[i],&attr,calcola,(void*)pc_op);
        }


        /* TBD:  Join threads a wait processo*/
        int status;
        for(int i=0;i<NUM_THREADS_OPERANDI;i++){
            pthread_join(threads_operandi[i],(void**)&status);
            printf("Thread GENERATORE %d terminato con stato <%d>\n",i,status);
        }

        for(int i=0;i<NUM_THREADS_CALCOLO;i++){
            pthread_join(threads_calcolo[i],(void**)&status);
            printf("Thread CALCOLATORI %d terminato con stato <%d>\n",i,status);
        }

        pid = wait(&status);
        if(pid<0){
            perror("WAIT ERROR\n");
            exit(1);
        }
        printf("Processo PRELEVATORE terminato con stato <%d>\n",status);

        /*deallocazione risorse*/	
        /* TBD */
        pthread_mutex_destroy(&pc_op->mutex);
        pthread_cond_destroy(&pc_op->ok_cons_cv);
        pthread_cond_destroy(&pc_op->ok_prod_cv);

        free(pc_op);
        pthread_attr_destroy(&attr);
        msgctl(coda_risultati,IPC_RMID,0);

        pthread_exit(NULL);
}

