#include "header.h"

int main(){
    pid_t pid;
    int status;
    //KEY
    key_t key_1 = ftok(".",'a');
    key_t key_2 = ftok(".",'b');

    int queue_1 = msgget(key_1,IPC_CREAT|0664);
    int queue_2 = msgget(key_2,IPC_CREAT|0664);

    //AVVIO CLIENT
    for(int i=0;i<NUM_CLIENT;i++){
        pid = fork();
        if(pid == 0){
            //Processi client
            execl("./client_exe","./client_exe",NULL);
            perror("ERRORE EXECL");
            exit(1);
        }
    }

    //AVVIO SERVER
    pid = fork();
    if(pid == 0){
        //Processi client
        execl("./server_exe","./server_exe",NULL);
        perror("ERRORE EXECL");
        exit(1);
    }

    for(int i=0;i<NUM_CLIENT;i++){
        pid = wait(&status);
        if(pid<0){
            perror("Errore wait!\n");
        }
        printf("Processo %d terminato con stato:%d\n",pid,status);
    }

    //Invio messaggio di terminazione server
    Message exit_msg;
    exit_msg.type = MSG_TYPE;

    for(int i=0;i<DIM_BUFFER;i++){
        exit_msg.vett[i] = -1;
    }
    msgsnd(queue_1,(void*)&exit_msg,sizeof(Message)-sizeof(long),IPC_NOWAIT);
    printf("Attendo terminazione SERVER...\n");
    pid = wait(&status);
    if(pid<0){
        perror("Errore wait!\n");
    }
    printf("Processo SERVER %d terminato con stato:%d\n",pid,status);


    msgctl(queue_1,IPC_RMID,0);
    msgctl(queue_2,IPC_RMID,0);
    return 0;
}