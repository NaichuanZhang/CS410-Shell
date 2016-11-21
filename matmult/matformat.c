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
#define MAX_COLUMN 100 //define the max number of columns
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
    while (input_count != 1) {// only input the first matrix but not the second.  also only checks the columns numbers
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
    int mat_result[a_col][a_row];
    for (int x = 0; x < a_row; x++){
        for(int y = 0; y< a_col; y++){
            mat_result[y][x] = mat_a[x][y];
        }
    }
    for (int i = 0; i < a_col; i++) {
        for (int j = 0; j < a_row; j++) {
            printf("%d ", mat_result[i][j]);
        }
        printf("%c", '\n');
    }


}