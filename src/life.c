#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

// Initialize the grid with a specific pattern
void init_grid() {
    // Clear the grid with all zeros
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            current_grid[i][j] = 0;
        }
    }

    // Glider pattern
    //  . O .
    //  . . O
    //  O O O
    current_grid[1][2] = 1;
    current_grid[2][3] = 1;
    current_grid[3][1] = 1;
    current_grid[3][2] = 1;
    current_grid[3][3] = 1;
    
    printf("Grid initialized with Glider pattern.\n");
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

            // Calculate neighbor coordinates
            int neighbor_r = r + i;
            int neighbor_c = c + j;

            // Boundary Check: Ensure we don't read outside the array
            // If neighbor is off-screen, we treat it as dead (do nothing).
            if (neighbor_r >= 0 && neighbor_r < ROWS && neighbor_c >= 0 && neighbor_c < COLS) {
                
                // Add the value (1 if alive, 0 if dead)
                count += current_grid[neighbor_r][neighbor_c];
            }
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
