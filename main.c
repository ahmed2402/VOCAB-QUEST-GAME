#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define WORD_LENGTH 5
#define MAX_ATTEMPTS 6
#define WORD_LIST_SIZE 10

// ANSI color codes for colored output
#define RESET "\033[0m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define RED "\033[1;31m"
#define BLUE "\033[1;34m"
#define MAGENTA "\033[1;35m"

// Word list
const char *word_list[WORD_LIST_SIZE] = {
    "apple", "grape", "peach", "berry", "melon",
    "mango", "lemon", "plumb", "guava", "kiwif"
};

void print_feedback(const char *guess, const char *secret) {
    char feedback[WORD_LENGTH + 1] = "_____"; // Initialize feedback with underscores
    int i, j;

    // First pass: mark correct letters in the correct position
    for (i = 0; i < WORD_LENGTH; i++) {
        if (guess[i] == secret[i]) {
            feedback[i] = 'G'; // Correct position
        }
    }

    // Second pass: mark correct letters in the wrong position
    for (i = 0; i < WORD_LENGTH; i++) {
        if (guess[i] != secret[i]) {
            for (j = 0; j < WORD_LENGTH; j++) {
                if (guess[i] == secret[j] && feedback[j] != 'G') {
                    feedback[i] = 'Y'; // Correct letter, wrong position
                    break;
                }
            }
        }
    }

    // Print feedback with colors
    printf("Feedback: ");
    for (i = 0; i < WORD_LENGTH; i++) {
        if (feedback[i] == 'G') {
            printf(GREEN "%c " RESET, guess[i]);
        } else if (feedback[i] == 'Y') {
            printf(YELLOW "%c " RESET, guess[i]);
        } else {
            printf(RED "_ " RESET);
        }
    }
    printf("\n");
}

void display_instructions() {
    printf(MAGENTA "\n===== How to Play Wordle =====\n" RESET);
    printf("Guess the Wordle in %d tries.\n\n", MAX_ATTEMPTS);
    printf(" - Each guess must be a valid 5-letter word.\n");
    printf(" - The color of the tiles will change to show how close your guess was to the word.\n\n");

    // Examples section
    printf("Examples:\n");

    // Example 1: Correct letter in the correct spot
    printf(GREEN " W " RESET " O R D Y\n");
    printf("W is in the word and in the correct spot.\n\n");

    // Example 2: Correct letter but in the wrong spot
    printf("L " YELLOW " I " RESET " G H T\n");
    printf("I is in the word but in the wrong spot.\n\n");

    // Example 3: Letter not in the word
    printf("R O G " RED " U " RESET " E\n");
    printf("U is not in the word in any spot.\n\n");

    printf("Press Enter to start the game...\n");
    while (getchar() != '\n'); // Wait for the user to press Enter
}

int is_valid_input(const char *input) {
    if (strlen(input) != WORD_LENGTH) return 0;
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (!isalpha(input[i])) {
            return 0; // Invalid character found
        }
    }
    return 1; // Valid input
}

void start_game() {
    srand(time(NULL));
    const char *secret = word_list[rand() % WORD_LIST_SIZE];
    char guess[WORD_LENGTH + 1];
    int attempts = 0;

    printf(MAGENTA "Welcome to Wordle!\n" RESET);
    printf(BLUE "Guess the 5-letter word. You have %d attempts.\n" RESET, MAX_ATTEMPTS);

    while (attempts < MAX_ATTEMPTS) {
        printf("Attempt %d: ", attempts + 1);
        scanf("%5s", guess);

        if (!is_valid_input(guess)) {
            printf(RED "Invalid input. Please enter a valid 5-letter word.\n" RESET);
            continue;
        }

        print_feedback(guess, secret);
        attempts++;

        if (strcmp(guess, secret) == 0) {
            printf(GREEN "Congratulations! You've guessed the word: %s\n" RESET, secret);
            return;
        }
    }

    printf(RED "Sorry! You've used all attempts. The word was: %s\n" RESET, secret);
}

int main() {
    char choice;

    printf(MAGENTA "===== Welcome to Wordle =====\n" RESET);
    printf("Press Enter to play or type 'Q' to quit: ");
    
    choice = getchar();
    if (choice == 'Q' || choice == 'q') {
        printf(RED "Exiting the game. Goodbye!\n" RESET);
        return 0;
    }
    if (choice != '\n') {
        while (getchar() != '\n');
    }

    display_instructions();
    start_game();
    return 0;
}
