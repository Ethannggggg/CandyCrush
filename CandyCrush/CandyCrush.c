
//
/* Header File Declaration */
#include <stdio.h>
#include <stdlib.h>
/* NO other header files are allowed */
//
//
/* Given Global Variables and Definition */
#define H 6  // height
#define W 6  // width

char candies[] = {'*', '#', '@', '%'};
/* NO other global variables are allowed */
//

//

/* Part I Functions */
void initGameBoard(int board[][W], int board_sample[][W]);
void printGameBoard(int board[][W]);
int askForSwap(int board[][W]);
void swap(int board[][6], int row1, int col1, int row2, int col2) ;
int findAndRemoveMatch(int board[][W], int row, int col);
int isMatching(int board[6][6], int row, int col);
/* Part II Functions */
int initGameBoardFromFile(int board[][W], int stacks[]);
void applyGravity(int board[][W]);
int fillEmpty(int board[][W], int stacks[], int current, int numCandies);
int cascade(int board[][6], int stacks[], int current, int numCandies);

//
// Function Definitions/

/**
 * Main() will call this function in the beginning.
 * initGameBoard() load the game board with the pre-defined board_sample array
 * @param board The gameboard the game is using
 * @param board_sample The given game board sample values
 */
void initGameBoard(int board[H][W], int board_sample[][W]) {
   for(int i=0;i<H;i++) for(int j=0;j<W;j++) board[i][j]=board_sample[i][j];
}

/**
 * [Part II]
 * Main() will call this function in the beginning.
 * initGameBoardFromFile will load the file "board.txt" to load two information:
 * (1) Game Board ; (2) Candy Stacks
 * If the file loading is unsuccessful, quit the program with the error message.
 * @param board The Game board the program is using.
 * @param stacks The stack array to store candies. It will be inititalized from the file.
 * @return The number of candies in the stack.
 */
int initGameBoardFromFile(int board[][W], int stacks[]) {
    int num=0;
    int array[200];
    char check;
    FILE *fptr;
    fptr = fopen("board.txt","r");
    if(fptr == NULL){
        printf("Failed to open board.txt!\n");
        exit(-1);
    }
    while(fscanf(fptr,"%d", &array[num])!=EOF){
        num++;
    }
    fclose(fptr);
   // int array[]= {6 ,6 ,1 ,3 ,2 ,0 ,1 ,0 ,2 ,2, 0, 1, 1, 3,1 ,3 ,3 ,2 ,3 ,0,3, 0, 2, 1, 0, 2,1, 0, 2, 3, 3, 2,3, 2, 1, 0, 3, 3,100,1, 2, 3, 3, 2, 1, 1, 0 ,1 ,3, 1, 0 ,3, 0, 2, 1, 1, 3, 3, 1, 1, 2, 0 ,3 ,2 ,2, 0, 2, 0, 2, 0, 0, 2, 1, 2, 1, 2, 2, 2, 1, 1, 0, 1, 3, 0, 2, 0, 0, 2, 0,3, 0, 0, 3, 2, 3, 1, 2, 2, 2, 1, 1, 1, 3, 3, 2, 0, 2, 1, 2, 1, 2, 1, 1, 0, 0, 2, 1, 1, 1, 1, 0, 2, 2, 1, 0, 3, 3, 3, 2, 3, 0, 2, 2, 1, 0, 2, 2, 3, 3};
    int row=array[0],col=array[1];
    for(int i=0;i<row;i++) for(int j=0;j<col;j++) board[i][j]=array[2+i*row+j];
    for(int i=0;i<array[38];i++) stacks[i]=array[39+i];
    return array[38];
}

/**
 * Various functions will call this.
 * printGameBoard() will print the current game board in the specified format
 * @param board The game board the game is using
 */
void printGameBoard(int board[][W]) {
    printf("=====\n  | 0 | 1 | 2 | 3 | 4 | 5 |\n");
    for(int i=0;i<H;i++){
        printf(" %d|", i);
        for(int j=0;j<W;j++){
            if(board[i][j]==32) printf(" %c |", 32);
            else printf(" %c |", candies[board[i][j]]);
        }
        printf("\n");
    }
}

