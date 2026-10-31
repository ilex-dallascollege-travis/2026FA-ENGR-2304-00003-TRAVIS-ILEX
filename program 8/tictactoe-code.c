/*
2026FA-ENGR-2304-00003-TRAVIS-ILEX-P07
Programming Assignment 07
Tic-Tac-Toe Using Artificial Intelligence
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define SIZE 9

char board[SIZE];
char humanMark = 'X';
char computerMark = 'O';

/* Pause briefly before the computer makes its move. */
void pauseBriefly(void)
{
#ifdef _WIN32
    Sleep(700);
#else
    usleep(700000);
#endif
}

/* Initialize the board with positions 1 through 9. */
void initializeBoard(void)
{
    int i;

    for (i = 0; i < SIZE; i++)
        board[i] = '1' + i;
}

/* Display the current board. */
void displayBoard(void)
{
    printf("\n");
    printf(" %c | %c | %c\n", board[0], board[1], board[2]);
    printf("---+---+---\n");
    printf(" %c | %c | %c\n", board[3], board[4], board[5]);
    printf("---+---+---\n");
    printf(" %c | %c | %c\n", board[6], board[7], board[8]);
    printf("\n");
}

/* Check whether a player has won. */
int checkWin(char mark)
{
    int wins[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
        {0, 4, 8}, {2, 4, 6}
    };

    int i;

    for (i = 0; i < 8; i++) {
        if (board[wins[i][0]] == mark &&
            board[wins[i][1]] == mark &&
            board[wins[i][2]] == mark)
            return 1;
    }

    return 0;
}

/* Return 1 if the board is full; otherwise return 0. */
int boardFull(void)
{
    int i;

    for (i = 0; i < SIZE; i++) {
        if (board[i] >= '1' && board[i] <= '9')
            return 0;
    }

    return 1;
}

/* Check whether a board position is available. */
int validMove(int position)
{
    return position >= 0 &&
           position < SIZE &&
           board[position] >= '1' &&
           board[position] <= '9';
}

/* Get and validate a player's move. */
void playerMove(char mark)
{
    int position;
    int result;

    while (1) {
        printf("Player (%c), choose a position (1-9): ", mark);
        result = scanf("%d", &position);

        if (result != 1) {
            int ch;

            printf("Invalid input. Enter a number from 1 to 9.\n");

            while ((ch = getchar()) != '\n' && ch != EOF) {
                /* Clear invalid input. */
            }

            continue;
        }

        if (position < 1 || position > 9 ||
            !validMove(position - 1)) {
            printf("That position is unavailable. Try again.\n");
            continue;
        }

        board[position - 1] = mark;
        break;
    }
}

/* Minimax AI: find the best possible outcome for the computer. */
int minimax(int isComputerTurn)
{
    int i;
    int bestScore;
    int score;
    char mark;

    if (checkWin(computerMark))
        return 10;

    if (checkWin(humanMark))
        return -10;

    if (boardFull())
        return 0;

    if (isComputerTurn) {
        bestScore = -1000;
        mark = computerMark;
    } else {
        bestScore = 1000;
        mark = humanMark;
    }

    for (i = 0; i < SIZE; i++) {
        if (validMove(i)) {
            char original = board[i];

            board[i] = mark;
            score = minimax(!isComputerTurn);
            board[i] = original;

            if (isComputerTurn) {
                if (score > bestScore)
                    bestScore = score;
            } else {
                if (score < bestScore)
                    bestScore = score;
            }
        }
    }

    return bestScore;
}

/* Use minimax to choose the computer's move. */
void computerMove(void)
{
    int i;
    int bestScore = -1000;
    int score;
    int bestPosition = -1;

    printf("Computer is thinking...\n");
    pauseBriefly();

    for (i = 0; i < SIZE; i++) {
        if (validMove(i)) {
            char original = board[i];

            board[i] = computerMark;
            score = minimax(0);
            board[i] = original;

            if (score > bestScore) {
                bestScore = score;
                bestPosition = i;
            }
        }
    }

    if (bestPosition != -1) {
        board[bestPosition] = computerMark;
        printf("Computer chooses position %d.\n",
               bestPosition + 1);
    }
}

/* Play one-player mode. */
void onePlayerGame(void)
{
    int humanFirst;

    printf("\nChoose your mark:\n");
    printf("1. X\n");
    printf("2. O\n");
    printf("Enter choice: ");

    while (scanf("%d", &humanFirst) != 1 ||
           (humanFirst != 1 && humanFirst != 2)) {
        int ch;

        printf("Enter 1 for X or 2 for O: ");

        while ((ch = getchar()) != '\n' && ch != EOF) {
            /* Clear invalid input. */
        }
    }

    humanMark = (humanFirst == 1) ? 'X' : 'O';
    computerMark = (humanMark == 'X') ? 'O' : 'X';

    /* Randomly decide whether the human or computer starts. */
    humanFirst = rand() % 2;

    if (humanFirst)
        printf("\nYou will go first!\n");
    else
        printf("\nThe computer will go first!\n");

    initializeBoard();
    displayBoard();

    while (1) {
        if (humanFirst) {
            playerMove(humanMark);
        } else {
            computerMove();
        }

        displayBoard();

        if (checkWin(humanFirst ? humanMark : computerMark)) {
            if (humanFirst)
                printf("Congratulations! You win!\n");
            else
                printf("The computer wins!\n");

            break;
        }

        if (boardFull()) {
            printf("The game is a tie!\n");
            break;
        }

        humanFirst = !humanFirst;
    }
}

/* Play two-player mode. */
void twoPlayerGame(void)
{
    int currentPlayer = rand() % 2;
    char marks[2] = {'X', 'O'};

    printf("\nPlayer %d will go first!\n", currentPlayer + 1);

    initializeBoard();
    displayBoard();

    while (1) {
        char mark = marks[currentPlayer];

        playerMove(mark);
        displayBoard();

        if (checkWin(mark)) {
            printf("Player %d (%c) wins!\n",
                   currentPlayer + 1, mark);
            break;
        }

        if (boardFull()) {
            printf("The game is a tie!\n");
            break;
        }

        currentPlayer = 1 - currentPlayer;
    }
}

int main(void)
{
    int mode;
    int playAgain = 1;

    srand((unsigned int)time(NULL));

    printf("=================================\n");
    printf(" TIC-TAC-TOE WITH AI\n");
    printf("=================================\n");

    while (playAgain) {
        printf("\nSelect game mode:\n");
        printf("1. One player vs. computer\n");
        printf("2. Two players\n");
        printf("Enter choice: ");

        while (scanf("%d", &mode) != 1 ||
               (mode != 1 && mode != 2)) {
            int ch;

            printf("Invalid choice. Enter 1 or 2: ");

            while ((ch = getchar()) != '\n' && ch != EOF) {
                /* Clear invalid input. */
            }
        }

        if (mode == 1)
            onePlayerGame();
        else
            twoPlayerGame();

        printf("\nWould you like to play again?\n");
        printf("1. Yes\n");
        printf("2. No\n");
        printf("Enter choice: ");

        while (scanf("%d", &playAgain) != 1 ||
               (playAgain != 1 && playAgain != 2)) {
            int ch;

            printf("Enter 1 to play again or 2 to quit: ");

            while ((ch = getchar()) != '\n' && ch != EOF) {
                /* Clear invalid input. */
            }
        }

        if (playAgain == 2)
            playAgain = 0;
    }

    printf("\nThank you for playing Tic-Tac-Toe!\n");

    return 0;
}
