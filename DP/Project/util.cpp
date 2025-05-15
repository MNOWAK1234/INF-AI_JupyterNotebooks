#include "main.h"
#include "util.h"
MPI_Datatype MPI_PAKIET_T;

struct tagNames_t{
    const char *name;
    int tag;
} tagNames[] = {{"APP Packet", APP_PKT },
                {"Finish", FINISH},
                {"Want to be paired", PARTNER_REQ},
                {"Accept pairing request", PAIRING_ACK},
                {"You are killer", YOU_ARE_KILLER},
                {"You are runner", YOU_ARE_RUNNER},
                {"Delete from pairing queue", REMOVE_FROM_PAIRING_QUEUE},
                {"Delete from gun queue", REMOVE_FROM_GUN_QUEUE},
                {"I want a gun", GUN_REQ},
                {"You can get a gun", GUN_ACK},
                {"I want to kill you", KILL_ATTEMPT},
                {"You missed", KILL_AVOIDED},
                {"Headshot", KILL_CONFIRMED},
                {"Wait for me", WAIT}
};

state_t stan=FREE;

const char *const tag2string( int tag )
{
    for (int i=0; i < (int)sizeof(tagNames)/sizeof(struct tagNames_t);i++) {
	if ( tagNames[i].tag == tag )  return tagNames[i].name;
    }
    return "<unknown>";
}
/* tworzy typ MPI_PAKIET_T
*/
void inicjuj_typ_pakietu()
{
    /* Stworzenie typu */
    /* Poniższe (aż do MPI_Type_commit) potrzebne tylko, jeżeli
       brzydzimy się czymś w rodzaju MPI_Send(&typ, sizeof(pakiet_t), MPI_BYTE....
    */
    /* sklejone z stackoverflow */
    int       blocklengths[NITEMS] = {1,1,1};
    MPI_Datatype typy[NITEMS] = {MPI_INT, MPI_INT, MPI_INT};

    MPI_Aint     offsets[NITEMS]; 
    offsets[0] = offsetof(packet_t, ts);
    offsets[1] = offsetof(packet_t, src);
    offsets[2] = offsetof(packet_t, data);

    MPI_Type_create_struct(NITEMS, blocklengths, offsets, typy, &MPI_PAKIET_T);

    MPI_Type_commit(&MPI_PAKIET_T);
}

/* opis patrz util.h */
void sendPacket(packet_t *pkt, int destination, int tag)
{
    int freepkt=0;
    if (pkt==0) { pkt = (packet_t *)malloc(sizeof(packet_t)); freepkt=1;}
    pkt->src = rank;
    pthread_mutex_lock(&clock_mutex);
    LamportClock++;
    pkt->ts = LamportClock;
    MPI_Send( pkt, 1, MPI_PAKIET_T, destination, tag, MPI_COMM_WORLD);
    pthread_mutex_unlock(&clock_mutex);
    //debug("Wysyłam %s do %d w czasie %d\n", tag2string( tag), destination, pkt->ts);
    if (freepkt) free(pkt);
}

void changeState( state_t newState )
{
    pthread_mutex_lock( &state_mutex );
    if (stan==FINISH) { 
	pthread_mutex_unlock( &state_mutex );
        return;
    }
    stan = newState;
    pthread_mutex_unlock( &state_mutex );
}

void tick_Lamport_clock(int nowy)
{
    pthread_mutex_lock( &clock_mutex );
    if(nowy > LamportClock){
        LamportClock = nowy + 1;
    }
    else{
        LamportClock += 1;
    }
    pthread_mutex_unlock( &clock_mutex );

}

void broadcast(packet_t *pkt, int tag)
{
    int freepkt=1;
    if (pkt==0) { pkt = (packet_t *)malloc(sizeof(packet_t)); freepkt=1;}
    pkt->src = rank;
    pthread_mutex_lock(&clock_mutex);
    LamportClock++;
    pkt->ts = LamportClock;
    for(int j=0; j<size; j++){
        MPI_Send( pkt, 1, MPI_PAKIET_T, j, tag, MPI_COMM_WORLD);
    }
    pthread_mutex_unlock(&clock_mutex);
    //debug("Wysyłam Grupowo %s w czasie %d\n", tag2string(tag), pkt->ts);
    if (freepkt) free(pkt);
}

