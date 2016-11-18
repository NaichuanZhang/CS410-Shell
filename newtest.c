#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define MAX_COUNT 100
void main(){
    char cmd[MAX_COUNT];	
    int commands,pipecount;
    int fd[MAX_COUNT][2];
    int pid[MAX_COUNT];
    char *token,command,line;
    pid_t child;
    char *str_copy; // for pipe functionality
    while(1){
 
        printf("myshell> ");
        
        fgets(line, MAX_COUNT, stdin);
	printf(cmd);
	if(!line){
            break; 
	}
    }
}
