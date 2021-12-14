#include "header.h"

int main(){

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

    pthread_t capitreno[NUM_CAPITRENO];
    pthread_t viaggiatori[NUM_VIAGGIATORI];

    struct monitor_treno* treni[NUM_TRENI];
    int i;
    for(i=0;i<NUM_TRENI;i++){
        treni[i] = malloc(sizeof(struct monitor_treno));
        inizializza(treni[i]);
        treni[i]->ID = i;
    }

    //****AVVIO THREADS
    //CAPITRENO
    for(i=0;i<NUM_CAPITRENO;i++){
        pthread_create(&capitreno[i],&attr,start_capotreno,(void*)treni[i]);
    }

    //VIAGGIATORI
    int val;
    for(i=0;i<NUM_VIAGGIATORI;i++){
        srand(time(NULL)*i);
        val = rand()%4;
        pthread_create(&viaggiatori[i],&attr,start_viaggiatori,(void*)treni[val]);
    }

    //****JOIN
    //CAPITRENO
    for(i=0;i<NUM_CAPITRENO;i++){
        pthread_join(capitreno[i],NULL);
        printf("Thread capotreno n.%d terminato.\n",i);
    }

    //VIAGGIATORI
    for(i=0;i<NUM_VIAGGIATORI;i++){
        pthread_join(viaggiatori[i],NULL);
        printf("Thread viaggiatore n.%d terminato.\n",i);
    }


    for(i=0;i<NUM_TRENI;i++){
        rimuovi(treni[i]);
    }
    pthread_attr_destroy(&attr);
    pthread_exit(NULL);
    return 0;
}