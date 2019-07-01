#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <dos.h>
#include <dir.h>
#include <conio.h>

int grid[3][3];
char humanMark = 'X', computerMark = 'O';

// Get point values of position
int getValByPos(int pos);

// Get who won
// 0 - none 1 - computer 2 - player
int getWinner();

// Reset game, Set all values of grid to zero
void resetGame();

// Play the game between player and computer
void playGame();

// Check is the game won by someone or is it draw.
int checkWon();

// Show the board which contains 3x3 cells
void showBoard();

// Show next move of computer
void showNextMove();

// Clear screen and screen will become empty
void clearScreen();

// Ask question which has two option.If user select first choice return 0 else return 1
int askQuestion(char question[], char selection1, char selection2);

// Input sign which used by player
void inputHumanMark();

// Place computer's mark in given position
void placeMark(int position);

// Get point of a row
int getRowVal(int p1, int p2, int p3);

// Put point of a row into given array with cells of given row
void getRowValInToArr(int p1, int p2, int p3, int vals[4]);

// return is given row blocked
int isBlocked(int v1, int v2, int v3);

// Return count of given mark in given row
int getCountOfMarkInRow(int mark, int v1, int v2, int v3);

// Set cursor to given position
void setPos(int posX, int posY);

// Set given foreground and background color of given position
void setColor(int posX, int posY, int fgColor, int bgColor);

// Wait until user press enter
void pressEnterToContinue();

int main()
{
    int isComputerPlayFirst;
    do{
        resetGame();

        inputHumanMark();

        isComputerPlayFirst = askQuestion("Do you want to play first ?", 'Y', 'N');

        showBoard(grid);

        playGame(isComputerPlayFirst);

        pressEnterToContinue();

        clearScreen();
    }while(askQuestion("Do you want to play again ?", 'Y', 'N')==0);
    return 0;
}

void pressEnterToContinue(){
    int pKey;
    do{
        pKey = _getch();
    }while(pKey!=13);
}

int getValByPos(int pos){
    return grid[pos/3][pos%3];
}

int getWinner(){
    int rows[8][3] = {
        { 0, 1, 2},
        { 3, 4, 5},
        { 6, 7, 8},
        { 0, 3, 6},
        { 1, 4, 7},
        { 2, 5, 8},
        { 0, 4, 8},
        { 2, 4, 6}
    };

    int i;
    for(i=0; i<8; i++){
        if(getValByPos(rows[i][0])==getValByPos(rows[i][1]) && getValByPos(rows[i][1])==getValByPos(rows[i][2]) && getValByPos(rows[i][0])==getValByPos(rows[i][2]))
            return getValByPos(rows[i][0]);
    }


    for(i=0; i<8; i++)
        if(getRowVal(rows[i][0], rows[i][1], rows[i][2])!=0)
            return 0;

    return 3;
}

void resetGame(){
    int i, j;
    for(i=0; i<3; i++)
        for(j=0; j<3; j++)
            grid[i][j] = 0;
}

void playGame(int isComputerPlayFirst){
    int pKey, colI = 0, rowI = 0;

    if(isComputerPlayFirst){
        int randPos = rand() % 9;
        placeMark(randPos);
    }

    while (1){
        setPos(6*colI+3, 4*rowI+2);

        pKey = _getch();

        if(pKey=='x') pKey='X';
        if(pKey=='o') pKey='O';

        switch(pKey){
            case 72: // UP
                rowI -= (rowI!=0);
                /*
                if(rowI!=0)
                    rowI -= 1;
                */
                break;
            case 80: // DOWN
                rowI += (rowI!=2);
                break;
            case 77: // RIGHT
                colI += (colI!=2);
                break;
            case 75: // LEFT
                colI -= (colI!=0);
                break;
        }

        // 1 = computer mark
        // 2 = human mark
        if(grid[rowI][colI]==0 && (pKey==humanMark)){
            grid[rowI][colI] = 2;
            setPos(3+6*colI, 2+4*rowI);
            printf("%c", humanMark);
            if(checkWon()) return;
            showNextMove();
            if(checkWon()) return;
        }
    }
}

int checkWon(){
    int winner = getWinner();
    if(winner){
        setPos(3, 14);
        if(winner==1)
            printf("Computer won\n");
        else if(winner==2)
            printf("Congratulations. You won\n");
        else if (winner==3)
            printf("No possibilities to win anyone.\n");
        return 1;
    }
    return 0;
}

void setColor(int posX, int posY,int ForgC, int BackC){
    WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = {posX, posY};
    DWORD count;

    CONSOLE_SCREEN_BUFFER_INFO csbi;

    SetConsoleTextAttribute(hStdOut, wColor);
    if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        FillConsoleOutputCharacter(hStdOut, (TCHAR) 32, 1, coord, &count);
        FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, 1, coord, &count );
        SetConsoleCursorPosition(hStdOut, coord);
    }
    return;
}



