/******************
Name: Ortal Bauer
ID: 214896102
Assignment: ex3
*******************/

#include <stdio.h>

#ifndef ROWS
#define ROWS 6
#endif

#ifndef COLS
#define COLS 7
#endif

#define CONNECT_N 4

/* Tokens */
#define EMPTY '.'
#define TOKEN_P1 'X'
#define TOKEN_P2 'O'

#define HUMAN 1
#define COMPUTER 2

int isColumnFull(char[][COLS], int);

int isBoardFull(char[][COLS], int);

/* Return index of row where token will land, or -1 if column full */
int getFreeRow(char[][COLS], int, int);

/* Place token in column (0-based). Return row index or -1 if illegal */
int makeMove(char[][COLS], int, int, char);

int checkVictory(char[][COLS], int, int, char);

int checkThree(char[][COLS], int, int, int, int, char);

void sortColsByDistance(int, int[]);

/* Human player: asks repeatedly until a valid non-full column is chosen (0-based) */
int humanChoose(char[][COLS], int);

/* Computer*/
int computerChoose(char[][COLS], int, int, char, char);

void runConnectFour(char[][COLS], int, int, int, int);

void initBoard(char[][COLS], int, int);

void printBoard(char[][COLS], int, int);

int getPlayerType(int);

int main() {
    char board[ROWS][COLS];
    printf("Connect Four (%d rows x %d cols)\n\n", ROWS, COLS);
    int p1Type = getPlayerType(1);
    int p2Type = getPlayerType(2);
    initBoard(board, ROWS, COLS);
    printBoard(board, ROWS, COLS);
    runConnectFour(board, ROWS, COLS, p1Type, p2Type);
    return 0;
}

int isColumnFull(char board[][COLS], int col){

    //check if the last row in the column empty
    if(board[0][col] == EMPTY){
        return 0;
    } else{
        return 1;
    }
}

int isBoardFull(char board[][COLS], int cols){

    //loop that goes through all the columns
    for(int i = 0; i < cols; i++){
        //check for every column if is it full
        if(isColumnFull(board, i) == 0){
            //return 0 if the board is not foll
            return 0;
        }
    }
    //return 1 if the board is full
    return 1;
}

int getFreeRow(char board[][COLS], int rows, int col){

    //go on every row from the bottom to the top
    for(int i = rows-1; i >= 0; i--){
        //for every row check if it is empty
        if(board[i][col] == EMPTY){
            //return i if it is empty
            return i;
        }
    }
    //return -1 if it is full
    return -1;
}

int makeMove(char board[][COLS], int rows, int col, char token){

    //get the row that empty in col
    int row = getFreeRow(board, rows, col);
    
    //check if the roe is full
    if(row == -1){
        return -1;
    } else{
        //if the row is not full put the token in the column and return the row
        board[row][col] = token;
        return row;
    }
}

int checkVictory(char board[][COLS], int rows, int cols, char token){

    //check viktory in rows
    for(int i = 0; i < rows; i++){
        for(int j = 0; j <= cols - CONNECT_N; j++){
            if(board[i][j] == token && board[i][j+1] == token && board[i][j+2] == token && board[i][j+(CONNECT_N -1)] == token){
                return 1;
            }
        }
    }

    //check viktory in columns
    for(int i = 0; i <= rows - CONNECT_N; i++){
        for(int j = 0; j < cols; j++){
            if(board[i][j] == token && board[i+1][j] == token && board[i+2][j] == token && board[i+(CONNECT_N-1)][j] == token){
                return 1;
            }
        }
    }

    //check viktory in slant down right
    for(int i = 0; i <= rows - CONNECT_N; i++){
        for(int j = 0; j <= cols - CONNECT_N; j++){
            if(board[i][j] == token && board[i+1][j+1] == token && board[i+2][j+2] == token && board[i+(CONNECT_N-1)][j+(CONNECT_N-1)] == token){
                return 1;
            }
        }
    }

    //check viktory in slant up right
    for(int i = 0; i <= rows - CONNECT_N; i++){
        for(int j = CONNECT_N-1; j < cols; j++){
            if(board[i][j] == token && board[i+1][j-1] == token && board[i+2][j-2] == token && board[i+(CONNECT_N-1)][j-(CONNECT_N-1)] == token){
                return 1;
            }
        }
    }
    
    //return 0 if there is no victory
    return 0;
}

