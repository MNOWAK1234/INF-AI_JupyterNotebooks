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

struct cln
{
	int cfd;
	struct sockaddr_in caddr;
};

int p = 0;
int pmax = 2;
char buf[256];

void* cthread(void* arg)
{
	struct cln* c = (struct cln*)arg;
	printf("[%lu] new connection from: %s:%d\n",
	(unsigned long int)pthread_self(),
	inet_ntoa((struct in_addr)c->caddr.sin_addr),
	ntohs(c->caddr.sin_port));
	read(c->cfd, buf, sizeof(buf));
	if(strcmp(buf, "151813") == 0)
	{
		write(c->cfd, "Micktory02\n", 11);
	}
	else if(strcmp(buf, "151859") == 0)
	{
		write(c->cfd, "Jezatek\n", 8);
	}
	else if(strcmp(buf, "151865") == 0)
	{
		write(c->cfd, "Glowslaw\n", 9);
	}
	else if(strcmp(buf, "151883") == 0)
	{
		write(c->cfd, "Aronia13\n", 9);
	}
	else if(strcmp(buf, "151816") == 0)
	{
		write(c->cfd, "Kris_Ja\n", 8);
	}
	else
	{
		write(c->cfd, "Noone\n", 6);
	}
	close(c->cfd);
	free(c);
	return 0;
}

int main(int argc, char**argv)
{
	pthread_t tid;
    int sfd, on = 1;
	sfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (char*)&on,sizeof(on));
	socklen_t sl;
	struct sockaddr_in saddr, caddr;
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = INADDR_ANY;
	saddr.sin_port = htons(1234);
	bind(sfd, (struct sockaddr*) &saddr, sizeof(saddr));
	listen(sfd, 10);
	while(1)
	{
		struct cln* c = malloc(sizeof(struct cln));
		sl = sizeof(c->caddr);
		while(p >= pmax)
		{
			sleep(1);
		}
		c->cfd = accept(sfd, (struct sockaddr*) &c->caddr, &sl);
		pthread_create(&tid, NULL, cthread, c);
		pthread_detach(tid);
	}
}
