/* Tiffany Yu, Ivan Lomeli
 * CS 31 Lab 6: Game of Life.
 * Given a correctly formatted text file, this program plays the Game of Life
 * based on the information provided from the text file. The user must specify
 * the text file they'd like to use and can choose whether they'd like to see
 * each iteration or only the end result.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>

/* Declaring functions and their types */
int *init_board(int live[], int row, int column, int num_pairs);
void GOL(int board[], int row, int column);
int life_of_cell(int board[], int x, int y, int row, int column);
void print_result(int matrix[], int size, int column);
int * extractLiveCells(int num_pairs,FILE *infile);

int main (int argc, char **argv) {
  /* Checks that the user passed in the correct number of
   * command line arguments */
  if(argc != 3) {
    printf("Correct usage: %s <filename.txt> <0/1, depending on your preference.>\n", argv[0]);
    exit(1);
  }
  /*Creating variables to save information from text file */
  int row, column, num_pairs, iterations, ret;
  FILE *infile;
  /*Opens the file and errors if the file cannot be opened. */
  infile = fopen(argv[1], "r");
  if (infile == NULL) {
      perror("Error: Unable to open file...\n");
      exit(1);
  }
  /* Reads values from the text file and puts them into their corresponding
  * variables*/
  ret = fscanf(infile, "%d %d %d %d", &row, &column, &iterations, &num_pairs);
  if (ret != 4) {
    printf("Error: Invalid text file...\n");
    exit(1);
  }
  int option = atoi(argv[2]);
  /* Saving coordinates indicating initial live cells from text file */
  int *live, *matrix;
  live = extractLiveCells(num_pairs, infile);
  if (live == NULL) {
    printf("Error: malloc failed... \n");
    exit(1);
  }
  /* Itializes board */

  matrix = init_board(live, row, column, num_pairs);
  printf("initial board\n");
  print_result(matrix, row*column, column);
  if (matrix == NULL) {
    printf("Error: malloc failed... \n");
    exit(1);
  }

  /* Calculates time elapsed for either scenario */
  struct timeval start, end;
  gettimeofday(&start, NULL);
  if (option == 0) {
    printf("\n\n");
    for (int i = 0; i < 1; i++) {
      GOL(matrix, row, column);
    }
  }
  if (option == 1) {
    for (int j=0; j< 3; j++) { //TODO
      printf("\n");
      GOL(matrix, row, column);
      system("clear");
      print_result(matrix, row*column, column);
      usleep(200000);
    }
  }
  //system("clear");
  //printf("\n\n");
  //print_result(matrix, row*column, column);
  gettimeofday(&end, NULL);
  float elapsed = (end.tv_sec - start.tv_sec)*1000000   + end.tv_usec - start.tv_usec;
  printf("total time for %d iterations of %dx%d is %f secs\n",iterations, row, column, (elapsed/1000000) );
  /*frees memory from dynamic arrays */
  free(matrix);
  matrix = NULL;
  free(live);
  live = NULL;

  return 0;
}

/* Returns an array that contains the live pair coordinates or NULL if unable to.
 * Quits if the given text file is formatted incorrectly. */
int * extractLiveCells(int num_pairs, FILE *infile) {
  int *live;
  int i, j, ret;
  live = (int *)malloc(sizeof(int)*num_pairs*2);
  if(live == NULL){
    return NULL;
  }
  ret = fscanf(infile, "%d %d", &i, &j);
  if (ret != 2 && ret != EOF) {
    printf("Invalid text file.\n");
    exit(1);
  }
  live[0] = i;
  live[1] = j;
  int k = 2;
  while (ret != EOF){
      ret = fscanf(infile, "%d %d", &i, &j);
      if (ret != EOF) {
        live[k] = i;
        live[k+1] = j;
        k += 2;
      }
  }
  fclose(infile);
  return live;
}

/* Initializes the maze. First makes all the values in the matrix zero, then
* looks at the list of live cells and replaces their value to 1 */
int * init_board(int live[], int row, int column, int num_pairs) {
  int *matrix;
  matrix = (int *)malloc(sizeof(int)*row*column);
  if(matrix== NULL){
    return NULL;
  }
  for (int a=0;a<row;a++){
    for(int b=0;b<column;b++){
      matrix[a*column+b] = 0;
    }
  }
  for (int i=0;i<num_pairs*2;i= i+2) {
    int x = live[i];
    int y = live[i+1];
    matrix[x*column+y] = 1;
  }
  return matrix;
}

/*Implements the Game of life*/
void GOL(int board[], int row, int column) {
    /* Sets up two arrays containing dead and alive cells*/
    int size = row*column;
    int *b_alive, *b_dead;
    b_alive = (malloc(sizeof(int)*size));
    if (b_alive == NULL) {
      printf("Error: malloc failed... \n");
      exit(1);
    }
    b_dead = (malloc(sizeof(int)*size));
    if (b_dead == NULL) {
      printf("Error: malloc failed... \n");
      exit(1);
    }
    int a = 0;
    int d = 0;
    for (int j=0;j<row;j++){
        for (int k=0;k<column;k++){
                int index = j*column+k;
                int value = board[index];
                int result = life_of_cell(board, j, k, row, column);
                if (index == 10){
                    printf("index: %d\n", index);
                    printf("value: %d\n", value);
                    printf("result: %d\n", result);
                }
                if (result < 2 && value == 1){
                    b_dead[d] = index;
                    d += 1;
                //If more than four live cells around cell is dead
              } else if (result > 4 && value == 1){
                    b_dead[d] = index;
                    d += 1;
                //If dead cell is surrounded by 3 live cells, cell becomes alive
                } else if (value == 0 && result == 3){
                    b_alive[a] = index;
                    a += 1;
                } //Otherwise cell stays at same state*/
              }
        }
    printf("value after1: %d\n",board[10]);
    for (int r =0;r<a;r++){
        int c_index = b_alive[r];
        printf("alive c_index: %d\n",c_index);
        board[c_index] = 1;
    }
    for (int r =0;r<d;r++){
        int c_index = b_dead[r];
        printf("dead c_index: %d\n",c_index);
        board[c_index] = 0;
    }
    printf("value after: %d\n",board[10]);
    /* Frees memory */
    free(b_dead);
    free(b_alive);
    b_dead = NULL;
    b_alive = NULL;
}

/* Checks if the cell is alive or not in this iteration, changes the value of
* the cell accordingly */
int life_of_cell(int matrix[], int y, int x, int row, int column){
    //Makes sure it acts like a taurus
    int result = 0;
    int left_v = x-1;
    int right_v = x+1;
    int up_v = y-1;
    int down_v = y+1;

    if (x == 0){
        left_v = column-1;
    } else if (x == column-1){
        right_v = 0;
    }
    if (y == 0){
        up_v = row-1;
    } else if (y == row-1){
        down_v = 0;
    }

    //Calculates the coordinates of the 8 neighbors of the cell
    int left = matrix[y*column+left_v];
    int right = matrix[y*column+right_v];
    int up = matrix[up_v*column+x];
    int left_up = matrix[up_v*column+left_v];
    int right_up = matrix[up_v*column+right_v];
    int down = matrix[down_v*column+x];
    int left_down = matrix[down_v*column+left_v];
    int right_down = matrix[down_v*column+right_v];
    result = left+right+up+left_up+right_up+down+left_down+right_down;
    return result;
}

/*prints the result board*/
void print_result(int matrix[], int size, int column){
  for (int i=0; i< size; i++) {
    if ((i+1)%column == 0 ) {
      printf("%d\n", matrix[i]);
    } else {
      printf("%d", matrix[i]);
    }
  }
}