int checkThree(char board[][COLS], int rows, int cols, int row, int col, char token){

    //check in rows
    for (int i = col - 2; i <= col; i++) {
        if (i >= 0 && i + 2 < cols) {
            if (board[row][i] == token && board[row][i+1] == token && board[row][i+2] == token)
                return 1;
        }
    }

    //check in columns
    for (int i = row - 2; i <= row; i++) {
        if (i >= 0 && i + 2 < rows) {
            if (board[i][col] == token && board[i+1][col] == token && board[i+2][col] == token)
                return 1;
        }
    }

    //check in slant down right
    for (int i = -2; i <= 0; i++) {
        if (row+i >= 0 && row+i+2 < rows && col+i >= 0 && col+i+2 < cols) {
            if (board[row+i][col+i] == token && 
                board[row+i+1][col+i+1] == token && 
                board[row+i+2][col+i+2] == token)
                return 1;
        }
    }

    //check in slant up right
    for (int i = -2; i <= 0; i++) {
        if (row-i-2 >= 0 && row-i < rows && col+i >= 0 && col+i+2 < cols) {
            if (board[row-i][col+i] == token && 
                board[row-i-1][col+i+1] == token && 
                board[row-i-2][col+i+2] == token)
                return 1;
        }
    }
    
    //return 0 if there is no three
    return 0;
}

//sort the column, who is distance from the center column is minimal, and if the distance is equal the left one
void sortColsByDistance(int cols, int searchOrder[]){
    int center = (cols - 1) / 2;
    int current = 0;

    searchOrder[current++] = center;

    for(int i = 1; current < cols; i++){
        if(cols%2 != 0){
            if (center - i >= 0){
                searchOrder[current++] = center - i;
            }
            if (center + i < cols){
                searchOrder[current++] = center + i;
            }
        }else {
            if(center + i < cols){
                searchOrder[current++] = center + i;
            }
            if(center - i >= 0){
                searchOrder[current++] = center - i;
            }
        }
    }
}

int humanChoose(char board[][COLS], int cols){

    int choice;

    while(1){
        printf("Enter column (1-%d): ", cols);

        //check if the input is a number
        if(scanf("%d", &choice) != 1){
            printf("Invalid input. Enter a number.\n");

            while(getchar() != '\n');
            continue;
        }

        //check if the number is vaild
        if(choice < 1 || choice > cols){
            printf("Invalid column. Choose between 1 and %d.\n", cols);
            continue;
        }

        //check if the column is full
        if(isColumnFull(board, choice - 1)){
            printf("Column %d is full. Choose another column.\n", choice);
            continue;
        }

        return choice -1;
    }
}

