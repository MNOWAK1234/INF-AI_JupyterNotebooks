#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

int getbuffsize(int sfd, int buffname)
{
    int s;
    socklen_t sl = (socklen_t)sizeof(s);
    getsockopt(sfd, SOL_SOCKET, buffname, (void*)&s, &sl);
    return s;
}

void buffsizes(int sfd, int *srb, int *ssb)
{
    *srb = getbuffsize(sfd, SO_RCVBUF);
    *ssb = getbuffsize(sfd, SO_SNDBUF);
}

int main(int argc, char**argv)
{
    int tcp, tcprb, tcpsb, udp, udprb, udpsb;
    tcp = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    buffsizes(tcp, &tcprb, &tcpsb);
    udp = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    buffsizes(udp, &udprb, &udpsb);
    printf("%d %d %d %d \n", tcprb, tcpsb, udprb, udpsb); //prints buffer size
}
