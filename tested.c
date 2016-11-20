                      
//tested code
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
	      
	token = strtok(line, " ");
        
        int commands = 0; // number of commands counted
       
        while(token){ // checking the commands
        
	    commands++;
           
	    FILE *fp;

	    //open file if exists, otherwise create. pipe stdout into it
	    fp = fopen(token, "w+");

	    if ( fp == -1 ){

		    fprintf( stderr, "myshell error: can't open file.");
		    exit(1);
                
		}
             
		dup2(fp, STDOUT_FILENO);
                  
		if ((child = fork()) == 0){ // make child process
		    execvp(command,argv);
                }
                else{
		    waitpid(child);
                }
              
		fclose(fp);
               
        
		printf(cmd);
		
		if(!line){
		    break;
		}
		}
		
    }

}
