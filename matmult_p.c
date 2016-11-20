#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define MAX_COUNT 100

int intbuf[2000];


int parseArray(char *buf[], char *first){

	char *tok;
	char line[MAX_COUNT];
	int rows=0;
	int y=0;

	fflush(stdout);
	tok = strtok(first, " ");
	if (strcmp(tok,"\n") == 0){
		if(fgets(line, MAX_COUNT, stdin) == NULL)
			return -1;
		tok = strtok(line," ");
	}
	while(1){
		while(tok){
			if(strcmp(tok, "\n") == 0){
			  rows++;
			  break;
			}
			buf[y] = tok;
			y++;
			tok = strtok(NULL, " ");
		}	
		if(fgets(line, MAX_COUNT, stdin) == NULL)
			break;
		tok = strtok(line," ");
		if (strcmp(tok,"\n") == 0)
			break;
	}

	return rows;

}


void main(){

// read two matrices from stdin, preform error checking
char *ptr, *token;
char line [MAX_COUNT],line2[MAX_COUNT];
char *a1_buf [2000]; // preliminary value
char *a2_buf [2000];
int row2_count,row1_count,col1_count;

int x =0;
int y = 0;
int z;

char *num;

while(1){
/*		
	fflush(stdout);
	if(fgets(line, MAX_COUNT, stdin) == NULL)

		return;
	
	num = strtok(line," ");
  
	if(strcmp(token, "\n") == 0){
	printf("now input next array:\n");
	while(1){
        	fflush(stdout);
        	if(fgets(line2, MAX_COUNT, stdin) == NULL)
                	return;
		
		num = strtok(line2," ");
		while(num){
			if(strcmp(num, "\n") == 0){
			  break;
			}
			array2_buf[y] = (num);
			y++;
			num = strtok(NULL, " ");
			row2_count++; //columns
             
		}
	}
	}
	
	while(num){
		//puts(line);
		// convert each token to an int
		token = strtok(NULL, " ");
		
		if(token == NULL)
	                break;

		array1_buf[x] = (token);
		x++;

		// see if you are in the next column
		if(strcmp(token, "/n") == 0){
			col1_count++;	
		}
	}
}
*/


	if(fgets(line,MAX_COUNT,stdin) == NULL)
		break;
	token = strtok(line," ");
	if(strcmp(token,"\n") == 0){
		row2_count = parseArray(a2_buf,line);
		break;
	}

	row1_count = parseArray(a1_buf,line);
}


printf("row1Count of matrices\n");
printf("%d\n",row1_count);

printf("row2Count of matrices\n");
printf("%d\n",row2_count);

printf("%d %d %d %d",a1_buf[0],a1_buf[3],a2_buf[0],a2_buf[3]);

// verify the correctness of the matrices
if(col1_count != row2_count){

	printf("Matrices can not be multiplied: TRY AGAIN MOFOS");

}


}
