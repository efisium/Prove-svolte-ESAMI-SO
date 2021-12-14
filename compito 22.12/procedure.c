#include "header.h"

void Inizializza(GestioneIO* pc){
    //INIT MUTEX E VARCOND
    pthread_mutex_init(&pc->mutex,NULL);
    pthread_cond_init(&pc->ok_prod_cv,NULL);
    pthread_cond_init(&pc->ok_cons_cv,NULL);

    //INIT VAR. STATO
    pc->ok_produzione = 1;
    pc->ok_consumo = 0;
}