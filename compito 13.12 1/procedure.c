#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

#include "header.h"

void inizializza(MonitorCoda *p) {

    /* TBD: Aggiungere codice per l'inizializzazione del monitor e delle relative
     * variabili per la sincronizzazione
     */
    init_monitor(&p->m,2);
    p->num_liberi = DIMENSIONE;
    p->num_occupati = 0;
    p->produttori_in_attesa = 0;

}

void rimuovi(MonitorCoda * p) {
    /* TBD: Aggiungere codice per la rimozione del monitor */
    remove_monitor(&(p->m));
}


int produzione(MonitorCoda *p, int valore) {

    /* TBD: Aggiungere codice per la produzione in accordo allo schema
     * con vettore di stato, ricordando che il valore di ritorno della funzione
     * indica se il monitor è sovraccarico o meno.
     * Nel caso in cui ci siano almeno 4 produttori in attesa, bisogna forzare
     * l'uscita del processo dal monitor e ritornare un valore pari ad 1.
     * Nel caso in cui la produzione avviene normalmente ritornare un valore pari a 0.
     */

    enter_monitor(&(p->m));
    if(p->num_liberi == 0){
        if(p->produttori_in_attesa < 4){
            (p->produttori_in_attesa)++;
            //printf("***** PRIMA Produttori in attesa: %d\n",p->produttori_in_attesa);
            wait_condition(&(p->m),VAR_COND_PROD);
            (p->produttori_in_attesa)--;
            //printf("***** DOPO Produttori in attesa: %d\n",p->produttori_in_attesa);
        }else{
            //printf("Sovraccarico, esco\n");
            leave_monitor(&(p->m));
            return 1;
        }
        
    }
    
    int i = 0;
    while(p->stato[i] != LIBERO && i<DIMENSIONE){
        i++;
    }
    p->stato[i] = IN_USO;
    (p->num_liberi)--;
    leave_monitor(&(p->m));

    p->vettore[i] = valore;
    

    enter_monitor(&(p->m));
    p->stato[i] = OCCUPATO;
    (p->num_occupati)++;
    printf("[%d] Numero occupati: %d\n",getpid(),p->num_occupati);
    signal_condition(&(p->m),VAR_COND_CONS);
    leave_monitor(&(p->m));
    return 0;
}


/* NOTA: il valore di ritorno di consumazione() è il valore prelevato dalla coda */

int consumazione(MonitorCoda *p) {


    /* TBD: Aggiungere codice per la consumazione in accordo allo schema
     * con vettore di stato, ricordando che il valore di ritorno della funzione
     * indica il valore prelevato dal buffer condiviso.
     */
    int val;
    enter_monitor(&(p->m));
    if(p->num_occupati == 0){
        //printf("///////// PRIMA CONSUMATORE IN ATTESA\n");
        wait_condition(&(p->m),VAR_COND_CONS);
        //rintf("///////// DOPO CONSUMATORE IN ATTESA\n");
    }
    
    int i = 0;
    while(p->stato[i] != OCCUPATO && i<DIMENSIONE){
        i++;
    }
    p->stato[i] = IN_USO;
    (p->num_occupati)--;
    leave_monitor(&(p->m));

    val = p->vettore[i];
    

    enter_monitor(&(p->m));
    p->stato[i] = LIBERO;
    (p->num_liberi)++;
    signal_condition(&(p->m),VAR_COND_PROD);
    leave_monitor(&(p->m));
    return val;
}

void produttore(MonitorCoda *m) {
    
    int valore;
    int ret;
    int i;
    
    for(i=0; i<5; i++) {
        
        while(1) {
            
            srand(time(NULL)*getpid());
            
            valore = rand() % 10;
            
            ret = produzione(m, valore);
            
            if(ret==0) { break; }
            
            printf("[%d] MONITOR SOVRACCARICO, ATTENDO 3 secondi...\n",getpid());
            
            sleep(3);
        }
        
        printf("[%d]VALORE PRODOTTO: %d\n",getpid(),valore);
        
        sleep(1);
    }
    
    
    exit(0);
}

void consumatore(MonitorCoda *m) {
    
    int i;
    int valore;
    
    for(i=0; i<15; i++) {
        
        valore = consumazione(m);
        
        printf("[%d] VALORE CONSUMATO: %d\n",getpid(),valore);
        
        sleep(2);
    }
    
    
    exit(0);
}

