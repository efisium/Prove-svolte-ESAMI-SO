#include "header.h"

void inizializza(struct monitor_treno* m){
    
    pthread_mutex_init(&m->mutex,NULL);
    pthread_cond_init(&m->cv_lettori,NULL);
    pthread_cond_init(&m->cv_scrittori,NULL);
    m->num_lettori = 0;
    m->num_scrittori = 0;
    m->stazione = 0;
}

void rimuovi(struct monitor_treno* m){
    pthread_cond_destroy(&m->cv_lettori);
    pthread_mutex_destroy(&m->mutex);
    free(m);
    
}
int leggi_stazione(struct monitor_treno* m){
    pthread_mutex_lock(&m->mutex);
    if(m->num_scrittori>0){
        pthread_cond_wait(&m->cv_lettori,&m->mutex);
    }
    (m->num_lettori)++;
    pthread_mutex_unlock(&m->mutex);

    printf("*VIAGGIATORE* :TRENO[%d] in stazione <%d>\n",m->ID,m->stazione);
    
    pthread_mutex_lock(&m->mutex);
    (m->num_lettori)--;
    if(m->num_lettori == 0){
        pthread_cond_signal(&m->cv_scrittori);
    }
    pthread_mutex_unlock(&m->mutex);
}
void scrivi_stazione(struct monitor_treno*m, int stazione){
    pthread_mutex_lock(&m->mutex);
    if(m->num_lettori>0){
        pthread_cond_wait(&m->cv_scrittori,&m->mutex);
    }
    (m->num_scrittori)++;
    pthread_mutex_unlock(&m->mutex);

    m->stazione = stazione;
    printf("*CAPOTRENO* :TRENO[%d] in viaggio verso stazione <%d>\n",m->ID,m->stazione);
    
    pthread_mutex_lock(&m->mutex);
    (m->num_scrittori)--;
    pthread_cond_broadcast(&m->cv_lettori);
    pthread_mutex_unlock(&m->mutex);
}

void* start_capotreno(void* g){
    struct monitor_treno* m = (struct monitor_treno*) g;
    printf("AVVIO TRHEAD CAPOTRENO...\n");
    int staz;
    for(int i=0;i<10;i++){
        sleep(3);
        staz = m->stazione + 1;
        scrivi_stazione(m,staz);
    }
    pthread_exit(NULL);
}
void* start_viaggiatori(void* g){
    struct monitor_treno* m = (struct monitor_treno*) g;
    printf("AVVIO TRHEAD VIAGGIATORE...\n");
    int staz;
    int var;
    for(int i=0;i<3;i++){
        srand(time(NULL)*i);
        var = 1+rand()%6;
        //var = 4;
        sleep(var);
        leggi_stazione(m);
    }
    pthread_exit(NULL);
}