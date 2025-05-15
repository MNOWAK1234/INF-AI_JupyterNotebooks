#ifndef PUT_PR_MPI_PACKET_H
#define PUT_PR_MPI_PACKET_H


/* typ pakietu */
typedef struct {
    int ts;       /* timestamp (zegar lamporta) */
    int src;
    int data;     /* przykładowe pole z danymi; można zmienić nazwę na bardziej pasującą */
} packet_t;
/* packet_t ma trzy pola, więc NITEMS = 3. Wykorzystane w inicjuj_typ_pakietu */
#define NITEMS 3



#endif //PUT_PR_MPI_PACKET_H
