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

int main(int argc, char**argv)
{
    char buf[1024];
    socklen_t slt;
    int sfd, cfd, fdmax, fda, rc, i;
    struct sockaddr_in saddr, caddr;
    static struct timeval timeout;
    fd_set mask, rmask, wmask;
    fd_set wait4read, wait4write;
    fd_set name1, name2, name3, name4, name5, nikt;

    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(1234);
    sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bind(sfd, (struct sockaddr*)&saddr, sizeof(saddr));
    listen(sfd, 10);
    FD_ZERO(&mask);
    FD_ZERO(&rmask);
    FD_ZERO(&wmask);
    FD_ZERO(&wait4read);
    FD_ZERO(&wait4write);
    FD_ZERO(&name1);
    FD_ZERO(&name2);
    FD_ZERO(&name3);
    FD_ZERO(&name4);
    FD_ZERO(&name5);
    FD_ZERO(&nikt);
    fdmax = sfd;
    while(1)
    {
    	wmask = wait4write;
        rmask = wait4read;
        FD_SET(sfd, &rmask);
        timeout.tv_sec = 5 * 60;
        timeout.tv_usec = 0;
        rc = select(fdmax+1, &rmask, &wmask, (fd_set*)0, &timeout);
        if (rc == 0) continue;
        fda = rc;
        if (FD_ISSET(sfd, &rmask))
        {
            fda -= 1;
            slt = sizeof(caddr);
            cfd = accept(sfd, (struct sockaddr*)&caddr, &slt);
            printf("new connection from %s:%d\n",
				inet_ntoa((struct in_addr)caddr.sin_addr),
				ntohs(caddr.sin_port));
            FD_SET(cfd, &wait4read);
            if (cfd > fdmax) fdmax = cfd;
        }
        for (i = sfd+1; i <= fdmax && fda > 0; i++)
        {
            if (FD_ISSET(i, &rmask))
            {
                fda -= 1;
                read(cfd, buf, sizeof(buf));
                if(strcmp(buf, "151813") == 0)
                {
                    FD_SET(i, &name1);
                }
                else if(strcmp(buf, "151859") == 0)
                {
                    FD_SET(i, &name2);
                }
                else if(strcmp(buf, "151865") == 0)
                {
                    FD_SET(i, &name3);
                }
                else if(strcmp(buf, "151883") == 0)
                {
                    FD_SET(i, &name4);
                }
                else if(strcmp(buf, "151816") == 0)
                {
                    FD_SET(i, &name5);
                }
                else
                {
                    FD_SET(i, &nikt);
                }
                FD_SET(i, &wait4write);
                FD_CLR(i, &wait4read); 
                if (i == fdmax)
                    while(fdmax > sfd && !FD_ISSET(fdmax, &wait4read) && !FD_ISSET(fdmax, &wait4write))
                        fdmax -= 1;
            }
        }
        for (i = sfd+1; i <= fdmax && fda > 0; i++)
        {
            if (FD_ISSET(i, &wmask))
            {
                fda -= 1;
                if(FD_ISSET(i, &name1))
                {
                    write(cfd, "Micktory02\n", 11);
                    FD_CLR(i, &name1);
                }
                else if(FD_ISSET(i, &name2))
                {
                    write(cfd, "Jezatek\n", 8);
                    FD_CLR(i, &name2);
                }
                else if(FD_ISSET(i, &name3))
                {
                    write(cfd, "Glowslaw\n", 9);
                    FD_CLR(i, &name3);
                }
                else if(FD_ISSET(i, &name4))
                {
                    write(cfd, "Aronia13\n", 9);
                    FD_CLR(i, &name4);
                }
                else if(FD_ISSET(i, &name5))
                {
                    write(cfd, "Kris_Ja\n", 8);
                    FD_CLR(i, &name5);
                }
                else
                {
                    write(cfd, "Noone\n", 6);
                    FD_CLR(i, &nikt);
                }
                close(i);
                FD_CLR(i, &wmask);
                FD_CLR(i, &wait4write);
                if (i == fdmax)
                    while(fdmax > sfd && !FD_ISSET(fdmax, &wait4read) && !FD_ISSET(fdmax, &wait4write))
                        fdmax -= 1;
            }
        }
    }
    close(sfd);
    return 0;
}
