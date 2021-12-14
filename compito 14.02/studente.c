#include "header.h"

int main(){
    printf("[%d] Avvio processo STUDENTE...\n",getpid());
    
    //SHM
    key_t shm_key = ftok(".",'a');
    int shm_id = shmget(shm_key,sizeof(Buffer),IPC_CREAT|0664);
    //printf("[STUDENTE] SHM ID %d\n",shm_id);
    Buffer* p = (Buffer*) shmat(shm_id,NULL,0);

    //SEM
    key_t sem_key = ftok(".",'b');
    //printf("[STUDENTE] SEM KEY %d\n",sem_key);
    int sem_id = semget(sem_key,2,0664);
    //printf("[STUDENTE] SEM ID %d\n",sem_id);

    srand(time(NULL)*getpid());
    int t = rand()%9;
    sleep(t);
    prenota(p,sem_id);
    
    return 0;
}