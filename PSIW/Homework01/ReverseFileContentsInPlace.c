#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define MAX 512/2

int main(int argc, char* argv[]){
   char buf[MAX];
   char buf2[MAX];
   int desc;
   int lbajt;
   char help;
 
   if (argc<2){
      fprintf(stderr, "Za malo argumentow. Uzyj:\n");
      fprintf(stderr, "%s <plik do obrocenia>\n", argv[0]);
      exit(1);
   }

   desc = open(argv[1], O_RDWR);
   if (desc == -1){
      perror("Blad otwarcia pliku zrodlowego");
      exit(1);
   }

   int rozm=lseek(desc,0,SEEK_END);
   int mod=rozm%MAX;
   int mid=rozm/2;
   int mem1=lseek(desc,mid,SEEK_SET);
   int mem2=lseek(desc,-mid,SEEK_END);
   while((lbajt = read(desc, buf, MAX)) > 0)
   {
      for(int i=0; i<lbajt/2; i++)
      {
	  help=buf[i];
	  buf[i]=buf[lbajt-1-i];
	  buf[lbajt-1-i]=help;
      }
      lseek(desc,mem1-lbajt,SEEK_SET);
      read(desc, buf2, lbajt);
      for(int i=0; i<lbajt/2; i++)
      {
          help=buf2[i];
          buf2[i]=buf2[lbajt-1-i];
          buf2[lbajt-1-i]=help;
      }
      lseek(desc,mem1-lbajt,SEEK_SET);
      write(desc,buf,lbajt);
      lseek(desc,mem2,SEEK_SET);
      write(desc,buf2,lbajt);
      mem2=mem2+lbajt;
      mem1=mem1-lbajt;
   }

   if (close(desc) == -1){
      perror("Blad zamkniecia pliku");
      exit(1);
   }

   exit(0);
}
