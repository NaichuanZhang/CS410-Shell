#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define MAX_COUNT 100

void interruptHandler(int);
int isSeq(char *);
void pipes(int,int,char*);

void interruptHandler(int signum){ //close foreground process. 

	printf("this is me interrupting your foreground process");

}

int isSeq(char *token){	//checks last character to determine if the command is in a sequence of commands (i.e. ends with ';')
	if (token && *token){
  		if (token[strlen(token)-1] == ';')
	    		return 1;
  	}
	return 0;
}


void pipes(int i, int pipecount,char *token){
/*
  //code from lecture to handle piping
     int fd[MAX_COUNT][2];
     int pid[MAX_COUNT];
        
  	   // make a pipe 
  			pipe(fd[i]); 
  
       // for(int j = 0; j < commands; j++){
        
  		if((pid[i]=fork()) == 0){ // child 
          	
            if(i==0){ //first command
        
            		dup2(fd[i][1], STDOUT_FILENO);
              	close(fd[i][0]);
            	}
          	}
          
      
          else if(i == pipecount - 1){ //last command
          
        		//redirect pipe to std-in
            
            if(pipecount){
            	
              dup2(fd[i-1][0], STDIN_FILENO);
              close(fd[i-1][1]); //write end
            }
          }
          else{ // nested command ie not last command
            	
              //redirect both stdin & stdout
              if(pipecount){
    
		dup2(fd[i-1][0], STDIN_FILENO);
                dup2(fd[i][1], STDOUT_FILENO);
                close(fd[i-1][1]);
                close(fd[i][0]);
                
              }
        	}
       
    //execute command in child
    
    execvp(token, token[1]); // dont close fds on execve
    
//    if(!pipecount){
    
//    for(int i = 0;i < commands; i++)

//	}
  	

    waitpid(pid[i]);  //parent waits for child
 */    
    
  }     

                    
int main(int argc, char *argv[]){
    int commands,pipecount,i=0;
    int fpfd[3];
    char *token,*command;
    char cmd[MAX_COUNT],copy[MAX_COUNT];
    pid_t child;
    FILE *fp;
    char *ars[2];
    
        while(1){
 
	signal(SIGINT, interruptHandler);

        printf("myshell> ");
        
	if(fgets(cmd, MAX_COUNT, stdin) == NULL){
		break;
	}

/*        
         if (feof(stdin) != 0) { 	// control + d
            return 0;
         }
*/
	strcpy(copy,cmd);
          
        token = strtok(copy, " ");
          //********************************argv*******************
      		 
          // functionality for pipe code : COUNT THE PIPES
          
       	 
          while(token){
        
	    commands++;  
 
            if(strcmp(token, "|") == 0){           
           		pipecount++;
		}

	    token = strtok(NULL, " ");

          
          }
        //***************************************************
                

	token = strtok(cmd," ");
	ars[0] = token;
	ars[1] = NULL;
        while(token){ // checking the commands
        
          if(isSeq(token)){ 
          
		child = fork();
            if(child==0){ //child (fork successful)
            	
              execvp(token,ars); //not the right syntax, but call command here
              
              }
            }
            else{ //parent, keep moving on
            
              command = token;
              
              token = strtok(NULL," ");

		if(token){
              	if ((strcmp(token, ">") == 0) || (strcmp(token, "1>") == 0)){

                	token = strtok(NULL," ");
                //open file if exists, otherwise create. pipe stdout into it
                	if(token){
			fp = fopen(token, "w+");

                	if (!fp){

                 	 fprintf( stderr, "myshell error: can't open file.");
                  	exit(1);
                	}

			pipe(fpfd);

                //write stuff (redirect stdout of first command with dup2)

               		dup2(fpfd[0], STDOUT_FILENO);
                	fclose(fp);
			child = fork();
                    	if (child == 0){ // make child process
                		execvp(command,ars);
                    	}
                    	else{
                    	waitpid(child);
                    	}
                    	}
			else{
				printf("Panic: So many commands D:\n");
			}
		}

				
              
              if (strcmp(token, "<") == 0){
              
              	token = strtok(NULL," ");


                //open file if exists, otherwise create. pipe stdout into it
                fp = fopen(token, "r");

                if (!fp){

                  fprintf( stderr, "myshell error: can't open file.");
                  exit(1);
                }
		
		pipe(fpfd);

                //read in stuff (redirect stdin of first command with dup2)
		dup2(fpfd[1], STDIN_FILENO);
                fclose(fp);
                    child = fork();
                    if (child == 0){ // make child process
                				
                      execvp(command,ars);
                    }
                    
                    else{
                    	
                      waitpid(child);
                    }
              }
              
              if (strcmp(token, "2>") == 0){
              
                token = strtok(NULL," ");

                //open file if exists, otherwise create. pipe stdout into it
                fp = fopen(token, "w+");

                if (!fp){

                  fprintf( stderr, "myshell error: can't open file.");
                  exit(1);
                  
                }

		pipe(fpfd);
                
                //write stuff (redirect stdout of first command with dup2)

                dup2(fpfd[2], STDERR_FILENO);
                fclose(fp);
                 child = fork();
                if (child == 0){ // make child process
                    execvp(command,ars);
                  }
                  else{
                  waitpid(child);
                  }
              	}
                    
              if (strcmp(token, "|") == 0){
              	
                	i++;
                	pipes(i, pipecount,token);
              }
	}
		else{ //only command, execute
			child = fork();
                	if (child == 0){ // make child process
				printf("%s\n",command);
                		execvp(command,ars);
                    	}
                    	else{
                    	child = waitpid(child);
                    	}
		}
                
               
              	}
              }
	}
}
          
              

