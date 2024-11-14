#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define MAX_ATTEMPTS 6
#define LEVEL_COUNT 3

// ANSI color codes for colored output
#define RESET "\033[0m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define RED "\033[1;31m"
#define BLUE "\033[1;34m"
#define MAGENTA "\033[1;35m"

// Word lists for different levels (5, 6, and 7 letter words)
const char *level_1_words[] = { "apple", "grape", "peach", "berry", "melon" };
const char *level_2_words[] = { "banana", "orange", "cherry", "papaya", "kiwi" };
const char *level_3_words[] = { "plumber", "genuine", "cabbage", "spacious", "climate" };

const char **level_words[] = { level_1_words, level_2_words, level_3_words };
const int level_lengths[] = { 5, 6, 7 }; // Word lengths for each level
const int level_sizes[] = { 5, 5, 5 }; // Number of words for each level

void print_feedback(const char *guess, const char *secret, int word_length) {
    char feedback[word_length + 1]; // Initialize feedback with underscores
    int i, j;

    // First pass: mark correct letters in the correct position
    for (i = 0; i < word_length; i++) {
        if (guess[i] == secret[i]) {
            feedback[i] = 'G'; // Correct position
        } else {
            feedback[i] = '_'; // Placeholder for letters yet to be processed
        }
    }

    // Second pass: mark correct letters in the wrong position
    for (i = 0; i < word_length; i++) {
        if (feedback[i] == '_') {
            for (j = 0; j < word_length; j++) {
                if (guess[i] == secret[j] && feedback[j] != 'G') {
                    feedback[i] = 'Y'; // Correct letter, wrong position
                    break;
                }
            }
        }
    }

    // Print feedback with colors
    printf("Feedback: ");
    for (i = 0; i < word_length; i++) {
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

void display_instructions(int level) {
    if (level == 0) {
        printf(MAGENTA "\n===== How to Play Wordle - Level %d =====\n" RESET, level + 1);
        printf("Guess the Wordle in %d tries.\n\n", MAX_ATTEMPTS);
        printf(" - Each guess must be a valid %d-letter word.\n", level_lengths[level]);
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
}

int is_valid_input(const char *input, int word_length) {
    if (strlen(input) != word_length) return 0;
    for (int i = 0; i < word_length; i++) {
        if (!isalpha(input[i])) {
            return 0; // Invalid character found
        }
    }
    return 1; // Valid input
}

void start_game(int level) {
    srand(time(NULL));
    const char *secret = level_words[level][rand() % level_sizes[level]];
    int word_length = level_lengths[level];
    char guess[word_length + 1];
    int attempts = 0;

    printf(MAGENTA "Welcome to Wordle - Level %d!\n" RESET, level + 1);
    printf(BLUE "Guess the %d-letter word. You have %d attempts.\n" RESET, word_length, MAX_ATTEMPTS);

    while (attempts < MAX_ATTEMPTS) {
        printf("Attempt %d: ", attempts + 1);
        
        // Use fgets to read input, this allows longer input and better handling of newlines
        fgets(guess, word_length + 2, stdin); // +2 to accommodate the null terminator and newline
        guess[strcspn(guess, "\n")] = '\0'; // Remove newline character from input

        if (!is_valid_input(guess, word_length)) {
            printf(RED "Invalid input. Please enter a valid %d-letter word.\n" RESET, word_length);
            continue;
        }

        print_feedback(guess, secret, word_length);
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
    int level = 0;

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

    while (level < LEVEL_COUNT) {
        display_instructions(level);
        start_game(level);
        
        level++; // Move to the next level after a successful guess
        if (level < LEVEL_COUNT) {
            printf(MAGENTA "Congratulations! You've advanced to level %d.\n" RESET, level + 1);
        }
    }

    printf("You've completed all levels. Thanks for playing!\n");
    return 0;
}
