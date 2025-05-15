#include "main.h"
#include "watek_glowny.h"
#include "watek_komunikacyjny.h"

int rank = 0, size; // rank == MyPID , size == WORLD_SIZE
int LamportClock = 0;
pthread_t threadKom, threadMon;
// Mutex - change state
pthread_mutex_t state_mutex = PTHREAD_MUTEX_INITIALIZER;
// Mutex - clock
pthread_mutex_t clock_mutex = PTHREAD_MUTEX_INITIALIZER;
// Mutex - ACK Count
pthread_mutex_t ACK_mutex = PTHREAD_MUTEX_INITIALIZER;
int ACKcount = 0;

void finalizuj()
{
    pthread_mutex_destroy(&state_mutex);
    pthread_mutex_destroy(&clock_mutex);
    pthread_mutex_destroy(&ACK_mutex);
    /* Czekamy, aż wątek potomny się zakończy */
    println("czekam na wątek \"komunikacyjny\"\n" );
    pthread_join(threadKom,NULL);
    MPI_Type_free(&MPI_PAKIET_T);
    MPI_Finalize();
}

void check_thread_support(int provided)
{
    printf("THREAD SUPPORT: chcemy %d. Co otrzymamy?\n", provided);
    switch (provided) {
        case MPI_THREAD_SINGLE: 
            printf("Brak wsparcia dla wątków, kończę\n");
            /* Nie ma co, trzeba wychodzić */
	    fprintf(stderr, "Brak wystarczającego wsparcia dla wątków - wychodzę!\n");
	    MPI_Finalize();
	    exit(-1);
	    break;
        case MPI_THREAD_FUNNELED: 
            printf("tylko te wątki, ktore wykonaly mpi_init_thread mogą wykonać wołania do biblioteki mpi\n");
	    break;
        case MPI_THREAD_SERIALIZED: 
            /* Potrzebne zamki wokół wywołań biblioteki MPI */
            printf("tylko jeden watek naraz może wykonać wołania do biblioteki MPI\n");
	    break;
        case MPI_THREAD_MULTIPLE: printf("Pełne wsparcie dla wątków\n"); /* Tego chcemy. Wszystkie inne powodują problemy */
	    break;
        default: printf("Nikt nic nie wie\n");
    }
}


int main(int argc, char **argv)
{
    int provided;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    check_thread_support(provided);
    inicjuj_typ_pakietu(); // tworzy typ pakietu
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    srand(rank);
    pthread_create( &threadKom, NULL, startKomWatek , 0);
    mainLoop();
    
    finalizuj();
    return 0;
}

