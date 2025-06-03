#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define MAX 512

int main(int argc, char* argv[]){
   char buf[MAX];
   int desc_zrod, desc_cel;
   int lbajt;
   char help;
 
   if (argc<3){
      fprintf(stderr, "Za malo argumentow. Uzyj:\n");
      fprintf(stderr, "%s <plik zrodlowy> <plik docelowy>\n", argv[0]);
      exit(1);
   }

   desc_zrod = open(argv[1], O_RDONLY);
   if (desc_zrod == -1){
      perror("Blad otwarcia pliku zrodlowego");
      exit(1);
   }

   desc_cel = creat(argv[2], 0640);
   if (desc_cel == -1){
      perror("Blad utworzenia pliku docelowego");
      exit(1);
   }

   int rozm=lseek(desc_zrod,0,SEEK_END);
   int mod=rozm%MAX;
   lseek(desc_zrod,0,SEEK_SET);
   lseek(desc_cel,rozm,SEEK_SET);
   while((lbajt = read(desc_zrod, buf, MAX)) > 0)
   {
      lseek(desc_cel,-lbajt,SEEK_CUR);
      for(int i=0; i<lbajt/2; i++)
      {
	  help=buf[i];
	  buf[i]=buf[lbajt-1-i];
	  buf[lbajt-1-i]=help;
      }
      if (write(desc_cel, buf, lbajt) == -1){
         perror("Blad zapisu pliku docelowego");
         exit(1);
      }
      lseek(desc_cel,-lbajt,SEEK_CUR);
   }
   if (lbajt == -1){
      perror("Blad odczytu pliku zrodlowego");
      exit(1);
   }

   if (close(desc_zrod) == -1 || close(desc_cel) == -1){
      perror("Blad zamkniecia pliku");
      exit(1);
   }

   exit(0);
}
