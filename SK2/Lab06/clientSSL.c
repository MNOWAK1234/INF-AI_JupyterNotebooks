#define ERROR(e) { perror(e); exit(EXIT_FAILURE); }
#define SSLERROR { ERR_print_errors_fp(stderr); exit(EXIT_FAILURE); }

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
    int fd, rc;
    char buf[1024];
    struct sockaddr_in sa;
    struct hostent* he;
    SSL_CTX* ctx;
    SSL* ssl;

    if(argc != 4)
    {
        fprintf(stderr, "usage: %d HOST PORT: %d", argv[0]);
        exit(EXIT_FAILURE);
    }

    SSL_load_error_strings();
    SSL_library_init();
    if ((ctx = SSL_CTX_new(TLS_client_method())) == NULL) SSLERROR
    if ((ssl = SSL_new(ctx)) == NULL) SSLERROR
    if ((he = gethostbyname(argv[1])) == NULL) ERROR("gethostbyname()")
    if ((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) ERROR("socket()")
    sa.sin_family = AF_INET;
    sa.sin_port = htons(atoi(argv[2]));
    memcpy(&sa.sin_addr.s_addr, he->h_addr, he->h_length);
    if (connect(fd, (struct sockaddr*)&sa, sizeof(sa)) < 0) ERROR("connect()")
    SSL_set_fd(ssl, fd);
    SSL_connect(ssl);
    SSL_write(ssl, argv[3], 6);
    //SSL_write(ssl, "Hello, Server!", 15);
    rc = SSL_read(ssl, buf, sizeof(buf));
    write(1, buf, rc);
    SSL_free(ssl);
    close(fd);
    SSL_CTX_free(ctx);
    return EXIT_SUCCESS;
}
