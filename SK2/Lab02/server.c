#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char**argv)
{
    int sfd, cfd, on = 1;
	char buf[256];
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
		sl = sizeof(caddr);
		cfd = accept(sfd, (struct sockaddr*) &caddr, &sl);
		printf("new connection from %s:%d\n",
				inet_ntoa((struct in_addr)caddr.sin_addr),
				ntohs(caddr.sin_port));
		read(cfd, buf, sizeof(buf));
		if(strcmp(buf, "151813") == 0)
		{
			write(cfd, "Micktory02\n", 11);
		}
		else if(strcmp(buf, "151859") == 0)
        {
            write(cfd, "Jezatek\n", 8);
        }
		else if(strcmp(buf, "151865") == 0)
        {
            write(cfd, "Glowslaw\n", 9);
        }
		else if(strcmp(buf, "151883") == 0)
        {
            write(cfd, "Aronia13\n", 9);
        }
		else if(strcmp(buf, "151816") == 0)
        {
            write(cfd, "Kris_Ja\n", 8);
        }
		else
		{
			write(cfd, "Noone\n", 6);
		}
		close(cfd);
	}
}
