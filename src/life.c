#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define ROWS 20
#define COLS 40
#define DELAY_MICROSECONDS 100000 // 0.1 seconds between frames

#define LIVE_CELL 'O'
#define DEAD_CELL '.'

int** current_grid;
int** next_grid;

void allocate_grids();
void init_grid();
void display_grid();
int count_neighbors(int r, int c);
void swap_grids();
void cleanup_grids();
void compute_next_generation();

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

void allocate_grids() {
    current_grid = (int**)malloc(ROWS * sizeof(int*));
    next_grid = (int**)malloc(ROWS * sizeof(int*));

    for (int i = 0; i < ROWS; i++) {
        // For each row, allocate the columns
        current_grid[i] = (int*)malloc(COLS * sizeof(int));
        next_grid[i] = (int*)malloc(COLS * sizeof(int));
    }
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
            if (current_grid[i][j] == 1) {
                printf("%c ", LIVE_CELL);
            } else {
                printf("%c ", DEAD_CELL);
            }
        }
        printf("\n");
    }
}

// Count how many of the 8 neighbors are ALIVE
int count_neighbors(int r, int c) {
    int count = 0;

    // Check a 3x3 box around the cell (r, c)
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            
            // Skip the center cell
            if (i == 0 && j == 0) continue;

            //Toroidal wrapping
            int neighbor_r = (r + i + ROWS) % ROWS;
            int neighbor_c = (c + j + COLS) % COLS;

            count += current_grid[neighbor_r][neighbor_c];
        }
    }
    return count;
}

void swap_grids() {
    int** temp = current_grid; // 1. Hold the address of the current grid
    current_grid = next_grid;  // 2. Point 'current' to the calculated 'next' data
    next_grid = temp;          // 3. Recycle the old 'current' memory as the new 'next' buffer
}

void cleanup_grids() {
    for (int i = 0; i < ROWS; i++) {
        free(current_grid[i]);
        free(next_grid[i]);
    }
    free(current_grid);
    free(next_grid);
}

// Apply the 4 Rules of Life
void compute_next_generation() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            
            // 1. Get info from the CURRENT buffer
            int neighbors = count_neighbors(i, j);
            int is_alive = current_grid[i][j];

            // 2. Apply rules and write to NEXT buffer
            if (is_alive) {
                // Rule 1 & 3: Underpopulation (<2) or Overpopulation (>3) -> DIES
                if (neighbors < 2 || neighbors > 3) {
                    next_grid[i][j] = 0;
                }
                // Rule 2: Survival (2 or 3) -> LIVES
                else {
                    next_grid[i][j] = 1;
                }
            } else {
                // Rule 4: Reproduction (exactly 3) -> BECOMES ALIVE
                if (neighbors == 3) {
                    next_grid[i][j] = 1;
                } else {
                    next_grid[i][j] = 0;
                }
            }
        }
    }

    // 3. Pointer Swap
    swap_grids();
}