/**
 * Main() will call this function.
 * askForSwap() will perform the major game logic by (1) asking the users which candy to swap
 * (2)Validate whether the swapping can be done (3) Perform the swapping
 * @param board The game board the game is using.
 * @return 0 if the error is encountered (e.g. Coordinates out of bound);
 * 1 if swapping is successfully
 */
int askForSwap(int board[][W]) {
    int row,column,put,count=0,place=0,test=0;
    char direction;
   printf("Enter the coordinate (row, column) of the candy:");
     scanf("%d%d", &row, &column);
     if(row<0||row>=H||column<0||column>W){
         printf("Coordinates Out of Bound.\n");
         return 0;
     }else if(board[row][column]==32){
         printf("Empty Cell Selected.\n");
         return 0;
     }else{
         printf("Enter the direction to swap (U for Up, D for Down, L for Left, R for Right):");
         scanf(" %c", &direction);
         if(!(direction=='U'||direction=='D'||direction=='L'||direction=='R')){
             printf("Wrong Direction Input.\n");
             return 0;
         }
         if((direction=='U'&&row-1<0)||(direction=='D'&&row+1>=H)||(direction=='L'&&column-1<0)||(direction=='R'&&column+1>=W)){
             printf("Move Out of Bound.\n");
             return 0;
         }
         if((direction=='U'&&board[row-1][column]==32)||(direction=='D'&&board[row+1][column]==32)||(direction=='L'&&board[row][column-1]==32)||(direction=='R'&&board[row][column+1]==32)){
             printf("Empty Cell Selected.\n");
             return 0;
         }
     }
     if(direction=='U') {
         swap(board,row,column,row-1,column);
         if(isMatching(board,row,column)==1&&isMatching(board,row-1,column)==1){
             findAndRemoveMatch(board,row-1,column);
             //printf("testing1");
             findAndRemoveMatch(board,row,column);
             //printf("testing2");
             printGameBoard(board);
         }else if(isMatching(board,row,column)==1&&isMatching(board,row-1,column)==0){
             findAndRemoveMatch(board,row,column);
             printGameBoard(board);
         }else if(isMatching(board,row,column)==0&&isMatching(board,row-1,column)==1){
             findAndRemoveMatch(board,row-1,column);
             printGameBoard(board);
         }else findAndRemoveMatch(board,row,column);

     }
     else if(direction=='D'){
        swap(board,row,column,row+1,column);
        if(isMatching(board,row,column)==1&&isMatching(board,row+1,column)==1){
             findAndRemoveMatch(board,row+1,column);
             //printf("testing1");
             findAndRemoveMatch(board,row,column);
             //printf("testing2");
             printGameBoard(board);
         }else if(isMatching(board,row,column)==1&&isMatching(board,row+1,column)==0){
             findAndRemoveMatch(board,row,column);
             printGameBoard(board);
         }else if(isMatching(board,row,column)==0&&isMatching(board,row+1,column)==1){
             findAndRemoveMatch(board,row+1,column);
             printGameBoard(board);
         }else findAndRemoveMatch(board,row,column);
     }
     else if(direction=='L'){
       swap(board,row,column,row,column-1);
       if(isMatching(board,row,column)==1&&isMatching(board,row,column-1)==1){
             findAndRemoveMatch(board,row,column-1);
             //printf("testing1");
             findAndRemoveMatch(board,row,column);
             //printf("testing2");
             printGameBoard(board);
         }else if(isMatching(board,row,column)==1&&isMatching(board,row,column-1)==0){
             findAndRemoveMatch(board,row,column);
             printGameBoard(board);
         }else if(isMatching(board,row,column)==0&&isMatching(board,row,column-1)==1){
             findAndRemoveMatch(board,row,column-1);
             printGameBoard(board);
         }else findAndRemoveMatch(board,row,column);
     }
     else{
       swap(board,row,column,row,column+1);
       if(isMatching(board,row,column)==1&&isMatching(board,row,column+1)==1){
             findAndRemoveMatch(board,row,column+1);
             //printf("testing1");
             findAndRemoveMatch(board,row,column);
             //printf("testing2");
             printGameBoard(board);
         }else if(isMatching(board,row,column)==1&&isMatching(board,row,column+1)==0){
             findAndRemoveMatch(board,row,column);
             printGameBoard(board);
         }else if(isMatching(board,row,column)==0&&isMatching(board,row,column+1)==1){
             findAndRemoveMatch(board,row,column+1);
             printGameBoard(board);
         }else findAndRemoveMatch(board,row,column);
     }
     return 1;
}
/**
 * Various functions will call this
 * swap() will perform value swapping of two cells,
 * with the given source and destination coordinates respectively.
 * @param board the game board the game is using
 * @param row1 The row number of the source cell
 * @param col1 The column number of the source cell
 * @param row2 The row number of the destination cell
 * @param col2 The column number of the destination cell
 */
