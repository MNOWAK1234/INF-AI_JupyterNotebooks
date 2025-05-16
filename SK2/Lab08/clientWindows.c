#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

WSADATA wd;
SOCKET fd;
struct sockaddr_in sa;
struct hostent* he;
char buf[1024];

int main(int argc, char**argv)
{
    WSAStartup(MAKEWORD(2, 2), &wd);
    he = gethostbyname(argv[1]);
    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sa.sin_family = AF_INET;
    memcpy(&sa.sin_addr.s_addr, he->h_addr, he->h_length);
    sa.sin_port = htons(atoi(argv[2]));
    connect(fd, (struct sockaddr*)&sa,sizeof(sa));

    send(fd, argv[3], 7, 0);
    send(fd, "\n", 1, 0);
    recv(fd, buf, sizeof(buf), 0);
    printf(buf);

    closesocket(fd);
    WSACleanup();
}
