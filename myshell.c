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
void pipes(int,int);

void interruptHandler(int signum){ //close foreground process. 

}
void closeTerminal(int signum){ //ctrl-d; exit terminal. AHHHHHHH how do we know if the user has entered ctrl-d??	Can signal with kill() but donn't know how to track user input that isn't text


}

int isSeq(char *token){	//checks last character to determine if the command is in a sequence of commands (i.e. ends with ';')
	if (token && *token){
  		if (token[strlen(token)-1] == ';')
	    		return 1;
  	}
	return 0;
}


void pipes(int i, int pipecount){
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
      }
      
    //execute command in child
    
    execvp(token, argv); // dont close fds on execve
    

//    if(!pipecount){
    
//    for(int i = 0;i < commands; i++)

//	}
  	

    waitpid(pid[i]);  //parent waits for child
     
*/    
  }
                    
int main(int argc, char *argv[]){
    int commands,pipecount,i=0;
    int fpfd[3];
    char *token,*command,line;
    pid_t child;
    char *cmd,*str_copy; // for pipe functionality
    FILE *fp;
    
        while(1){
 
        printf("myshell> ");
        
	if(!fgets(cmd, MAX_COUNT, stdin)){
		break;
	}

/*        
         if (feof(stdin) != 0) { 	// control + d
            return 0;
         }
*/
	str_copy = cmd;
          
        token = strtok(str_copy, " ");
         
          //********************************argv*******************
      		 
          // functionality for pipe code : COUNT THE PIPES
          
       	 
          while(token){
        
	    commands++;  
            token = strtok(NULL, " ");
            if(strcmp(token, "|") == 0){           
              	pipecount++;
            }
          
          	}
        //***************************************************
                

	token = strtok(cmd," ");
        while(token){ // checking the commands
        
          if(isSeq(token)){ 
          
            if((child=fork())==0){ //child (fork successful)
            	
              execvp(token,argv); //not the right syntax, but call command here
              
              }
            }
            else{ //parent, keep moving on
            
            	command = token;
              
              token = strtok(NULL," ");

              if ((strcmp(token, ">") == 0) || (strcmp(token, "1>") == 0)){

                token = strtok(NULL," ");
                //open file if exists, otherwise create. pipe stdout into it
                
		fp = fopen(token, "w+");

                if (!fp){

                  fprintf( stderr, "myshell error: can't open file.");
                  exit(1);
                }

		pipe(fpfd);

                //write stuff (redirect stdout of first command with dup2)

                dup2(fpfd[0], STDOUT_FILENO);
                    if ((child = fork()) == 0){ // make child process
                	execvp(command,argv);
                    }
                    else{
                    waitpid(child);
                    }
                fclose(fp);
                    
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
                    
                    if ((child = fork()) == 0){ // make child process
                				
                      execvp(command,argv);
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
                  
                if ((child = fork()) == 0){ // make child process
                    execvp(command,argv);
                  }
                  else{
                  waitpid(child);
                  }
              
              	fclose(fp);
               
              	}
              }
                    
              if (strcmp(token, "|") == 0){
              	
                	i++;
                	pipes(i, pipecount);
              }
                
               
              	}
              }
          
              
       }
