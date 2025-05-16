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
	int fd, odczyt;
	char buf[256];
	struct hostent* addrent;
	addrent = gethostbyname(argv[1]);
	struct sockaddr_in saddr;
	fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(atoi(argv[2]));
	memcpy(&saddr.sin_addr.s_addr, addrent->h_addr, addrent->h_length);
	//saddr.sin_addr.s_addr = inet_addr(argv[1]);
	connect(fd, (struct sockaddr*) &saddr, sizeof(saddr));
	write(fd, argv[3], 6);
	odczyt = read(fd, buf, sizeof(buf));
	write(1, buf, odczyt);
	close(fd);
}
