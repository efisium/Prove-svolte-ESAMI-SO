#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>

#include "header.h"

int main() {
    pid_t pid;
    int status;

    int shm_id = shmget(IPC_PRIVATE,sizeof(MonitorCoda),IPC_CREAT|0664);
    
    if (shm_id < 0)
    {
        perror("Errore creazione shared memory");
        exit(1);
    }
    
    MonitorCoda *p = shmat(shm_id,NULL,0);
    
    if (p == (void *)-1)
    {
        perror("Errore attach shared memory");
        exit(1);
    }

    /* TBD: Inizializzare monitor e variabili per la sincronizzazione tramite la procedura inizializza(...) */
    inizializza(p);

    /* TBD: Aggiungere codice per la creazione dei processi produttori e consumatori */
    //PRODUTTORI
    for(int i=0;i<NUM_PRODUTTORI;i++){
        pid = fork();
        if( pid == 0){
            //Processo Produttore
            printf("Avvio produttore %d...\n",getpid());
            produttore(p);
            perror("FORK ERROR\n");
            exit(1);
        }
    }

    //CONSUMATORI
    for(int i=0;i<NUM_CONSUMATORI;i++){
        pid = fork();
        if(pid == 0){
            //Processo Consumatore
            printf("Avvio consumatore %d...\n",getpid());
            consumatore(p);
            perror("FORK ERROR\n");
            exit(1);
        }
    }


    /* TBD: Aggiungere codice per l'attesa della terminazione dei processi produttori e consumatori */
    for(int i=0;i<NUM_CONSUMATORI+NUM_PRODUTTORI;i++){
        pid = wait(&status);
        if(pid<0){
            perror("WAIT ERROR\n");
            exit(1);
        }
        printf("Processo %d terminato con stato <%d>\n",pid,status);
    }

    /* TBD: Aggiungere codice per la rimozione del monitor tramite la procedura rimuovi(...) */
    rimuovi(p);
    shmctl(shm_id,IPC_RMID,0);
    return 0;
}

