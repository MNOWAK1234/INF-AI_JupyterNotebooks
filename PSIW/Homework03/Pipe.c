#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
   int status;

   char *ps_args[]={"ps", "-e", "-o", "uname", NULL};
   char *sort1_args[]={"sort", NULL};
   char *uniq_args[]={"uniq", "-c", NULL};
   char *sort2_args[]={"sort", "-n", NULL};
  
   int pipes[6];
   pipe(pipes);//1st
   pipe(pipes+2);//2nd
   pipe(pipes+4);//3rd
  
   // we now have 6 fds:
  // pipes[0] = read end of ps->sort1 pipe (read by sort1)
  // pipes[1] = write end of ps->sort1 pipe (written by ps)
  // pipes[2] = read end of sort1->uniq pipe (read by uniq)
  // pipes[3] = write end of sort1->uniq pipe (written by sort1)
  // pipes[4] = read end of uniq->sort2 pipe (read by sort2)
  // pipes[5] = write end of uniq->sort2 pipe (written by uniq)
   
   if(fork()==0)
   {
	   // replace ps's stdout with write part of 1st pipe
	   dup2(pipes[1], 1);
	   // close all ends of pipes
	   for(int j=0; j<6; j++)close(pipes[j]);
	   execvp(*ps_args, ps_args);
   }
   else
   {
	   // fork second child (to execute sort1)
	   if(fork()==0)
	   {
		   // replace sort1's stdin with read end of 1st pipe
		   dup2(pipes[0], 0);
		   // replace sort1's stdout with write end of 2nd pipe
		   dup2(pipes[3], 1);
		   // close all ends of pipes
		   for(int j=0; j<6; j++)close(pipes[j]);
		   execvp(*sort1_args, sort1_args);
	   }
	   else
	   {
		   // fork third child (to execute uniq)
		   if(fork()==0)
		   {
			   // replace uniq's stdin with read end of 2nd pipe
			   dup2(pipes[2], 0);
			   // replace uniq's stdout with write end of 3rd pipe
			   dup2(pipes[5], 1);
			   // close all ends of pipes
			   for(int j=0; j<6; j++)close(pipes[j]);
			   execvp(*uniq_args, uniq_args);
		   }
		   else
		   {
			   //fork fourth child (to execute sort2)
			   if(fork()==0)
			   {
				   //replace sort2's stdin with input read of 3rd pipe
				   dup2(pipes[4],0);
				   //close all ends of pipes
				   for(int j=0; j<6; j++)close(pipes[j]);
				   execvp(*sort2_args, sort2_args);
			   }
		   }
	   }
   }
   // only the parent gets here and waits for 3 children to finish
  for(int j=0; j<6; j++)close(pipes[j]);
  for(int i=0; i<4; i++)wait(&status);

}


