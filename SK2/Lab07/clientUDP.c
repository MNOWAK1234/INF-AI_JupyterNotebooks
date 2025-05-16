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
    char buf[1024];
    struct sockaddr_in sa;
    struct hostent* he;
    he = gethostbyname(argv[1]);
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    memcpy(&sa.sin_addr.s_addr, he->h_addr, he->h_length);
    sa.sin_port = htons(atoi(argv[2]));
    fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    connect(fd, (struct sockaddr*)&sa, sizeof(sa));
    write(fd, "Hello, Server!", 15);
    rc = read(fd, buf, sizeof(buf));
    write(1, buf, rc);
    close(fd);
}