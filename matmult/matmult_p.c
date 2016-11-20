#include <stdio.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#define MAX_ROW 100 //define the max number of rows
#define MAX_COLUMN 100 //define the max number of 
//check token numbers -- columns
int token_count(char *row_cpy) {
    int n_tok = 0;
    char *token = strtok(row_cpy, " ");
    while (token) {
        n_tok++;
        token = strtok(NULL, " ");
    }
    return n_tok;
}

int main(){
	int buffsize = 1024;
	char buff[buffsize];
	char mat[buffsize*buffsize];
    int mat_b[MAX_ROW][MAX_COLUMN], mat_a[MAX_ROW][MAX_COLUMN];
    int input_count = 0, a_row = 0, a_col = 0, b_row = 0, b_col = 0;
    int i, j, c_row, c_col;
    char *row = (char *) malloc(sizeof(char) * 128);
    char *row_cpy = (char *) malloc(sizeof(char) * 128);
    //the code under is how you parse through the whole arguments
    while (input_count != 2) {
        fgets(row, 128, stdin);
        row[strlen(row) - 1] = 0;
        //using strcpy() to avoid "/n" situations
        strcpy(row_cpy, row);
        //use strlen to check the input
        if (strlen(row) == 0) {// empty line
            input_count++; // This input count is the divider to decide which matrix the input will go in
        } else if (input_count == 0) {// input for mat_a
            // sets the number of columns of mat_a
            printf("you just put a row into matrix_a\n");
            if (a_col == 0) {
                a_col = token_count(row_cpy);
                strcpy(row_cpy, row);
                int data = 0, token_track = 0;
                char *token = strtok(row_cpy, " ");
                while (token) {
                data = atoi(token);
                mat_a[a_row][token_track] = data;
                token_track++;
                token = strtok(NULL, " ");
                }
            } else if (token_count(row_cpy) != a_col) { // check if the input of columns is consistant
                printf("Please put in the same number of columns!!!!\n");
                exit(0);
            } else {
                strcpy(row_cpy, row);  // copy the data into the row

                int data = 0, token_track = 0;
                char *token = strtok(row_cpy, " ");
                while (token) {
                data = atoi(token);
                mat_a[a_row][token_track] = data;
                token_track++;
                token = strtok(NULL, " ");
                }
            }
            a_row++;
        } else if (input_count == 1) {                   // mat_b
            // for the second matrix
            printf("you just put a row into matrix_b\n");
            if (b_col == 0) {
                b_col = token_count(row_cpy);
                strcpy(row_cpy, row);
                int data = 0, token_track = 0;
                char *token = strtok(row_cpy, " ");
                while (token) {
                data = atoi(token);
                mat_b[b_row][token_track] = data;
                token_track++;
                token = strtok(NULL, " ");
                }
            } else if (token_count(row_cpy) != b_col) {// checks the input columns 
                printf("Please put in the same number of columns!!!!\n");
                exit(0);
            } else {
                strcpy(row_cpy, row);
                int data = 0, token_track = 0;
                char *token = strtok(row_cpy, " ");
                while (token) {
                data = atoi(token);
                mat_b[b_row][token_track] = data;
                token_track++;
                token = strtok(NULL, " ");
                }
            }

            b_row++;
        }
    }
    buffsize = 10;
	char buff_shmid[buffsize];
	char buf_i[buffsize];
	char buf_j[buffsize];
	char buf_a_row[buffsize];
	char buf_b_col[buffsize];
	char buf_multual[buffsize];


	int mat_c[a_row][b_col];  //init the result matrix

	int *shm_mem; // init the shared memory
	int shmid;
	// calculat the size of the shared memory based on the data to be stored -- size of all the three matrix

	/* 
	The structure for the shared memory structure.
	| mat_a | mat_b | mat_c |
	*/
	int shm_size =sizeof(mat_a)+sizeof(mat_b)+sizeof(mat_c); //the first three int is for m(a_col or b_row), a_row,b_col and  in the multiply.c

	// get the shmid
	if ((shmid = shmget(IPC_PRIVATE, shm_size, 0666)) < 0) {
		printf("Error: Failed to get the shared memory ID!\n");
		printf("%s\n", strerror(errno)); // error message
		exit(errno);
	}

	// get the pointer to shared memory
    if ((shm_mem = (int *)shmat(shmid, 0, 0)) == (int *)-1) {
		printf("Error: Failed to get the pointer to shared memory!\n");
		printf("%s\n", strerror(errno));
		exit(errno);
    }
    int * p =shm_mem;
    // put data into the shared memory
    // set the pointer positions to point to the right thing
    for (int i = 0; i < a_row; i++) {
        for (int j = 0; j < a_col; j++) {
            *p = mat_a[i][j];
            p++;
        }
    }
    for (int i = 0; i < b_row; i++) {
        for (int j = 0; j < b_col; j++) {
            *p = mat_b[i][j];
            p++;
        }
    }
	// create child processes  matmult_p!!!
	for(int i=0; i<a_row; i++){ // at the end of the for loop i = a_row
		for(int j=0; j<b_col; j++){ // at the end of the for loop j = b_col
				int pid = fork();
				if(pid == 0){// child process
				// pass the data into multiply
				snprintf(buff_shmid, buffsize, "%d", shmid); // give shmid to shmid buffer
				snprintf(buf_i, buffsize, "%d", i);  //for each process , i indicates the right row for mat_a
				snprintf(buf_j, buffsize, "%d", j);  //for each process, i indicates the right columns for mat_b
				snprintf(buf_a_row, buffsize, "%d", a_row); //pass in the permanent value for the row number of mat_a
				snprintf(buf_b_col, buffsize, "%d", b_col); //pass in the permanent value for column number of mat_b
				snprintf(buf_multual, buffsize, "%d", a_col); //for each process, buf_mutual is the process_number that you need
				//create the args array to pass into multiply
				char * args [8] = {"./multiply", buff_shmid, buf_i, buf_j, buf_a_row, buf_b_col, buf_multual, (char*)NULL};
	             	if (execvp(args[0], args) < 0) {
						printf("Failed to run ./multiply\n");
						printf("%s\n", strerror(errno));
						exit(errno);
	                }
					perror("Error. Should exit out already!!!\n");
					exit(1);
				}
				else{
					continue;
				}
		}
	}
	// wait for all the processes
	for(int i=0; i<a_row; i++){
		for(int j=0; j<b_col; j++){
			wait(NULL);
		}
	}

	//print out the result 
	for (int i = 0; i < a_row; i++) {
		for (int j = 0; j < b_col; j++) {
		    printf("%d ", *p);
		    p++;
		}
		printf("%c", '\n');
	}

	// IPC_RMID  Mark the segment to be destroyed. 
	shmctl(shmid, IPC_RMID, NULL);

	return 0;
}