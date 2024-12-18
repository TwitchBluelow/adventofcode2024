#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ROWS 57
#define COLS 57

typedef struct Point {
    int x;
    int y;
} Point;

void parseInputToArray(char **field, char *input) {
    int index = 0;
    for (int i = 0; i < ROWS; i++) {
        field[i] = (char *)malloc(COLS * sizeof(char));
        for (int j = 0; j < COLS; j++) {
            // Skip newline characters and ensure input does not terminate prematurely
            while (input[index] == '\n') {
                index++;
            }
            if (input[index] == '\0') {
                printf("End of input reached\n");
                return; // Stop processing if we hit the end of input
            }
            field[i][j] = input[index++];
            //printf("add %c\n", field[i][j]);
        }
    }
}


// Print the 2D field (for debugging)
void printField(char **field) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%c", field[i][j]);
        }
        printf("\n");
    }
}

// Depth-First Search to calculate reachable goals from a starting point
int countReachableGoals(char **field, Point start) {
    bool visited[ROWS][COLS] = {false};
    int goalCount = 0;

    // Movement directions: up, down, left, right
    int directions[4][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };

    // Stack for DFS
    Point stack[ROWS * COLS];
    int stackSize = 0;

    // Push the starting point onto the stack
    stack[stackSize++] = start;
    visited[start.x][start.y] = true;

    // DFS loop
    while (stackSize > 0) {
        Point current = stack[--stackSize];
        char currentValue = field[current.x][current.y];

        // Count goals ('9')
        if (currentValue == '9') {
            goalCount++;
        }

        // Explore neighbors
        for (int i = 0; i < 4; i++) {
            int newX = current.x + directions[i][0];
            int newY = current.y + directions[i][1];

            // Check if the move is valid
            if (newX >= 0 && newX < ROWS && newY >= 0 && newY < COLS &&
                !visited[newX][newY] && field[newX][newY] == currentValue + 1) {
                
                visited[newX][newY] = true;
                stack[stackSize++] = (Point){newX, newY};
            }
        }
    }

    return goalCount;
}

// Read input from a file
char *readInputFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Determine file size
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    // Allocate memory and read the file content
    char *input = (char *)malloc((fileSize + 1) * sizeof(char));
    fread(input, sizeof(char), fileSize, file);
    input[fileSize] = '\0'; // Null-terminate the string

    fclose(file);
    return input;
}

int main() {
    //const char* filename = "input.txt";
    char *input = readInputFromFile("input.txt");
    //char *input = readInputFromFile("input2.txt");
    //char *input = readInputFromFile("example.txt");

    // Parse input into a 2D array
    char **topoField = (char **)malloc(ROWS * sizeof(char *));
    parseInputToArray(topoField, input);
    //printField(topoField);

    // Find all '0' points and calculate trail scores and sum them
    int sum = 0;
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            if (topoField[i][j] == '0') {
                Point start = {i, j};
                int goalCount = countReachableGoals(topoField, start);
                printf("Start at (%d, %d) and found %d goals\n", i, j, goalCount);
                sum += goalCount;
            }
        }
    }

    printf("Sum of goals: %d\n", sum);

    // Free memory
    for (int i = 0; i < ROWS; i++) {
        free(topoField[i]);
    }
    free(topoField);
    free(input);

    return 0;
}
