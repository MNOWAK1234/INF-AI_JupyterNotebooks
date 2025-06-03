#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdbool.h>
#include <time.h>

#define MAX_N 50
#define MAX_K 10
#define KEY 45281

int n,k;
pid_t pid;
int msgid;
int sztuczny;

struct msg_buf {
    long mtype;
    int readers[MAX_N+1];
};

struct msg_buf msg_send;
struct msg_buf msg_receive;

void podnies(int semid, int semnum){
   struct sembuf sem;
   sem.sem_num = semnum;
   sem.sem_op = 1;
   sem.sem_flg = 0;
   if (semop(semid, &sem, 1) == -1){
      perror("Podnoszenie semafora");
      exit(1);
   }
}

void opusc(int semid, int semnum){
   struct sembuf sem;
   sem.sem_num = semnum;
   sem.sem_op = -1;
   sem.sem_flg = 0;
   if (semop(semid, &sem, 1) == -1){
      perror("Opuszczenie semafora");
      exit(1);
   }
}

void rozmiar(){
    struct msqid_ds b;
    msgid = msgget(KEY, 0600 | IPC_CREAT);   
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }
    if (msgctl(msgid, IPC_STAT, &b) == -1) {
        perror("msgctl");
        exit(1); 
    }
    printf("Current size of the message queue: %ld bytes, a ile elementow:  %d \n", b.__msg_cbytes, (int)b.msg_qnum);
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        perror("Zla liczba parametrow.\nn->liczba procesow\nk -> maksymalna liczba dziel w kolejce\n");
        exit(1);
    }
    n = atoi(argv[1]);
    k = atoi(argv[2]);
    if (n > MAX_N){
        n = MAX_N;
        printf("Za duzo procesow, zmniejszam do %d.\n", n);
    }
    if (k > MAX_K) {
        k = MAX_K;
        printf("Za duza pojemnosc kolejki komunikatow. Zmniejszam do %d.\n", k);
    }
    //dodaj semafory i pamiec wspoldzielona
    int shmid, semid, i;
    int *buf;
    key_t key;
    key = ftok("/etc/passwd", 42);
    if (key == -1){
        perror("ftok");
        return 1;
    }
    //MAX_N->ile zablokowanych na czytaniu
    //MAX_N+1->ile zablokowanych na pisaniu
    //MAX_N+2->zakodowany komunikat co zrobic
    shmid = shmget(key, (MAX_N+3)*sizeof(int), IPC_CREAT|0600);
    if (shmid == -1){
        printf("Error: %s\n", strerror(errno));
        perror("Utworzenie segmentu pamieci wspoldzielonej");
        exit(1);
    }
    buf = (int*)shmat(shmid, NULL, 0);
    if (buf == NULL){
        perror("Przylaczenie segmentu pamieci wspoldzielonej");
        exit(1);
    }
    semid = semget(key, 6, IPC_CREAT|IPC_EXCL|0600);
    if (semid == -1){
        semid = semget(key, 6, 0600);
        if (semid == -1){
            perror("Utworzenie tablicy semaforow");
            exit(1);
        }
    }
    buf[MAX_N+0]=0;
    buf[MAX_N+1]=0;
    buf[MAX_N+2]=0;
    //ile do zapisu
    if (semctl(semid, 0, SETVAL, (int)MAX_K) == -1){
        perror("Nadanie wartosci semaforowi 0");
        exit(1);
    }
    //ile do odczytu
    if (semctl(semid, 1, SETVAL, (int)0) == -1){
        perror("Nadanie wartosci semaforowi 1");
        exit(1);
    }
    //wylacznosc zapisu
    if (semctl(semid, 2, SETVAL, (int)1) == -1){
        perror("Nadanie wartosci semaforowi 2");
        exit(1);
    }
    //wylacznosc odczytu
    if (semctl(semid, 3, SETVAL, (int)1) == -1){
        perror("Nadanie wartosci semaforowi 3");
        exit(1);
    }
    //sprawdzanie czy deadlock
    if (semctl(semid, 4, SETVAL, (int)1) == -1){
        perror("Nadanie wartosci semaforowi 4");
        exit(1);
    }
    //wylacznosc sprawdzania roli
    if (semctl(semid, 5, SETVAL, (int)1) == -1){
        perror("Nadanie wartosci semaforowi 5");
        exit(1);
    }
    //dodaj kolejke komunikatow
    msgid = msgget(KEY, IPC_CREAT|IPC_EXCL|0600);
    if (msgid == -1){
        msgid = msgget(KEY, IPC_CREAT|0600);
        if (msgid == -1){
            perror("Utworzenie kolejki komunikatow");
            exit(1);
        }
    }
    //tworzenie procesow
    for (i=0; i<n; i++){
        pid=fork();
        if (pid < 0){
            perror("fork");
            exit(1);
        }
        else{
            if(pid > 0){
                int forked = msgget(KEY, 0);
                if (forked == -1) {
                    perror("forked msgget error");
                    exit(1);
                }
                int role = rand_r(&pid)%2;
                opusc(semid, 5);
                buf[i]=role;
                podnies(semid,5);
                if(role==0){
                    printf("Proces %d jest czytelnikiem.\n", i);
                    fflush(stdout);
                }
                else{
                    printf("Proces %d jest pisarzem.\n", i);
                    fflush(stdout);
                }
                while(1){
                    sztuczny=0;
                    sleep(rand_r(&pid)%3+1); //faza relaksu
                    role=rand_r(&pid)%2; //zmiana
                    opusc(semid, 5);
                    buf[i]=role;
                    podnies(semid,5);
                    sleep(rand_r(&pid)%3+1); //dalej relaks
                    if(role==0){
                        printf("Proces %d jest teraz czytelnikiem.\n", i);
                        fflush(stdout);
                        opusc(semid,4);
                        buf[MAX_N]++;
                        //printf("Aktualnie %d czeka na czytanie, a %d na pisanie\n",buf[MAX_N], buf[MAX_N+1]);
                        //rozmiar();
                        fflush(stdout);
                        if(buf[MAX_N]==n){
                            buf[MAX_N+2]=1;
                            podnies(semid,1);
                        }
                        podnies(semid,4);
                        opusc(semid,1);
                        opusc(semid,3);
                        opusc(semid,4);
                        buf[MAX_N]--;
                        if(buf[MAX_N+2]==1){
                            buf[MAX_N+2]=0;
                            podnies(semid,3);
                            podnies(semid,4);
                            continue;
                        }
                        podnies(semid,4);
                        fflush(stdout);
                        msgrcv(forked, &msg_receive, sizeof(int)*(MAX_N+1), 0, 0);
                        bool domnie=false;
                        if(msg_receive.readers[i]==1){
                            domnie=true;
                        }
                        msg_receive.readers[i]=0;
                        bool zostalo=false;
                        int nadawca;
                        for(int r=0; r<MAX_N; r++){
                            if(msg_receive.readers[r]==1){
                                zostalo=true;
                                //printf("Jeszcze %d musi odczytac wiadomosc %d\n", r, msg_receive.readers[MAX_N]);
                            }
                            if(msg_receive.readers[r]==2){
                                nadawca=r;
                            }
                        }
                        if(domnie){
                            printf("Jestem czytelnikiem %d, i czytam wiadomosc %d od %d.\n", i, msg_receive.readers[MAX_N], msg_receive.readers[nadawca]);
                            fflush(stdout);
                        }
                        else{
                            printf("Jestem czytelnikiem %d, i wiadomosc %d od %d nie byla do mnie.\n", i, msg_receive.readers[MAX_N], msg_receive.readers[nadawca]);
                            fflush(stdout);
                        }
                        if(zostalo){
                            msgsnd(forked, &msg_receive, sizeof(int)*(MAX_N+1), 0);
                        }
                        podnies(semid, 3);
                        if(zostalo){
                            podnies(semid,1);
                        }
                        else{
                            podnies(semid,0);
                            //printf("podnioslem 0\n");
                        }
                    }
                    else{
                        printf("Proces %d jest teraz pisarzem.\n", i);
                        //zrob najpierw to samo co czytelnik
                        opusc(semid,4);
                        buf[MAX_N]++;
                        //printf("Aktualnie %d czeka na czytanie, a %d na pisanie\n", buf[MAX_N], buf[MAX_N+1]);
                        //rozmiar();
                        if(buf[MAX_N]==n){
                            buf[MAX_N+2]=1;
                            podnies(semid,1);
                        }
                        podnies(semid,4);
                        opusc(semid,1);
                        opusc(semid,3);
                        opusc(semid,4);
                        buf[MAX_N]--;
                        if(buf[MAX_N+2]==1){
                            buf[MAX_N+2]=0;
                            podnies(semid,3);
                            podnies(semid,4);
                            sztuczny=1; //idz od razu do pisania
                        }
                        podnies(semid,4);
                        if(sztuczny==0){
                            msgrcv(forked, &msg_receive, sizeof(int)*(MAX_N+1), 0, 0);
                            bool domnie=false;
                            if(msg_receive.readers[i]==1){
                                domnie=true;
                            }
                            msg_receive.readers[i]=0;
                            bool zostalo=false;
                            int nadawca;
                            for(int r=0; r<=MAX_N; r++){
                                if(msg_receive.readers[r]==1){
                                    zostalo=true;
                                    //printf("Jeszcze %d musi odczytac wiadomosc %d\n", r, msg_receive.readers[MAX_N]);
                                }
                                if(msg_receive.readers[r]==2){
                                    nadawca=r;
                                }
                            }
                            if(domnie){
                                printf("Jestem pisarzem %d, ale najpierw czytam wiadomosc %d od %d.\n", i, msg_receive.readers[MAX_N], msg_receive.readers[nadawca]);
                                fflush(stdout);
                            }
                            else{
                                printf("Jestem pisarzem %d, ale najpierw czytam wiadomosc %d od %d, ktora nie byla do mnie.\n", i, msg_receive.readers[MAX_N], msg_receive.readers[nadawca]);
                                fflush(stdout);
                            }
                            if(zostalo){
                                msgsnd(forked, &msg_send, sizeof(int)*(MAX_N+1), 0);
                            }
                            podnies(semid, 3);
                            if(zostalo){
                                podnies(semid,1);
                            }
                            else{
                                podnies(semid,0);
                                //printf("podnioslem 0\n");
                            }
                        }
                        //teraz wyslij
                        opusc(semid,4);
                        buf[MAX_N+1]++;
                        if(buf[MAX_N+1]==n){
                            buf[MAX_N+2]=1;
                            podnies(semid,0);
                        }
                        podnies(semid,4);
                        opusc(semid,0);
                        opusc(semid,2);
                        opusc(semid,4);
                        buf[MAX_N+1]--;
                        if(buf[MAX_N+2]==1){
                            buf[MAX_N+2]=0;
                            podnies(semid,2);
                            podnies(semid,4);
                            continue;
                        }
                        podnies(semid,4);
                        msg_send.mtype = 0;
                        opusc(semid, 5);
                        for(int r=0; r<MAX_N; r++){
                            msg_send.readers[r]=buf[r];
                        }
                        msg_send.mtype=1;
                        podnies(semid, 5);
                        msg_send.readers[MAX_N] = rand_r(&pid)%100;
                        msg_send.readers[i] = 2;
                        if(msgsnd(forked, &msg_send, sizeof(int)*(MAX_N+1), 0)==-1){
                            fprintf(stderr, "msgsnd failed\n");
                            exit(EXIT_FAILURE);
                        }
                        //rozmiar();
                        printf("Jestem pisarzem %d, i wysylam wiadomosc %d.\n", i, msg_send.readers[MAX_N]);
                        fflush(stdout);
                        podnies(semid, 2);
                        podnies(semid, 1);
                    }
                }
                exit(0);
            }
        }
    }
    return 0;
}
