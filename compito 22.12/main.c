#include "header.h"

int main(){
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

    //THREADS PRODUTTORI
    pthread_t thread_prod[NUM_THREADS_PROD];

    //THREADS CONSUMATORI
    pthread_t thread_cons[NUM_THREADS_CONS];

    //Creazione istanza struttura monitor
    GestioneIO* pc = malloc(sizeof(GestioneIO));

    

    //PRODUTTORI
    int k;
    for(k=0;k<NUM_THREADS_PROD;k++){
        pthread_create(&thread_prod[k],&attr,Produci,(void*)pc);
    }

    //CONSUMATORI
    int i;
    for(i=0;i<NUM_THREADS_CONS;i++){
        pthread_create(&thread_prod[i],&attr,Consuma,(void*)pc);
    }

    //JOIN DEI THREADS
    for(k=0;k<NUM_THREADS_PROD;k++){
        pthread_join(thread_prod[k],NULL);
    }

    for(i=0;i<NUM_THREADS_CONS;i++){
        pthread_join(thread_cons[i],NULL);
    }

    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&pc->mutex);
     pthread_cond_destroy(&pc->ok_prod_cv);
     pthread_cond_destroy(&pc->ok_cons_cv);



}