void swap(int board[][6], int row1, int col1, int row2, int col2) {
    char num;
    num=board[row1][col1];
    board[row1][col1]=board[row2][col2];
    board[row2][col2]=num;

}

/**
 * Various functions will call this.
 * findAndRemoveMatch() will find possible matches at the given coordinates (row,col)
 * for all four directions (Up, Down, Left, Right).
 * If found, turn the cell to ASCII 32 (' ').
 *
 * @param board The game board the game is using
 * @param row The row number of the given coordinate
 * @param col The column number of the given coordinate
 * @return 1 if there is a match in the board, 0 if there is not.
 */
int findAndRemoveMatch(int board[][W], int row, int col) {
    int count=0,consecutive=0,start=0,replace=0;
    int place=board[row][col];
    char target=candies[board[row][col]];
    for(int i=0;i<W;i++){
        if(board[row][i]==32){
          count=0;
          start=0;
        }
        else if(candies[board[row][i]]==target){
          count++;
          if(start==0&&count==1) start=i;
        }
        else {
            count=0;
            start=0;
        }

        if(count==3){
          if(candies[board[row][start+3]]==target&&(start+3<W)){
                if(candies[board[row][start+4]]==target&&(start+4<W)) count+=2;
                else count++;
          }
          consecutive=1;
          break;
        }
        //printf("testing %d,%d,%d", start, count, i);
    }
       if(consecutive==1){
       printf("Horizontal Match found at row %d!\n", row);
       for(int i=0;i<count;i++) {
            board[row][start+i]=32;
            if(start+i==col){
              board[row][col]=place;
              replace=1;
            }
       }
    }
    count=0;
    start=0;
    for(int i=0;i<H;i++){
        if(board[i][col]==32) count=0;
        else if(candies[board[i][col]]==target){
            count++;
            if(start==0&&count==1) start=i;
        }
        else{
            count=0;
            start=0;
        }
        if(count==3){
            if(candies[board[start+3][col]]==target&&start+3<H){
              if(candies[board[start+4][col]]==target&&start+4<H) count+=2;
              else count++;
            }
            consecutive=2;
            break;
        }
    }
    if(consecutive==2){
      printf("Vertical Match found at column %d!\n", col);
      for(int i=0;i<count;i++) if(start+i<H)board[start+i][col]=32;
    }

    if(consecutive==0){
        printGameBoard(board);
        printf("No Match found!\n");
        return 0;
    }
    if(consecutive==1&&replace==1) board[row][col]=32;
    return 1;
}

/**
 * Main() function will call this.
 * isGameOver() is to check whether there exists a game over situation, i.e.
 * no more match can be made under the current board scenerio.
 *
 * @param board The game board the game is using.
 * @return 1 if it is game over; 0 if it isn't .
 */
