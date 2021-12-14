#include "header.h"

void aggiorna(Buffer* p,int sem){
    //printf("Aggiorno..\n");
    Wait_Sem(sem,SYNCH);
    printf("STRINGA %s\n",p->stringa);
    for(int i=0;i<STR_LEN;i++){
        srand(time(NULL)*i);
        p->stringa[i] = (char) ((rand() % 26)+ 65);
    }
    p->stringa[STR_LEN] = '\0';
    //printf("Ci sono quasi..\n");
    Signal_Sem(sem,SYNCH);
    //printf("Fine aggiornamento.\n");
}   

void azzera(Buffer* p,int sem){
    Wait_Sem(sem,SYNCH);
    printf("[DOCENTE]  STRINGA <%s>  N. PRENOTATI <%d>\n",p->stringa,p->num_studenti);
    p->num_studenti = 0;
    Signal_Sem(sem,SYNCH);
}

void prenota(Buffer* p,int sem){
    Wait_Sem(sem,MUTEXL);
    (p->num_lettori)++;
    if(p->num_lettori == 1){
        Wait_Sem(sem,SYNCH);
    }
    Signal_Sem(sem,MUTEXL);

    printf("[STUDENTE]  STRINGA <%s>\n",p->stringa);
    

    Wait_Sem(sem,MUTEXL);
    (p->num_studenti)++;
    (p->num_lettori)--;
    if(p->num_lettori == 0){
        Signal_Sem(sem,SYNCH);
    }
    Signal_Sem(sem,MUTEXL);
}