#ifndef HEADER_H
#define HEADER_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>



struct monitor {

	int molo;
	int id_nave;

	/* TBD: Aggiungere variabili per la sincronizzazione */
    pthread_mutex_t mutex;

    pthread_cond_t ok_lett_cv;
    pthread_cond_t ok_scritt_cv;

    int num_lettori;	// lettori attivi
	int num_scrittori;	// scrittori attivi
	int num_lettori_wait;	// lettori in attesa
	int num_scrittori_wait;	// scrittori in attesa
};

void inizializza(struct monitor * m);
void rimuovi (struct monitor * m);
void scrivi_molo(struct monitor * m, int molo);
int leggi_molo(struct monitor * m);



#endif