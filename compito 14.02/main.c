#include "header.h"

int main(){
    pid_t pid;
    int status;

    //SHM
    key_t shm_key = ftok(".",'a');
    int shm_id = shmget(shm_key,sizeof(Buffer),IPC_CREAT|0664);
    //printf("[MAIN] SHM ID %d\n",shm_id);
    Buffer* p = (Buffer*) shmat(shm_id,NULL,0);

    for(int i=0;i<STR_LEN;i++){
        srand(time(NULL)*i);
        p->stringa[i] = (char) ((rand() % 26)+ 65);
    }
    p->stringa[STR_LEN] = '\0';
    p->num_studenti = 0;
    p->num_lettori = 0;

    //SEM
    key_t sem_key = ftok(".",'b');
    //printf("[MAIN] SEM KEY %d\n",sem_key);
    int sem_id = semget(sem_id,2,IPC_CREAT|0664);
    //printf("[MAIN] SEM ID %d\n",sem_id);
    semctl(sem_id,MUTEXL,SETVAL,1);
    semctl(sem_id,SYNCH,SETVAL,1);

    //PROCESSO DOCENTE
    pid = fork();
    if(pid == 0){
        execl("./docente_exe","./docente_exe",NULL);
        perror("EXECL ERROR");
        exit(1);
    }

    //PROCESSI STUDENTE
    int i;
    for(i=0;i<NUM_STUDENTI;i++){
        pid = fork();
        if(pid == 0){
            execl("./studente_exe","./studente_exe",NULL);
            perror("EXECL ERROR");
            exit(1);
        }
    }

    for(i=0;i<NUM_STUDENTI+1;i++){
        pid = wait(&status);
        if(pid<0){
            perror("WAIT ERROR");
            exit(1);
        }
        printf("Processo %d terminato con stato:%d",pid,status);
    }

    shmctl(shm_id,IPC_RMID,0);
    semctl(sem_id,0,IPC_RMID);
    return 0;
}