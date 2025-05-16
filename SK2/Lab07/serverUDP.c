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
    int fd, rc;
    socklen_t sl;
    char buf[1024];
    struct sockaddr_in sa, ca;

    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = INADDR_ANY;
    sa.sin_port = htons(1234);
    fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    bind(fd, (struct sockaddr*)&sa, sizeof(sa));
    while(1)
    {
        sl = sizeof(ca);
        rc = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr*)&ca, &sl);
        printf("from=%s:%d size=%dB message:=%.*s", inet_ntoa((struct in_addr)ca.sin_addr), ntohs(ca.sin_port), rc, rc, buf);
        sendto(fd, "Micktory02\n", 11, 0, (struct sockaddr*)&ca, sl);
    }
}