void showBoard(){
    setColor(0, 0, 9, 0);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 201, 205, 205, 205, 205, 205, 203, 205, 205, 205, 205, 205, 203, 205, 205, 205, 205, 205, 187);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 186, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 32, 186);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 186, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 32, 186);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 186, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 32, 186);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 204, 205, 205, 205, 205, 205, 206, 205, 205, 205, 205, 205, 206, 205, 205, 205, 205, 205, 185);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 186, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 32, 186);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 186, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 32, 186);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 186, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 32, 186);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 204, 205, 205, 205, 205, 205, 206, 205, 205, 205, 205, 205, 206, 205, 205, 205, 205, 205, 185);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 186, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 32, 186);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 186, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 32, 186);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 186, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 32, 186);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 200, 205, 205, 205, 205, 205, 202, 205, 205, 205, 205, 205, 202, 205, 205, 205, 205, 205, 188);
    setColor(1, 1, 15, 0);
}

void showNextMove(){
    int i, rowVals[8][4], positionVals[9];

    getRowValInToArr(0, 1, 2, rowVals[0]);
    getRowValInToArr(3, 4, 5, rowVals[1]);
    getRowValInToArr(6, 7, 8, rowVals[2]);
    getRowValInToArr(0, 3, 6, rowVals[3]);
    getRowValInToArr(1, 4, 7, rowVals[4]);
    getRowValInToArr(2, 5, 8, rowVals[5]);
    getRowValInToArr(0, 4, 8, rowVals[6]);
    getRowValInToArr(2, 4, 6, rowVals[7]);

    for(i=0; i<9; i++)
        positionVals[i] = 0;

    for(i=0; i<8; i++){
        positionVals[rowVals[i][0]] += rowVals[i][3];
        positionVals[rowVals[i][1]] += rowVals[i][3];
        positionVals[rowVals[i][2]] += rowVals[i][3];
    }

    int maxVal=-1, maxValPos=-1;

    for(i=0; i<9; i++){
        if(grid[i/3][i%3]==0){
            maxVal = positionVals[i];
            maxVal = i;
            break;
        }
    }

    for(i=0; i<9; i++){
        if(positionVals[i]>maxVal && grid[i/3][i%3]==0){
            maxVal = positionVals[i];
            maxValPos = i;
        }
    }

    placeMark(maxValPos);
}

void clearScreen(){
    setPos(0, 0);
    int i, j;
    for(i=0; i<30; i++){
        for(j=0; j<40; j++)
            printf(" ");
        printf("\n");
    }
}

int askQuestion(char question[], char selection1, char selection2){
    setPos(0, 0);
    printf("%s", question);
    printf("\n\n        %c    %c", selection1, selection2);

    int selection = 0;
    while(1){
        if(selection==1)
            setPos(13, 2);
        else
            setPos(8, 2);
        int pKey = _getch();
        if(pKey==75 || pKey==77)
            selection = !selection;
        if(pKey==13)
            break;
    }

    setPos(0, 0);
    printf("                                                          \n");
    printf("                                                          \n");
    printf("                                                          \n");

    return selection;
}

void inputHumanMark(){
    int selection = askQuestion("Select your mark", 'X', 'O');
    if(selection==1){
        humanMark = 'O';
        computerMark = 'X';
    }else{
        humanMark = 'X';
        computerMark = 'O';
    }
}

void placeMark(int position){
    int rowI = position/3, colI=position%3;
    grid[rowI][colI] = 1;

    setPos(3+6*colI, 2+4*rowI);
    printf("%c", computerMark);
}

int getRowVal(int p1, int p2, int p3){
    int r1, c1, r2, c2, r3, c3, val = 0;

    r1 = p1/3;
    r2 = p2/3;
    r3 = p3/3;

    c1 = p1%3;
    c2 = p2%3;
    c3 = p3%3;

    int v1, v2, v3;
    v1 = grid[r1][c1];
    v2 = grid[r2][c2];
    v3 = grid[r3][c3];

    if(isBlocked(v1, v2, v3))
        return val;

    val = (getCountOfMarkInRow(2, v1, v2, v3)==1)*9 + (getCountOfMarkInRow(2, v1, v2, v3)==2)*100 + (getCountOfMarkInRow(1, v1, v2, v3)==1)*10 + (getCountOfMarkInRow(1, v1, v2, v3)==2)*1000;
    /*
    if(getCountOfMarkInRow(2, v1, v2, v3)==1)
        val = 9;
    if(getCountOfMarkInRow(2, v1, v2, v3)==2)
        val = 100;
    if(getCountOfMarkInRow(1, v1, v2, v3)==1)
        val = 10;
    if(getCountOfMarkInRow(1, v1, v2, v3)==2)
        val = 1000;
    */
    return val;
}

void getRowValInToArr(int p1, int p2, int p3, int vals[4]){
    vals[0] = p1;
    vals[1] = p2;
    vals[2] = p3;
    vals[3] = getRowVal(p1, p2, p3);
}

int isBlocked(int v1, int v2, int v3){
    int isBlocked = (v1==1 || v2==1 || v3==1)&&(v1==2 || v2==2 || v3==2);
    return isBlocked;
}

int getCountOfMarkInRow(int mark, int v1, int v2, int v3){
    int count;
    count = (mark==v1) + (mark==v2) + (mark==v3);
    return count;
}

void setPos(int posX, int posY){
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = {posX, posY};

    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
        SetConsoleCursorPosition(hStdOut, coord);

    return;
}
