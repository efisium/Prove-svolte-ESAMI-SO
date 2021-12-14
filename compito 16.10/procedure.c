#include "header.h"

void inizializza(MonitorBuffer *b,MonitorVettore *v){
    b->m = v;
    b->buffer.a = 0;
    b->buffer.b = 0;

    pthread_mutex_init(&b->mutex,NULL);
    pthread_cond_init(&b->ok_lett_cv,NULL);
    pthread_cond_init(&b->ok_scritt_cv,NULL);

    pthread_mutex_init(&b->m->mutex,NULL);
    pthread_cond_init(&b->m->ok_cons_cv,NULL);
    pthread_cond_init(&b->m->ok_prod_cv,NULL);

    b->lettori = 0;
    b->scrittori = 0;
    b->lett_wait = 0;
    b->scritt_wait = 0;

    b->m->testa = 0;
    b->m->coda = 0;
    b->m->cont = 0;
    b->m->prod = 0;
    b->m->cons = 0;

    for(int i=0;i<LEN_VETT;i++){
        b->m->vettore[i].a = 0;
        b->m->vettore[i].b = 0;
    }

}
void rimuovi(MonitorBuffer *b, MonitorVettore *v){
    pthread_mutex_destroy(&b->mutex);
    pthread_cond_destroy(&b->ok_lett_cv);
    pthread_cond_destroy(&b->ok_scritt_cv);

    pthread_mutex_destroy(&b->m->mutex);
    pthread_cond_destroy(&b->m->ok_cons_cv);
    pthread_cond_destroy(&b->m->ok_prod_cv);
}

void genera(MonitorVettore *v, elemento e){
    pthread_mutex_lock(&v->mutex);
    while(v->cont == LEN_VETT){
        printf("[GENERATORE] Vettore pieno..\n");
        pthread_cond_wait(&v->ok_prod_cv,&v->mutex);
    }

    v->vettore[v->coda] = e;
    printf("[GENERATORE] Elemento generato [%d] [%d]\n",v->vettore[v->coda].a,v->vettore[v->coda].b);
    (v->coda) = (v->coda + 1) % LEN_VETT;
    (v->cont)++;
    printf("[GENERATORE] Cont: %d\n",v->cont);
    pthread_cond_signal(&v->ok_cons_cv);
    pthread_mutex_unlock(&v->mutex);
}


elemento preleva(MonitorVettore *v){
    pthread_mutex_lock(&v->mutex);
    while(v->cont == 0){
        pthread_cond_wait(&v->ok_cons_cv,&v->mutex);
    }

    elemento e = v->vettore[v->testa];
    printf("[AGGIORNATORE] Elemento prelevato [%d] [%d]\n",v->vettore[v->testa].a,v->vettore[v->testa].b);
    (v->testa) = (v->testa + 1) % LEN_VETT;
    (v->cont)--;
    printf("[AGGIORNATORE] Cont: %d\n",v->cont);
    
    pthread_cond_signal(&v->ok_prod_cv);
    pthread_mutex_unlock(&v->mutex);
    return e;
}


void aggiorna(MonitorBuffer *b, elemento e){
    pthread_mutex_lock(&b->mutex);
    while(b->lettori>0){
        pthread_cond_wait(&b->ok_scritt_cv,&b->mutex);
    }
    b->scrittori++;
    pthread_mutex_unlock(&b->mutex);

    b->buffer = e;
    printf("[AGGIORNATORE] Elemento posto nel buffer [%d] [%d]\n",b->buffer.a,b->buffer.b);
    
    pthread_mutex_lock(&b->mutex);
    b->scrittori--;
    pthread_cond_broadcast(&b->ok_lett_cv);
    pthread_mutex_unlock(&b->mutex);
}


void consulta(MonitorBuffer *b){
    pthread_mutex_lock(&b->mutex);
    while(b->scrittori>0){
        pthread_cond_wait(&b->ok_lett_cv,&b->mutex);
    }
    b->lettori++;
    pthread_mutex_unlock(&b->mutex);

    printf("[DESTINATARIO] Valori BUFFER [%d] [%d]\n",b->buffer.a,b->buffer.b);
    
    pthread_mutex_lock(&b->mutex);
    b->lettori--;
    if(b->lettori == 0){
        pthread_cond_signal(&b->ok_scritt_cv);
    }
    pthread_mutex_unlock(&b->mutex);
}


void* startGeneratore(void *g){
    MonitorVettore *v = (MonitorVettore*)g;
    elemento e;
    int i;
    for(int i=0;i<20;i++){
        srand(time(NULL)*i);
        e.a = rand()%11;
        i++;
        e.b = rand()%11;
        genera(v,e);
        //printf("[GENERATORE] Elemento generato [%d] [%d]\n",v->vettore[v->testa].a,v->vettore[v->testa].b);
    }
    pthread_exit(NULL);
}
void* startAggiornatore(void *g){
    MonitorBuffer *b = (MonitorBuffer*)g;
    elemento e;
    for(int i=0;i<10;i++){
        sleep(1);
        e = preleva(b->m);
        aggiorna(b,e);
    }
    pthread_exit(NULL);
}
void* startDestinatario(void *g){
    MonitorBuffer *b = (MonitorBuffer*)g;
    for(int i=0;i<6;i++){
        sleep(2);
        consulta(b);
    }
    pthread_exit(NULL);
}