int computerChoose(char board[][COLS], int rows, int cols, char myToken, char enemyToken){

    int searchOrder[COLS];

    sortColsByDistance(cols, searchOrder);

    //check if he can win and do it if he can
    for(int i = 0; i <= COLS - 1; i++){
        int k = searchOrder[i];
        if(isColumnFull(board, k) != 1){
            int row = getFreeRow(board, rows, k);
            board[row][k] = myToken;
            if(checkVictory(board, rows, cols, myToken)==1){
                board[row][k] = EMPTY;
                return k;
            }else{
                board[row][k] = EMPTY;
            }
        }
    }

    //check if he can block and do it if he can
    for(int i = 0; i <= COLS - 1; i++){
        int k = searchOrder[i];
        if(isColumnFull(board, k) != 1){
            int row = getFreeRow(board, rows, k);
            board[row][k] = enemyToken;
            if(checkVictory(board, rows, cols, enemyToken)==1){
                board[row][k] = EMPTY;
                return k;
            }else{
                board[row][k] = EMPTY;
            }
        }
    }

    //check if he can put 3 and do it if he can
    for(int i = 0; i <= COLS - 1; i++){
        int k = searchOrder[i];
        if(isColumnFull(board, k) != 1){
            int row = getFreeRow(board, rows, k);
            board[row][k] = myToken;
            if(checkThree(board, rows, cols, row, k, myToken)==1){
                board[row][k] = EMPTY;
                return k;
            }else{
                board[row][k] = EMPTY;
            }
        }
    }

    //chak if he can block 2 and do it if he can
    for(int i = 0; i <= COLS - 1; i++){
        int k = searchOrder[i];
        if(isColumnFull(board, k) != 1){
            int row = getFreeRow(board, rows, k);
            board[row][k] = enemyToken;
            if(checkThree(board, rows, cols, row, k, enemyToken)==1){
                board[row][k] = EMPTY;
                return k;
            }else{
                board[row][k] = EMPTY;
            }
        }
    }

    //put in the first column that empty
    for(int i = 0; i < COLS; i++){
        int k = searchOrder[i];
        if(isColumnFull(board, k) == 0){
            return k;
        }
    }
    return 0;
}

void runConnectFour(char board[][COLS], int rows, int cols, int p1Type, int p2Type){
    int currentPlayer = 1;
    int currentType;
    int currentToken;
    int colChoice;

    while(1){

        //check who is turn now
        if(currentPlayer == 1){
            currentType = p1Type;
            currentToken = TOKEN_P1;
        }else {
            currentType = p2Type;
            currentToken = TOKEN_P2;
        }

        //print who is turn now
        printf("Player %d (%c) turn.\n", currentPlayer, currentToken);

        //if the player is human call to humanChoose() if is computer call to computerChoose()
        if(currentType == HUMAN){
            colChoice = humanChoose(board, cols);
        }else {
            char enemyToken = (currentPlayer == 1) ? TOKEN_P2 : TOKEN_P1;
            colChoice = computerChoose(board, rows, cols, currentToken, enemyToken);
            printf("Computer chose column %d\n", colChoice + 1);
        }

        //make the move
        makeMove(board, rows, colChoice, currentToken);

        //print the new board
        printBoard(board, rows, cols);

        //check if there is win
        if(checkVictory(board, rows, cols, currentToken)){
            printf("Player %d (%c) wins!\n", currentPlayer, currentToken);
            return;
        }

        //check teko
        if (isBoardFull(board, cols)){
            printf("Board full and no winner. It's a tie!\n");
            return;
        }

        //change player
        currentPlayer = (currentPlayer == 1) ? 2 : 1;
    }
}

void initBoard(char board[][COLS], int rows, int cols){

    //Loop that goes through all the rows
    for(int i = 0; i < rows; i++){
        //loop that goes through all the columns
        for(int j = 0; j < cols; j++){
            //put empty in the board in rows i in column j
            board[i][j] = EMPTY;
        }
    }
}

void printBoard(char board[][COLS], int rows, int cols) {
    printf("\n");
    for (int r = 0; r < rows; r++) {
        printf("|");
        for (int c = 0; c < cols; c++) {
            putchar(board[r][c]);
            printf("|");
        }
        printf("\n");
    }
    for (int c = 1; c <= cols; c++) {
        printf(" %d", c % 10);
    }
    printf("\n\n");
}

int getPlayerType(int playerNumber) {
    char ch;
    while (1) {
        printf("Choose type for player %d: h - human, c - computer: ", playerNumber);
        int n = scanf(" %c", &ch);
        if (n != 1) {
            printf("Input error. Try again.\n");
            while (getchar() != '\n'); // clear input buffer
            continue;
        }
        if (ch == 'h' || ch == 'H') return HUMAN;
        if (ch == 'c' || ch == 'C') return COMPUTER;

        printf("Invalid selection. Enter h or c.\n");
        while (getchar() != '\n'); // clear rest of input
    }
}
