#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

int _write(int sfd, char *buf, int len)
{
	while (len > 0) {
	int i = write(sfd, buf, len);
	len -= i;
	buf += i;
	}
}


int _read(int sfd, char *buf, int bufsize)
{
	int totalRead = 0;
	do
	{
		int i = read(sfd, buf + totalRead, bufsize);
		bufsize -= i;
		totalRead += i;
	} while (buf[totalRead - 1] != '\n' && bufsize > 0);
	return totalRead;
}

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
		_read(cfd, buf, sizeof(buf));
		if(strncmp(buf, "151813", 6) == 0)
		{
			_write(cfd, "Micktory02\n", 11);
		}
		else if(strncmp(buf, "151859", 6) == 0)
        {
            _write(cfd, "Jezatek\n", 8);
        }
		else if(strncmp(buf, "151865", 6) == 0)
        {
            _write(cfd, "Glowslaw\n", 9);
        }
		else if(strncmp(buf, "151883", 6) == 0)
        {
            _write(cfd, "Aronia13\n", 9);
        }
		else if(strncmp(buf, "151816", 6) == 0)
        {
            _write(cfd, "Kris_Ja\n", 8);
        }
		else
		{
			_write(cfd, "Noone\n", 6);
		}
		close(cfd);
	}
}
