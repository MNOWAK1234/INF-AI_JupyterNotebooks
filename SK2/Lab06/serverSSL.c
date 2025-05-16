#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <openssl/ssl.h>

int main(int argc, char**argv)
{
    int sfd, cfd, rc = 0;
    char buf[1024];
    socklen_t sl;
    struct sockaddr_in sa, ca;
    SSL_CTX* ctx;
    SSL* ssl;
    SSL_load_error_strings();
    SSL_library_init();
    ctx = SSL_CTX_new(TLS_server_method());
    SSL_CTX_use_certificate_file(ctx, "server.crt", SSL_FILETYPE_PEM);
    SSL_CTX_use_PrivateKey_file(ctx, "server.key", SSL_FILETYPE_PEM);
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = INADDR_ANY;
    sa.sin_port = htons(1234);
    sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bind(sfd, (struct sockaddr*)&sa, sizeof(sa));
    listen(sfd, 10);
    while(1)
    {
        sl = sizeof(ca);
        cfd = accept(sfd, (struct sockaddr*)&ca, &sl);
        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, cfd);
        SSL_accept(ssl);
        rc = SSL_read(ssl, buf, sizeof(buf));
        if (rc > 0) {
            buf[rc+1] = '\0'; // Null-terminate the string
            printf("Received: %s\n", buf); // Print the contents of buf as a string
        } else {
            // Handle read errors or when nothing is read
            printf("No data received or read error!\n");
        }
        if(strncmp(buf, "151813", 6) == 0)
		{
			SSL_write(ssl, "Micktory02\n", 11);
		}
		else if(strncmp(buf, "151859", 6) == 0)
        {
            SSL_write(ssl, "Jezatek\n", 8);
        }
		else if(strncmp(buf, "151865", 6) == 0)
        {
            SSL_write(ssl, "Glowslaw\n", 9);
        }
		else if(strncmp(buf, "151883", 6) == 0)
        {
            SSL_write(ssl, "Aronia13\n", 9);
        }
		else if(strncmp(buf, "151816", 6) == 0)
        {
            SSL_write(ssl, "Kris_Ja\n", 8);
        }
		else
		{
			SSL_write(ssl, "Noone\n", 6);
		}
        SSL_free(ssl);
        close(cfd);
    }
    SSL_CTX_free(ctx);
    close(sfd);
    return EXIT_SUCCESS;
}
