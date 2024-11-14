#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

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
    int i, j; // Declare loop variables outside the loops

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
            printf(GREEN "%c " RESET, guess[i]); // Green for correct position
        } else if (feedback[i] == 'Y') {
            printf(YELLOW "%c " RESET, guess[i]); // Yellow for correct letter, wrong position
        } else {
            printf(RED "_ " RESET); // Red underscore for incorrect letters
        }
    }
    printf("\n");
}

void display_instructions() {
    printf(MAGENTA "\n===== How to Play Wordle =====\n" RESET);
    printf("1. You have %d attempts to guess the secret 5-letter word.\n", MAX_ATTEMPTS);
    printf("2. After each guess, you will receive feedback:\n");
    printf("   - " GREEN "Green" RESET ": Letter is correct and in the correct position.\n");
    printf("   - " YELLOW "Yellow" RESET ": Letter is correct but in the wrong position.\n");
    printf("   - " RED "Red underscore" RESET ": Letter is not in the word.\n");
    printf("3. Enter a valid 5-letter word each time. Invalid words will prompt an error.\n");
    printf("4. Try to guess the word within the allowed attempts to win!\n\n");
    printf("Press Enter to start the game...\n");
    while (getchar() != '\n'); // Wait for the user to press Enter
}

void start_game() {
    srand(time(NULL));
    const char *secret = word_list[rand() % WORD_LIST_SIZE];
    char guess[WORD_LENGTH + 1];
    int attempts = 0;

    // Display welcome message with color
    printf(MAGENTA "Welcome to Wordle!\n" RESET);
    printf(BLUE "Guess the 5-letter word. You have %d attempts.\n" RESET, MAX_ATTEMPTS);

    while (attempts < MAX_ATTEMPTS) {
        printf("Attempt %d: ", attempts + 1);
        scanf("%5s", guess);

        if (strlen(guess) != WORD_LENGTH) {
            printf(RED "Invalid input. Please enter a 5-letter word.\n" RESET);
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

    // Starting menu
    printf(MAGENTA "===== Welcome to Wordle =====\n" RESET);
    printf("Press Enter to play or type 'Q' to quit: ");
    
    choice = getchar();
    if (choice == 'Q' || choice == 'q') {
        printf(RED "Exiting the game. Goodbye!\n" RESET);
        return 0;
    }
    // Consume any remaining characters in the input buffer
    if (choice != '\n') {
        while (getchar() != '\n');
    }

    // Display instructions and start the game
    display_instructions();
    start_game();
    return 0;
}
