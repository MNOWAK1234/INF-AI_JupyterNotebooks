#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char* argv[]){
	pid_t pid[argc];
	int status[argc];
	close(1);
   	creat("log.txt", 0600);
	close(2);
	dup2(1,2);
	for(int i=1; i<argc; i++)
	{
		pid[i]=fork();
                if(pid[i]==0)
		{
                        execlp(argv[i],argv[i], NULL);
                        perror("Blad uruchomienia programu");
                        exit(1);
                }
	}
	int i=1;
	for(;;)
	{
		//printf("%d\n", pid[i]);
		if (waitpid(pid[i], &status[i], WNOHANG) != 0)
		{
			pid[i]=fork();
			if(pid[i]==0)
	                {
        	                execlp(argv[i],argv[i], NULL);
                	        perror("Blad uruchomienia programu");
                       		exit(1);
			}
		}
		i++;
		if(i==argc)i=1;
	}
	return 0;
}
