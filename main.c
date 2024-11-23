



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
const char *level_1_words[] = {"apple", "grape", "peach", "berry", "melon", "lemon", "mango", "guava", "olive"};
const char *level_2_words[] = {"brazil","canada","cyprus","france","greece","jordan","kuwait","mexico","norway","poland"};
const char *level_3_words[] = {
    "journey", 
    "freight", 
    "abandon", 
    "concert", 
    "beneath", 
    "fortune", 
    "garment", 
    "landing", 
    "cabinet", 
    "deserve", 
    "element", 
    "fashion", 
    "holiday", 
    "justice", 
    "monster", 
    "network", 
    "officer", 
    "picture", 
    "quarter", 
    "request"
};

const char **level_words[] = {level_1_words, level_2_words, level_3_words};
const int level_lengths[] = {5, 6, 7};  // Word lengths for each level
const int level_sizes[] = {10, 10, 10}; // Number of words for each level

void print_feedback(const char *guess, const char *secret, int word_length)
{
    char feedback[word_length + 1];
    int i, j;
    int secret_used[word_length];
    memset(secret_used, 0, sizeof(secret_used)); 

    for (i = 0; i < word_length; i++)
    {
        if (guess[i] == secret[i])
        {
            feedback[i] = 'G';
            secret_used[i] = 1; 
        }
        else
        {
            feedback[i] = '_';
        }
    }

    for (i = 0; i < word_length; i++)
    {
        if (feedback[i] == '_')
        {
            for (j = 0; j < word_length; j++)
            {
                if (guess[i] == secret[j] && feedback[j] != 'G' && secret_used[j] == 0)
                {
                    feedback[i] = 'Y';  
                    secret_used[j] = 1; 
                    break;
                }
            }
        }
    }

    printf("\nFeedback: ");
    for (i = 0; i < word_length; i++)
    {
        if (feedback[i] == 'G')
        {
            printf(GREEN "%c " RESET, guess[i]);
        }
        else if (feedback[i] == 'Y')
        {
            printf(YELLOW "%c " RESET, guess[i]);
        }
        else
        {
            printf(RED "_ " RESET);
        }
    }
    printf("\n\n");
}

void display_instructions(int level)
{
    if (level == 0)
    {
        printf(MAGENTA "\n===== How to Play Wordle - Level %d =====\n" RESET, level + 1);
        printf("Guess the Wordle in %d tries.\n\n", MAX_ATTEMPTS);
        printf(" - Each guess must be a valid %d-letter word.\n", level_lengths[level]);
        printf(" - The color of the tiles will change to show how close your guess was to the word.\n");
        printf(" - Press 'H' to get a hint.\n\n");

        // Example feedback
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
        while (getchar() != '\n')
            ; // Wait for the user to press Enter
    }
}
int is_valid_input(const char *input, int word_length)
{
    if (strlen(input) != word_length)
        return 0;
    for (int i = 0; i < word_length; i++)
    {
        if (!isalpha(input[i]))
        {
            return 0; 
        }
    }
    return 1; 
}

void start_game(int level)
{
    srand(time(NULL));
    const char *secret = level_words[level][rand() % level_sizes[level]];
    int word_length = level_lengths[level];
    char guess[word_length + 1];
    int attempts = 0;
    int hint_given = 0;

    printf(MAGENTA "\nWelcome to Wordle - Level %d!\n" RESET, level + 1);
    printf(BLUE "Guess the %d-letter word. You have %d attempts.\n\n" RESET, word_length, MAX_ATTEMPTS);

    while (attempts < MAX_ATTEMPTS)
    {
        printf("Attempt %d: ", attempts + 1);

        fgets(guess, word_length + 2, stdin);
        guess[strcspn(guess, "\n")] = '\0';

        if (strcmp(guess, "H") == 0 && !hint_given)
        {
            hint_given = 1;
            printf(MAGENTA "Hint: ");
            if (level == 0)
                printf("The word is a fruit.\n" RESET);
            else if (level == 1)
                printf("The word is a country name.\n" RESET);
            else
                printf("The word is a random 7-letter word.\n" RESET);
            continue;
        }

        if (!is_valid_input(guess, word_length))
        {
            printf(RED "Invalid input! Enter a valid %d-letter word.\n\n" RESET, word_length);
            continue;
        }

        print_feedback(guess, secret, word_length);
        attempts++;

        if (strcmp(guess, secret) == 0)
        {
            printf(GREEN "Congratulations! You've guessed the word: %s\n\n" RESET, secret);
            return;
        }
    }

    printf(RED "Out of attempts! The word was: %s\n\n" RESET, secret);
}

int main()
{
    char choice;
    int level = 0;

    printf(MAGENTA "\n========== Welcome to Wordle ==========\n" RESET);
    printf("Press Enter to play or type 'Q' to quit: ");

    choice = getchar();
    if (choice == 'Q' || choice == 'q')
    {
        printf(RED "Exiting the game. Goodbye!\n\n" RESET);
        return 0;
    }
    if (choice != '\n')
    {
        while (getchar() != '\n');
    }

    while (level < LEVEL_COUNT)
    {
        display_instructions(level);
        start_game(level);

        level++; 
        if (level < LEVEL_COUNT)
        {
            printf(MAGENTA "\nCongratulations! Proceeding to Level %d...\n\n" RESET, level + 1);
        }
    }

    printf(GREEN "You've completed all levels. Thanks for playing Wordle!\n\n" RESET);
    return 0;
}