int isGameOver(int board[][W]) {
    int test[H][W];
    for(int i=0;i<H;i++) for(int j=0;j<W;j++) test[i][j]=board[i][j];
    for(int i=0;i<H;i++){
      for(int j=0;j<W;j++){
        if(j-1>=0){
            swap(test,i,j,i,j-1);
            if(isMatching(test,i,j)&&board[i][j]!=32) return 1;
            //printf("test1\n");
            if(isMatching(test,i,j-1)&&board[i][j-1]!=32) return 1;
            //printf("test2\n");
            swap(test,i,j,i,j-1);
        }
        if(j+1<W){
           swap(test,i,j,i,j+1);
           // printf("test1\n");
            if(isMatching(test,i,j)&&board[i][j]!=32) return 1;
            //printf("test3\n");
            if(isMatching(test,i,j+1)&&board[i][j]!=32) return 1;
            //printf("test4\n");
            swap(test,i,j,i,j+1);
        }
        if(i-1>=0){
            swap(test,i,j,i-1,j);
            if(isMatching(test,i,j)&&board[i][j]!=32) return 1;
            //printf("test5\n");
            if(isMatching(test,i-1,j)&&board[i][j]!=32) return 1;
            //printf("test6\n");
            swap(test,i,j,i-1,j);
        }
        if(i+1<H){
            swap(test,i,j,i+1,j);
            if(isMatching(test,i,j)&&board[i][j]!=32) return 1;
            //printf("test7\n");
            if(isMatching(test,i+1,j)&&board[i][j]!=32) return 1;
            //printf("test8\n");
            swap(test,i,j,i+1,j);
        }
      }
  }
  return 0;
}

/**
 * isGameOver() will call this function.
 * isMatching will check if there is at least 1 match-three at the given coordinate (row,col)
 * for four directions (Top,Down,Left,Right).
 * @param board The game board the game is using.
 * @param row The row number of the target cell
 * @param col The column number of the target cell
 * @return 1 if there is at least 1 match in any direction; 0 if there is not.
 */
int isMatching(int board[][W], int row, int col) {
    int count=0,consecutive=0,start=0,replace=0;
    char target=candies[board[row][col]];
    for(int i=0;i<W;i++){
        if(board[row][i]==32) count=0;
        else if(candies[board[row][i]]==target){
          count++;
         // printf("testing %d,%d", i, count);
          if(start==0&&count==1) start=i;
        }
        else {
            count=0;
            start=0;
        }

        if(count==3){
          if(candies[board[row][start+3]]==target&&(start+3<W)){
                if(candies[board[row][start+4]]==target&&(start+4<W)) count+=2;
                else count++;
          }
          consecutive=1;
          break;
        }
        //printf("testing %d,%d,%d", start, count, i);
    }
    if(consecutive==1) return 1;
    count=0;
    consecutive=0;
    start=0;
    for(int i=0;i<H;i++){
        if(board[i][col]==32) count=0;
        else if(candies[board[i][col]]==target){
            count++;
            if(start==0&&count==1) start=i;
        }
        else{
            count=0;
            start=0;
        }
        if(count==3){
            if(candies[board[start+3][col]]==target&&start+3<H){
              if(candies[board[start+4][col]]==target&&start+4<H) count+=2;
              else count++;
            }
            consecutive=2;
            break;
        }
    }
    if(consecutive==2) return 1;
    return 0;

}

/**
 * [Part II]
 * askForSwap() and cascade() will call this function
 * applyGravity() will scan the all columns from left to right, and
 * if there are empty cells, it will move down the candy. Lastly, print the
 * gameboard.
 * @param board The game board the game is using
 */
void applyGravity(int board[][6]) {
    int replace;
  for(int i=0;i<H;i++){
      for(int j=0;j<W;j++){
          if(board[i][j]==32){
              for(int n=0;n<i;n++){
                 board[i-n][j]=board[i-n-1][j] ;
              }
              board[0][j]=32;
          }
      }
  }
}

/**
 * [Part II]
 * askForSwap() and cascade() will call this.
 * fillEmpty() will scan the columns from left to right. If there are empty cells,
 * it will fill them with candies got from the stacks, in bottom-to-top manner. Lastly,
 * print the gameboard.
 * @param board the game board the game is using
 * @param stacks the 1D array containing the candies
 * @param current the counter of the next candy location in stacks array
 * @param numCandies the total number of candies in the stacks
 * @return the updated current value, i.e. the updated counter pointing to the next
 * available candy
 */
