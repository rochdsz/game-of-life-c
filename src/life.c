#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "grid.h"

#define DELAY_MICROSECONDS 100000 // 0.1 seconds between frames

#define LIVE_CELL 'O'
#define DEAD_CELL '.'

void init_grid();
void display_grid();

int main() {
    allocate_grids();
    init_grid();

    for (int gen = 0; gen < 1000; gen++) {
        display_grid();

        compute_next_generation();

        usleep(DELAY_MICROSECONDS); 
    }

    cleanup_grids();
    return 0;
}

void init_grid() {
    int choice;
    int r, c;

    // 1. Clear the grid with all zeros
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            current_grid[i][j] = 0;
        }
    }

    // 2. Display the Menu
    printf("\n--- GAME OF LIFE SETUP ---\n");
    printf("1. Glider (Moving Pattern)\n");
    printf("2. Blinker (Oscillator)\n");
    printf("3. Random Soup\n");
    printf("4. Custom Input (Draw your own)\n");
    printf("Select an option: ");
    if (scanf("%d", &choice) != 1) {
        choice = 1; // default
    }

    // 3. Handle the Choice
    switch(choice) {
        case 1: // Glider
            current_grid[1][2] = 1;
            current_grid[2][3] = 1;
            current_grid[3][1] = 1;
            current_grid[3][2] = 1;
            current_grid[3][3] = 1;
            printf("Glider selected.\n");
            break;

        case 2: // Blinker
            current_grid[10][10] = 1;
            current_grid[10][11] = 1;
            current_grid[10][12] = 1;
            printf("Blinker selected.\n");
            break;

        case 3: // Random
            srand(time(0)); // Seed the random number generator
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    // 20% chance of a cell being alive
                    if ((rand() % 5) == 0) {
                        current_grid[i][j] = 1;
                    }
                }
            }
            printf("Random Chaos initialized.\n");
            break;

        case 4: // Custom Input Loop
             while (1) {
                // Show current state
                display_grid();
                
                printf("\n--- CUSTOM DRAW MODE ---\n");
                printf("Enter 'Row Col' to toggle a cell (e.g. 5 5).\n");
                printf("Enter '-1 -1' to START simulation.\n");
                printf(">> ");

                if (scanf("%d %d", &r, &c) != 2) {
                    break;
                }

                if (r == -1 && c == -1) break;

                if (r >= 0 && r < ROWS && c >= 0 && c < COLS) {
                    // Toggle: 1->0 or 0->1
                    current_grid[r][c] = !current_grid[r][c];
                } else {
                    printf("Invalid coordinate! Range: 0-%d, 0-%d\n", ROWS-1, COLS-1);
                }
            }
            break;

        default:
            printf("Invalid choice! Defaulting to Glider.\n");
            current_grid[1][2] = 1;
            current_grid[2][3] = 1;
            current_grid[3][1] = 1;
            current_grid[3][2] = 1;
            current_grid[3][3] = 1;
            break;
    }
    
    printf("Starting simulation in 1 second...\n");
    sleep(1);
}

void display_grid() {
    // ANSI Escape Code: "\033[H\033[J" clears the terminal screen
    printf("\033[H\033[J"); 

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            // If cell is 1, print 'O', else print '.'
            printf("%c ", current_grid[i][j]? LIVE_CELL : DEAD_CELL);
        }
        printf("\n");
    }
}
