#ifndef _HEAD_
#define _HEAD_

#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <sys/queue.h>
#include <sys/msg.h>
#include <sys/types.h>

#define NUM_CAPITRENO 4
#define NUM_VIAGGIATORI 10
#define NUM_TRENI 4

struct monitor_treno{
    int stazione;
    int ID;
    //VAR SYNCH
    pthread_mutex_t mutex;
    pthread_cond_t cv_lettori;
    pthread_cond_t cv_scrittori;

    int num_scrittori;
    int num_lettori;
};

void inizializza(struct monitor_treno* m);
void rimuovi(struct monitor_treno* m);
int leggi_stazione(struct monitor_treno* m);
void scrivi_stazione(struct monitor_treno*m, int stazione);
void* start_capotreno(void* g);
void* start_viaggiatori(void* g);
#endif 