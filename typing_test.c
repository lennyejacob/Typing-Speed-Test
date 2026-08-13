#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME 50
#define MAX_TEXT 300
#define MAX_SCORES 100

// Structure to store a leaderboard entry
struct Score {
    char name[MAX_NAME];
    float wpm;
    float accuracy;
};

// Sentences for each difficulty
const char *easy[] = {
    "C is a powerful programming language.",
    "I enjoy learning new programming skills.",
    "Coding helps us solve problems.",
    "Practice makes programming easier.",
    "Computers follow instructions very quickly."
};

const char *medium[] = {
    "Programming allows us to solve complex problems efficiently.",
    "Data structures help programmers organize information effectively.",
    "Software development requires patience and logical thinking.",
    "Learning C provides a strong foundation for programming.",
    "Algorithms help computers solve problems step by step."
};

const char *hard[] = {
    "Efficient algorithms and well designed data structures are fundamental to software development.",
    "Operating systems manage computer resources and provide services to application programs.",
    "Understanding memory management is essential when developing programs using the C language.",
    "Good programmers write code that is efficient, readable, maintainable, and easy to understand.",
    "Computer science combines mathematics, logic, algorithms, and programming to solve complex problems."
};

#define EASY_COUNT 5
#define MEDIUM_COUNT 5
#define HARD_COUNT 5


// Function to display the title
void displayTitle() {
    printf("\n");
    printf("============================================\n");
    printf("          C TYPING SPEED TEST\n");
    printf("============================================\n");
}


