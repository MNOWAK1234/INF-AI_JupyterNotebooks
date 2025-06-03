#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main(){
	for(int i=0; i<=1000000000; i++){
		if(i%100000000==0)printf("%d\n",i);
	}
	exit(0);
}
