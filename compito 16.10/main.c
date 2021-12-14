#include "header.h"

int main(){
    MonitorVettore *v = (MonitorVettore*)malloc(sizeof(MonitorVettore));
    MonitorBuffer *b = (MonitorBuffer*)malloc(sizeof(MonitorBuffer));

    inizializza(b,v);

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

    pthread_t thread[N_THREADS];

    //Thread Generatore
    pthread_create(&thread[0],&attr,startGeneratore,(void*)v);

    //Thread Aggiornatore
    pthread_create(&thread[1],&attr,startAggiornatore,(void*)b);

    //Threads_Destinatario
    for(int i=2;i<5;i++){
        pthread_create(&thread[i],&attr,startDestinatario,(void*)b);
    }

    //JOIN
    int status;
    pthread_join(thread[0],(void**)&status);
    printf("Thread GENERATORE terminato con stato: %d\n",status);
    pthread_join(thread[1],(void**)&status);
    printf("Thread AGGIORNATORE terminato con stato: %d\n",status);
    for(int i=2;i<5;i++){
        pthread_join(thread[i],(void**)&status);
        printf("Thread DESTINATARIO terminato con stato: %d\n",status);
    }

    rimuovi(b,v);
    free(b);
    free(v);
    pthread_attr_destroy(&attr);
    return 0;
}