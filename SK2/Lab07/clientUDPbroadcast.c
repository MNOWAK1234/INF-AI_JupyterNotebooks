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
    int fd, rc, on = 1;
    socklen_t sl;
    char buf[1024];
    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = INADDR_BROADCAST;
    sa.sin_port = htons(atoi(argv[1]));
    fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
    sendto(fd, "Micktory02\n", 11, 0, (struct sockaddr*)&sa, sizeof(sa));
    sl = sizeof(sa);
    for(int i=0; i<16; i++)
    {
        rc = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr*)&sa, &sl);
        printf("from=%s:%d size=%dB message:=%.*s", inet_ntoa((struct in_addr)sa.sin_addr), ntohs(sa.sin_port), rc, rc, buf);
    }
    close(fd);
}