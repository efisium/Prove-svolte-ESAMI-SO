#include "header.h"

int main(){
    pid_t pid;
    int status;

    //MSG QUEUE
    key_t queue_key = ftok(".",'a');
    int queue = msgget(queue_key,IPC_CREAT|0664);

    //**** Genero processi
    //CLIENT
    for(int i=0;i<N_CLIENT;i++){
        pid = fork();
        if(pid == 0){
            execl("./client_exe","./client_exe",NULL);
            perror("EXECL ERROR\n");
            exit(1);
        }
    }

    //SERVER
    pid = fork();
    if(pid == 0){
        execl("./server_exe","./server_exe",NULL);
        perror("EXECL ERROR\n");
        exit(1);
    }

    //Attendo terminazione 
    for(int i=0;i<N_CLIENT+1;i++){
        pid = wait(&status);
        if(pid<0){
            perror("WAIT ERROR\n");
            exit(1);
        }
        printf("Processo %d terminato con stato <%d>\n",pid,status);
    }

    
    msgctl(queue,IPC_RMID,0);
    return 0;
}