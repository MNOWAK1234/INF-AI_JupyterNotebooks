#ifndef UTILH
#define UTILH
#include "packet.h"
#include "main.h"


#define KILLER 1
#define RUNNER 0

/* Typy wiadomości */ // TAG, TAGS, tag, Tag, Tags, tags
#define APP_PKT 1
#define FINISH 2
#define KILL_ATTEMPT 11
#define KILL_AVOIDED 12
#define KILL_CONFIRMED 13
#define PARTNER_REQ 20
#define PAIRING_ACK 21
#define YOU_ARE_KILLER 22
#define YOU_ARE_RUNNER 23
#define GUN_REQ 33
#define GUN_ACK 34
#define WAIT 50
#define REMOVE_FROM_PAIRING_QUEUE 100
#define REMOVE_FROM_GUN_QUEUE 101

extern MPI_Datatype MPI_PAKIET_T;
void inicjuj_typ_pakietu();

/* wysyłanie pakietu, skrót: wskaźnik do pakietu (0 oznacza - stwórz pusty pakiet), do kogo, z jakim typem */
void sendPacket(packet_t *pkt, int destination, int tag);

void tick_Lamport_clock();
typedef enum {FREE, REQUESTING, FINISHED} state_t;
extern state_t stan;
void broadcast(packet_t *pkt, int tag);

#endif