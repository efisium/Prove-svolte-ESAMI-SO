#include "header.h"

int main(){
    pid_t pid;
    int status;

    //KEY SHM - QUEUE
    key_t shm_key = ftok(".",'a');
    key_t queue_key = ftok(".",'b');
    key_t sem_key = ftok(".",'c');

    printf("Avvio MAIN %d...   [queue key: %d]\n",getpid(),queue_key);

    //Creazione SHM
    int shm_id = shmget(shm_key,sizeof(struct buffer),IPC_CREAT|0664);

    //Definizione puntatore a SHM
    struct buffer* p = (struct buffer*)shmat(shm_id,NULL,0);

    p->val[0] = 0;
    p->val[1] = 0;

    //Creazione coda messaggi
    int queue_id = msgget(queue_id,IPC_CREAT|0664);

    //Creazione mutex
    int sem_id = semget(sem_key,1,IPC_CREAT|0664);
    semctl(sem_id,0,SETVAL,1);
    

    //Esecuzione processi
    //CLIENT
    for(int i=0;i<N_CLIENT;i++){
        pid = fork();
        if(pid == 0){
            execl("./client_exe","./client_exe",NULL);
            perror("ERRORE EXECL");
        }
    }

    //SERVER
    for(int i=0;i<N_SERVER;i++){
        pid = fork();
        if(pid == 0){
            execl("./server_exe","./server_exe",NULL);
            perror("ERRORE EXECL");
        }
    }

    for(int i=0;i<N_CLIENT + N_SERVER;i++){
        pid = wait(&status);
        if(pid<0){
            perror("PROCESSO TERMINATO CON VALORE < 0");
        }else{
            printf("Processo %d terminato con stato %d\n",pid,status);
        }    
    }


    msgctl(queue_id,IPC_RMID,0);
    shmctl(shm_id,IPC_RMID,0);
    return 0;
}