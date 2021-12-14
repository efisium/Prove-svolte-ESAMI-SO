#include "header.h"

int main(){
    printf("[%d] Avvio processo DOCENTE...\n",getpid());

    //SHM
    key_t shm_key = ftok(".",'a');
    int shm_id = shmget(shm_key,sizeof(Buffer),IPC_CREAT|0664);
    //printf("[DOCENTE] SHM ID %d\n",shm_id);
    Buffer* p = (Buffer*) shmat(shm_id,NULL,0);

    //SEM
    key_t sem_key = ftok(".",'b');
    //printf("[DOCENTE] SEM KEY %d\n",sem_key);
    int sem_id = semget(sem_key,2,IPC_CREAT|0664);
    //printf("[DOCENTE] SEM ID %d\n",sem_id);
    semctl(sem_id,MUTEXL,SETVAL,1);
    semctl(sem_id,SYNCH,SETVAL,1);

    for(int i=0;i<3;i++){
        aggiorna(p,sem_id);
        sleep(3);
        azzera(p,sem_id);
    }
    return 0;
}