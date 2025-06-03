#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MAX_THREADS 1000
#define MAX_D 11111
#define MAX_K 500

pthread_mutex_t lock, begin;
pthread_cond_t cond;
__thread int role;
int start;
__thread int position;
pthread_mutex_t walkp[MAX_D+1];
pthread_mutex_t walkc[MAX_D+1];
pthread_cond_t freep[MAX_D+1];
pthread_cond_t freec[MAX_D+1];

int posp[MAX_D];
int posc[MAX_D];
int n,d,k;

void *walk(void *arg) {
    int thread_num = *((int *) arg);
    //generate role
    //0->parent, 1->child
    role = rand_r(&thread_num)%2;
    if(role>1){
        role=1;
    }
    pthread_mutex_lock(&lock);
    thread_num=n-start;
    if(role==0){
        printf("I am thread %d -> a parent\n", thread_num);
    }
    else{
        printf("I am thread %d -> a child\n", thread_num);
    }
    start--;
    position=start+k;
    if(role==1){
        for(int i=position-10; i<=position; i++){
            if(i>=0 && i<=(d-k)){
                posc[i]++;
            }
        }
    }
    else{
        for(int i=position-10; i<=position; i++){
            if(i>=0 && i<=(d-k)){
                posp[i]++;
            }
        }
    }
    if (start == 0) {
        pthread_cond_broadcast(&cond);
    } else {
        pthread_cond_wait(&cond, &lock); // wait for all
    }
    pthread_mutex_unlock(&lock);
    while(position!=d+n){
        if(role==1){
            //go
            pthread_mutex_lock(&walkp[position+1]);
            while(posp[position+1]){
                pthread_cond_wait(&freec[position+1], &walkp[position+1]);
            }
            pthread_mutex_unlock(&walkp[position+1]);
            pthread_mutex_lock(&walkc[position+1]);
            position++;
            posc[position]++;
            printf("Child %d moved 1 step. Now at %d.\n", thread_num, position-n);
            fflush(stdout);
            pthread_mutex_unlock(&walkc[position]);
            //now free position-k
            pthread_mutex_lock(&walkc[position-k-1]);
            posc[position-k-1]--;
            if(posc[position-k-1]==0){
                pthread_cond_broadcast(&freep[position-k-1]);
            }
            pthread_mutex_unlock(&walkc[position-k-1]);
        }
        else{
            //go
            pthread_mutex_lock(&walkc[position+1]);
            while(posc[position+1]){
                pthread_cond_wait(&freep[position+1], &walkc[position+1]);
            }
            pthread_mutex_unlock(&walkc[position+1]);
            pthread_mutex_lock(&walkp[position+1]);
            position++;
            posp[position]++;
            printf("Parent %d moved 1 step. Now at %d.\n", thread_num, position-n);
            fflush(stdout);
            pthread_mutex_unlock(&walkp[position]);
            //now free position-k
            pthread_mutex_lock(&walkp[position-k-1]);
            posp[position-k-1]--;
            if(posp[position-k-1]==0){
                pthread_cond_broadcast(&freec[position-k-1]);
            }
            pthread_mutex_unlock(&walkp[position-k-1]);
        }
    }
    if (role==1){
        printf("Child %d is back at home.\n", thread_num);
    }
    else{
        printf("Parent %d is back at home.\n",thread_num);
    }
    for(int i=0; i<=10; i++){
        if(role==1){
            pthread_mutex_lock(&walkc[position-i]);
            posc[position-i]--;
            pthread_mutex_unlock(&walkc[position-i]);
            pthread_cond_signal(&freep[position-i]);
        }
        else{
            pthread_mutex_lock(&walkp[position-i]);
            posp[position-i]--;
            pthread_mutex_unlock(&walkp[position-i]);
            pthread_cond_signal(&freec[position-i]);
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        perror("Invalid number of parameters.\nn->number of people\nd ->walk length\nk -> number of steps\n");
        exit(1);
    }
    n = atoi(argv[1]);
    d = atoi(argv[2]);
    k = atoi(argv[3]);
    if (n > MAX_THREADS){
        n = MAX_THREADS;
        printf("Too many people, reducing to %d.\n", n);
    }
    if (d > MAX_D) {
        d = MAX_D;
        printf("Too long walk, reducing to %d.\n", d);
    }
    if (k > MAX_K) {
        k = MAX_K;
        printf("Too big step difference, reducing to %d.\n", k);
    }
    pthread_t threads[MAX_THREADS];
    int thread_nums[MAX_THREADS];
    int i, result;
    start=n;
    for (i = 0; i < n; i++) {
        thread_nums[i] = i + 1;
        result = pthread_create(&threads[i], NULL, walk, &thread_nums[i]);
        if (result != 0) {
            printf("Error creating thread\n");
            return -1;
        }
    }
    for (i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
