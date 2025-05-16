#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <stdbool.h>
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <openssl/ssl.h>
#include <openssl/crypto.h>
#define MAX_MESSAGE_LENGTH 20
/*
0: Game state
1,2: start square
3,4: target square
5: promotion (otherwise 0)
*/

//Handle different buffer sizes using _write and _read
int _write(SSL* ssl, char *buf, int len)
{
	while (len > 0) {
	    int i = SSL_write(ssl, buf, len);
	    len -= i;
	    buf += i;
	}
}

int _read(SSL* ssl, char *buf, int bufsize)
{
	int totalRead = 0;
	do
	{
		int i = SSL_read(ssl, buf + totalRead, bufsize);
		bufsize -= i;
		totalRead += i;
	} while (buf[totalRead - 1] != '\n' && bufsize > 0);
	if(buf[totalRead - 1] == '\n') totalRead--;
    return totalRead;
}

struct cln
{
	int cfd;
	struct sockaddr_in caddr;
};

struct player{
    char message[MAX_MESSAGE_LENGTH];
    int opponent_fd;
    SSL* ssl;
    bool color; //O: White, 1: Black
};

struct player* players;

int initialize_games(int size)
{
    players = (struct player*)calloc(size, sizeof(struct player));
    if (players == NULL) {
        printf("Cannot create games \n");
        exit(0);
    } else {
        printf("Gamespace created.\n");
        return 1;
    }
}

int update_players_count(int size)
{
    struct player* temp = players;
    players = (struct player*)realloc(players, size * sizeof(struct player));
    if (!players) {
        players = temp;
        return -1;
    } else {
        return 1;
    }
}

int player_count = 3;
int waiting_player_fd = -1;
char buf[256];
SSL_CTX* ctx; // SSL Context

pthread_mutex_t count_players_mutex; // Checking player number
pthread_mutex_t waiting_player_mutex; //Only 1 player can wait for their opponent
pthread_mutex_t can_play_mutex; //Helping mutex to avoid active waiting

void* cthread(void* arg)
{
    struct cln* c = (struct cln*)arg;
    printf("[%lu] new connection from: %s:%d\n",
        (unsigned long int)pthread_self(),
        inet_ntoa((struct in_addr)c->caddr.sin_addr),
        ntohs(c->caddr.sin_port)
    );
    printf("%d\n", c->cfd);

    // SSL setup for this thread
    players[c->cfd].ssl = SSL_new(ctx);
    if (!players[c->cfd].ssl) {
        printf("SSL creation error.\n");
        free(c);
        return NULL;
    }
    SSL_set_fd(players[c->cfd].ssl, c->cfd);

    // SSL Handshake
    if (SSL_accept(players[c->cfd].ssl) <= 0) {
        printf("SSL handshake error.\n");
        SSL_free(players[c->cfd].ssl);
        free(c);
        return NULL;
    }
    pthread_mutex_lock(&waiting_player_mutex);
    if(waiting_player_fd == -1)
    {
        //Noone is waiting, so this player will
        waiting_player_fd = c->cfd;
        //Make ther players able to enter the lobby
        pthread_mutex_unlock(&waiting_player_mutex);
        //This player won't start playing until some other player appears in the lobby
        pthread_mutex_lock(&can_play_mutex);
    }
    else
    {
        //There is one waiting player
        //Match them
        players[c->cfd].opponent_fd = waiting_player_fd;
        players[waiting_player_fd].opponent_fd = c->cfd;
        //Assign colors
        players[c->cfd].color = 1;
        players[waiting_player_fd].color = 0;
        players[c->cfd].message[0] = 'U';
        players[waiting_player_fd].message[0] = 'U';
        //Free the waiting spot
        waiting_player_fd = -1;
        //Allow waiting player to start the game
        pthread_mutex_unlock(&can_play_mutex);
        //Allow new players enter the lobby
        pthread_mutex_unlock(&waiting_player_mutex);
    }
    printf("Let's play: %d, %d \n", c->cfd, players[c->cfd].opponent_fd);
    if(players[c->cfd].color == 0)
    {
        //Inform this player, that they are playing white
        _write(players[c->cfd].ssl, "W\n", 2);
    }
    else
    {
        //Inform this player, that they are playing black
        _write(players[c->cfd].ssl, "B\n", 2);
    }
    //Take into account only 1 player
    //There is no need to duplicate the games
    if(players[c->cfd].color == 0)
    {
        //Continously read and write until the game is finished
        while(1)
        {
            //Read move from white player
            _read(players[c->cfd].ssl, players[c->cfd].message, sizeof(players[c->cfd].message));
            //Send this move to the black player
            _write(players[players[c->cfd].opponent_fd].ssl, players[c->cfd].message, sizeof(players[c->cfd].message));
            //_write(players[c->cfd].opponent_fd, "\n", 1);
            //Check if white ended the game
            if(players[c->cfd].message[0] != 'U')
            {
                break;
            }
            //Read move from black player
            _read(players[players[c->cfd].opponent_fd].ssl, players[players[c->cfd].opponent_fd].message, sizeof(players[players[c->cfd].opponent_fd].message));
            //Send this move to the white player
            _write(players[c->cfd].ssl, players[players[c->cfd].opponent_fd].message, sizeof(players[players[c->cfd].opponent_fd].message));
            //_write(c->cfd, "\n", 1);
            //Check if black ended the game
            if(players[players[c->cfd].opponent_fd].message[0] != 'U')
            {
                break;
            }
        }
        close(players[c->cfd].opponent_fd);
        close(c->cfd);
    }
    
    free(c);
    // Lock mutex to safely decrement player_count
    pthread_mutex_lock(&count_players_mutex);
    player_count--;
    pthread_mutex_unlock(&count_players_mutex); // Unlock mutex
	return 0;
}

int main(int argc, char** argv)
{
    pthread_t tid;
    int sfd, on = 1;
    sfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
    socklen_t sl;
    struct sockaddr_in saddr, caddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(1234);
    bind(sfd, (struct sockaddr*)&saddr, sizeof(saddr));
    listen(sfd, 10);

    initialize_games(3);

    SSL_library_init();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(TLS_server_method());
    if (!ctx) {
        printf("SSL context creation error.\n");
        return EXIT_FAILURE;
    }
    // Set certificate and key file paths
    if (SSL_CTX_use_certificate_file(ctx, "server.crt", SSL_FILETYPE_PEM) <= 0 ||
        SSL_CTX_use_PrivateKey_file(ctx, "server.key", SSL_FILETYPE_PEM) <= 0) {
        printf("Certificate or key loading error.\n");
        SSL_CTX_free(ctx);
        return EXIT_FAILURE;
    }

    pthread_mutex_lock(&can_play_mutex); //Noone can play at the start without an opponent
    while (1) {
        struct cln* c = malloc(sizeof(struct cln));
        sl = sizeof(c->caddr);
        c->cfd = accept(sfd, (struct sockaddr*)&c->caddr, &sl);
        if (c->cfd != -1) {
            pthread_mutex_lock(&count_players_mutex); // Lock mutex before incrementing player_count
            player_count++;
            //Don't allow too many players
            if (update_players_count(c->cfd + 1) == -1) {
                write(c->cfd, "E\n", 2); //Error message, no place for this player
                pthread_mutex_unlock(&count_players_mutex);
                //Don't create a new process
                continue;
            }
            pthread_mutex_unlock(&count_players_mutex);// Unlock mutex after modifying player_count
            pthread_create(&tid, NULL, cthread, c);
            pthread_detach(tid);
        }
    }

    SSL_CTX_free(ctx);
    close(sfd);
    return EXIT_SUCCESS;
}