int fillEmpty(int board[][W], int stacks[], int current, int numCandies) {
   for(int j=0;j<W;j++){
       for(int i=H-1;i>=0;i--){
           if(board[i][j]==32){
               if(current>numCandies){
                   printf("No more candies available.\n");
                   exit(-1);
               }else{
                   board[i][j]=stacks[current];
                   current++;
               }
           }
       }
   }
   return current;
}

/**
 * [Part II]
 * main() function will call this
 * cascade() will continously check the board to see if there is any matches
 * and perform removal until no further matches can be found.
 * First, you can apply the gravity and fill in the empty cells.
 * Then, for each cells on the board and if it is not empty, try to call
 * findAndRemoveMatch() to remove matches if any. If there is a match,
 * print the specified message.
 * Remember to collect the return value of current in fillEmpty() such that
 * you return to the main function to update the variable.
 * @param board the game board the game is using
 * @param stacks the 1D array containing the candies
 * @param current the counter of the next candy location in stacks array
 * @param numCandies the total number of candies in the stacks
* @return the updated current value, i.e. the updated counter pointing to the next
 * available candy
 */
int cascade(int board[][6], int stacks[], int current, int numCandies) {
    int check=1;
    while(check){
        check=0;
        applyGravity(board);
        printGameBoard(board);
        current=fillEmpty(board,stacks,current,numCandies);
        printGameBoard(board);
        for(int i=0;i<H;i++){
          for(int j=0;j<W;j++){
           if(isMatching(board,i,j)){
               findAndRemoveMatch(board,i,j);
               check=1;
               //printf("(%d)", current);
               break;
               break;
           }
          }
        }
        if(check==1) printf("Cascade Matches found!\n");
    }
    return current;

}
/* Main Function */
/**
 * The main function will hold the game loop and hold the game logic.
 * main() will call various functions to accompolish various tasks such as
 * initializing the game board, asking for Input, and etc.
 * It will hold an infinite loop to repeatly hold the game. If it is a game over status,
 * i.e. isGameOver() returns one (1), then it will break out of the loop and the program
 * finishes.
 *
 * @return 0
 */
int main(void) {
 // This is the variable holding a sample board value
 int board_sample[6][6] = {
  {1,3,2,0,1,0},
  {2,2,0,1,1,3},
  {1,3,3,2,3,0},
  {3,0,2,1,0,2},
  {1,0,2,3,3,2},
  {3,2,1,0,3,3}
 };

 int board[H][W] = {0}; // The game board, initialized to 0
 int stacks[100] = {0}; // Stacks array containing candy, Maximum capacity 100
 int numCandies = 0; // Number of candies in the stacks
 int current = 0;// the counter pointing to the next available candy
// You may declare more variables if needed.
int row,column;
 initGameBoard(board,board_sample);
 numCandies=initGameBoardFromFile(board,stacks);
 while(1){
     printf("=====\nNew Round:\n");
     printGameBoard(board);
     if(askForSwap(board)==0) printf("Please try again.\n");
     current=cascade(board,stacks,current,numCandies);
     if(!isGameOver(board)) break;

 }
 printf("Game Over! No more possible moves.\n");
 // 1. First, you may initialize the board first
 // For part 1, you may use initGameBoard() while for part 2, initGameBoardFromFile()

 // Then, you may hold a loop to keep the game running


  // printf for your reference
  //printf("=====\n");
  //printf("New Round:\n");

 // Print the board using printGameBoard()

  // Call askForSwap() to ask which candy to swap and perform swapping if successful.
  // If not, print error message, shown for your reference

  // printf("Please try again.\n");

  // Otherwise, continue the game logic by applying gravity and filling empty cells
  // For part II, you may replace the above two to cascade(), as cascade() will be
  // responsible to call at that moment.

 // Check if it is a game over status. If yes, print the message and break the loop
 // printf is left for your reference.

  // printf("Game Over! No more possible moves.\n");

// Hope you enjoy the game : )

 return 0;
}
