#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#define BOARD_SIZE 100
#define NUM_PLAYERS 2

// Define snakes and ladders
int snakes[5][2] = {{16, 6}, {47, 26}, {49, 11}, {56, 53}, {62, 19}};
int ladders[5][2] = {{4, 25}, {13, 46}, {33, 49}, {42, 63}, {50, 69}};

// Questions and answers
const char *questions[] = {
    "What is the capital of France?",
    "Which planet is known as the Red Planet?",
    "What is the largest mammal?",
    "How many continents are there?",
    "What gas do plants use for photosynthesis?"
};

const char *answers[] = {
    "paris",
    "mars",
    "blue whale",
    "seven",
    "carbon dioxide"
};

void initializeBoard(int board[]) {
    int i;
    for (i = 0; i < BOARD_SIZE; i++) {
        board[i] = i + 1;
    }

    // Place snakes and ladders on the board
    for (i = 0; i < 5; i++) {
        board[snakes[i][0] - 1] = snakes[i][1];
        board[ladders[i][0] - 1] = ladders[i][1];
    }
}

void printBoard() {
    printf("\t\t\t\t\tSnake and Ladder Board (Size: %d)\n\n", BOARD_SIZE);
    int row, i, col;
    for (row = 9; row >= 0; row--) {
        // Print the top row of numbers
        if (row % 2 == 0) {
            for (col = 10; col >= 1; col--) {
                int cellNumber = row * 10 + col;
                printf("|%3d ", cellNumber);

                // Check for snakes and ladders
                for (i = 0; i < 5; i++) {
                    if (cellNumber == snakes[i][0]) {
                        printf("S ");
                    } else if (cellNumber == ladders[i][0]) {
                        printf("L ");
                    }
                }
            }
        } else {
            for (col = 1; col <= 10; col++) {
                int cellNumber = row * 10 + col;
                printf("|%3d ", cellNumber);

                // Check for snakes and ladders
                for (i = 0; i < 5; i++) {
                    if (cellNumber == snakes[i][0]) {
                        printf("S ");
                    } else if (cellNumber == ladders[i][0]) {
                        printf("L ");
                    }
                }
            }
        }
        printf("|\n");
    }

    printf("The player who reaches 100 first will be the winner.\n");
}

int rollDice() {
    return rand() % 6 + 1;
}

bool askQuestion(const char *question, const char *answer) {
    char userAnswer[100];
    printf("%s ", question);
    gets(userAnswer);
    return strcmp(userAnswer, answer) == 0;
}

int main() {
    int board[BOARD_SIZE];
    int playerPositions[NUM_PLAYERS] = {0};
    int snakeEffect[NUM_PLAYERS] = {0}; // New array to track snake effect
    int diceValue;
    srand(time(NULL)); // the random number generator for dice

    initializeBoard(board);
    printBoard();

    int currentPlayer = 0;

    while (playerPositions[currentPlayer] < BOARD_SIZE - 1) {
        printf("Player %d's turn. Press Enter to roll the dice...\n", currentPlayer + 1);
        getchar();

        diceValue = rollDice();
        printf("Player %d rolled a %d!\n", currentPlayer + 1, diceValue);

        if (snakeEffect[currentPlayer] == 0) {
            // Player not under the effect of a snake
            int newPosition = playerPositions[currentPlayer] + diceValue;
            if (newPosition >= BOARD_SIZE) {
                newPosition = BOARD_SIZE - 1;
            }
            
            if (newPosition != board[newPosition] - 1) {
                if (newPosition < board[newPosition] - 1) {
                    printf("Player %d: Climb up the ladder to position %d\n", currentPlayer + 1, board[newPosition]);
                    playerPositions[currentPlayer] = board[newPosition] - 1;
                } else {
                    printf("Player %d: Oops! You encountered a snake.\n", currentPlayer + 1);
                    if (askQuestion(questions[diceValue - 1], answers[diceValue - 1])) {
                        printf("Player %d answered correctly, so the snake effect is skipped.\n", currentPlayer + 1);
                    } else {
                        snakeEffect[currentPlayer] = board[newPosition]; // Set the snake effect
                        printf("Player %d was bitten by the snake and moves back to position %d.\n", currentPlayer + 1, board[newPosition]);
                        playerPositions[currentPlayer] = board[newPosition] - 1;
                    }
                }
            } else {
                playerPositions[currentPlayer] = newPosition;
            }
        } else {
            // Player under the effect of a snake
            playerPositions[currentPlayer] = snakeEffect[currentPlayer];
            snakeEffect[currentPlayer] = 0; // Remove the snake effect
        }

        printf("Player %d is now at position %d\n", currentPlayer + 1, playerPositions[currentPlayer] + 1);

        // Switch to the next player
        currentPlayer = (currentPlayer + 1) % NUM_PLAYERS;
    }

    printf("Congratulations! Player %d reached the final position.\n", currentPlayer + 1);

    return 0;
}