// Function to display difficulty menu
int chooseDifficulty() {
    int choice;

    printf("\nChoose Difficulty:\n");
    printf("1. Easy\n");
    printf("2. Medium\n");
    printf("3. Hard\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    // Clear input buffer
    while (getchar() != '\n');

    return choice;
}


// Function to get a random sentence
const char* getSentence(int difficulty) {
    int index;

    if (difficulty == 1) {
        index = rand() % EASY_COUNT;
        return easy[index];
    }
    else if (difficulty == 2) {
        index = rand() % MEDIUM_COUNT;
        return medium[index];
    }
    else {
        index = rand() % HARD_COUNT;
        return hard[index];
    }
}


// Function to calculate accuracy
float calculateAccuracy(const char *original, const char *typed) {
    int originalLength = strlen(original);
    int typedLength = strlen(typed);

    int correct = 0;
    int i;

    int maxLength = originalLength;

    if (typedLength < maxLength)
        maxLength = typedLength;

    for (i = 0; i < maxLength; i++) {
        if (original[i] == typed[i])
            correct++;
    }

    if (typedLength == 0)
        return 0;

    return ((float)correct / typedLength) * 100;
}


// Function to load scores from file
int loadScores(struct Score scores[]) {
    FILE *file;
    int count = 0;

    file = fopen("leaderboard.txt", "r");

    if (file == NULL) {
        return 0;
    }

    while (count < MAX_SCORES &&
           fscanf(file, "%49s %f %f",
                  scores[count].name,
                  &scores[count].wpm,
                  &scores[count].accuracy) == 3) {

        count++;
    }

    fclose(file);

    return count;
}


// Function to save scores to file
void saveScore(struct Score score) {
    FILE *file;

    file = fopen("leaderboard.txt", "a");

    if (file == NULL) {
        printf("Error opening leaderboard file!\n");
        return;
    }

    fprintf(file, "%s %.2f %.2f\n",
            score.name,
            score.wpm,
            score.accuracy);

    fclose(file);
}


// Function to sort leaderboard
void sortScores(struct Score scores[], int count) {
    int i, j;
    struct Score temp;

    for (i = 0; i < count - 1; i++) {
        for (j = 0; j < count - i - 1; j++) {

            if (scores[j].wpm < scores[j + 1].wpm) {

                temp = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = temp;
            }
        }
    }
}


// Function to display leaderboard
void displayLeaderboard() {
    struct Score scores[MAX_SCORES];

    int count = loadScores(scores);

    if (count == 0) {
        printf("\n============================================\n");
        printf("              LEADERBOARD\n");
        printf("============================================\n");
        printf("No scores recorded yet.\n");
        return;
    }

    sortScores(scores, count);

    printf("\n");
    printf("============================================\n");
    printf("              LEADERBOARD\n");
    printf("============================================\n");

    printf("%-6s %-20s %-10s %-10s\n",
           "Rank", "Name", "WPM", "Accuracy");

    printf("--------------------------------------------\n");

    int limit = count;

    if (limit > 10)
        limit = 10;

    for (int i = 0; i < limit; i++) {

        printf("%-6d %-20s %-10.2f %-10.2f%%\n",
               i + 1,
               scores[i].name,
               scores[i].wpm,
               scores[i].accuracy);
    }

    printf("============================================\n");
}


// Function to run the typing test
void startTest() {

    char name[MAX_NAME];
    char typed[MAX_TEXT];

    int difficulty;

    const char *sentence;

    printf("\nEnter your name: ");
    fgets(name, MAX_NAME, stdin);

    // Remove newline from name
    name[strcspn(name, "\n")] = '\0';

    difficulty = chooseDifficulty();

    // Validate difficulty
    while (difficulty < 1 || difficulty > 3) {

        printf("Invalid choice. Enter 1, 2, or 3: ");
        scanf("%d", &difficulty);

        while (getchar() != '\n');
    }

    sentence = getSentence(difficulty);

    printf("\n============================================\n");
    printf("                 GET READY!\n");
    printf("============================================\n");

    printf("\nType the following sentence:\n\n");

    printf("\"%s\"\n\n", sentence);

    printf("Press ENTER when you're ready...");

    getchar();

    printf("\nSTART TYPING:\n> ");

    // Start timer
    clock_t start = clock();

    fgets(typed, MAX_TEXT, stdin);

    // Stop timer
    clock_t end = clock();

    // Remove newline
    typed[strcspn(typed, "\n")] = '\0';

    // Calculate time
    double timeTaken =
        (double)(end - start) / CLOCKS_PER_SEC;

    // Calculate number of characters
    int characters = strlen(typed);

    // Calculate WPM
    double minutes = timeTaken / 60.0;

    float wpm = 0;

    if (minutes > 0) {
        wpm = (characters / 5.0) / minutes;
    }

    // Calculate accuracy
    float accuracy =
        calculateAccuracy(sentence, typed);

    // Create score
    struct Score score;

    strcpy(score.name, name);

    score.wpm = wpm;
    score.accuracy = accuracy;

    // Display results
    printf("\n");
    printf("============================================\n");
    printf("                 RESULTS\n");
    printf("============================================\n");

    printf("Time Taken : %.2f seconds\n", timeTaken);
    printf("Characters : %d\n", characters);
    printf("WPM        : %.2f\n", wpm);
    printf("Accuracy   : %.2f%%\n", accuracy);

    printf("============================================\n");

    // Performance message
    if (wpm >= 60 && accuracy >= 95) {
        printf("Excellent! ??\n");
    }
    else if (wpm >= 40 && accuracy >= 90) {
        printf("Great job! ??\n");
    }
    else if (wpm >= 25) {
        printf("Good effort! Keep practicing.\n");
    }
    else {
        printf("Keep practicing! You will improve.\n");
    }

    // Save score
    saveScore(score);

    printf("\nYour score has been added to the leaderboard!\n");
}


// Main function
int main() {

    int choice;

    // Initialize random number generator
    srand(time(NULL));

    do {

        displayTitle();

        printf("\n1. Start Typing Test\n");
        printf("2. View Leaderboard\n");
        printf("3. Exit\n");

        printf("\nEnter choice: ");
        scanf("%d", &choice);

        // Clear input buffer
        while (getchar() != '\n');

        switch (choice) {

            case 1:
                startTest();
                break;

            case 2:
                displayLeaderboard();
                break;

            case 3:
                printf("\nThank you for using the Typing Speed Test!\n");
                printf("Goodbye!\n");
                break;

            default:
                printf("\nInvalid choice. Please try again.\n");
        }

    } while (choice != 3);

    return 0;
}
