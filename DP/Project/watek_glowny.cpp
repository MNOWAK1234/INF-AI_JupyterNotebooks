#include "main.h"
#include "watek_glowny.h"
#include "util.h"

// Number of pistols
int P = 5;
// Number of cycles
int C = 3;

// Partner ID, default -1 to correct check this inside functions
int partnerID = -1;
int myrole = -1;
int iteration = 0;
int score=0;
int roundsfinished = 0;
bool haveme;
bool havegun;

// Need someone to kill or get killed
void acquire_partner()
{
    pthread_mutex_lock(&ACK_mutex);
    ACKcount = -1;
    pthread_mutex_unlock(&ACK_mutex);
    broadcast(0, PARTNER_REQ);
    //wait for getting role assigned
    while(myrole == -1);
    if(myrole == KILLER) {
        // Selected partner - time to go killing
        printf("[%05d][PID: %02d][IT: %02d] I have partner! Selected process %02d And I am a killer \n", LamportClock,
               rank, iteration, partnerID);
    }
    else{
        printf("[%05d][PID: %02d][IT: %02d] I have partner! Selected process %02d And I am a runner \n", LamportClock,
               rank, iteration, partnerID);
    }
}


void release_pistol(){
    changeState(FREE);
    havegun = false;
    haveme = false;
    broadcast(0, REMOVE_FROM_GUN_QUEUE);
};


void get_pistol()
{
    changeState(REQUESTING);
    pthread_mutex_lock(&ACK_mutex);
    ACKcount = -1;
    broadcast(0, GUN_REQ);
    pthread_mutex_unlock(&ACK_mutex);
    while(!havegun);
}


void killing()
{
    get_pistol();
    int attack = rand() % 20;
    packet_t *pkt = (packet_t *)malloc(sizeof(packet_t));
    pkt->data = attack;
    sendPacket(pkt, partnerID, KILL_ATTEMPT);
    free(pkt);
    release_pistol();
}

void sleep(double seconds)
{
    usleep(seconds*1000000);
}


void mainLoop()
{
    srandom(rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    changeState(FREE);
    iteration = 0;
    while (stan != FINISH)
    {
        iteration++;
        printf("[%05d][%02d] -- CODE RUN -- ITERATION %02d --\n", LamportClock, rank, iteration);
        //Refresh data
        myrole = -1;
        score = 0;
        roundsfinished = 0;
        haveme = false;
        havegun = false;
        changeState(FREE);
        //get a partner
        acquire_partner();
        debug("My partner is: %d", partnerID);
        for (int current_cycle = 0; current_cycle < C; current_cycle++)
        {
            //Obtain results
            if(myrole == KILLER)
            {
                killing();
            }
        }
        while(roundsfinished < C);
        //Summary
        if(myrole == KILLER)
        {
            if(score > 0)
            {
                debug("KILLER %d won against RUNNER %d by %d points", rank, partnerID, score);
            }
            if(score < 0)
            {
                debug("KILLER %d lost against RUNNER %d by %d points", rank, partnerID, -score);
            }
            if(score == 0)
            {
                debug("KILLER %d tied against RUNNER %d", rank, partnerID);
            }
        }
        //Start new round
    }
}
