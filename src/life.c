#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 

#define ROWS 20
#define COLS 40
#define DELAY_MICROSECONDS 100000 // 0.1 seconds between frames

#define LIVE_CELL 'O'
#define DEAD_CELL '.'

int current_grid[ROWS][COLS];
int next_grid[ROWS][COLS];

void init_grid();
void display_grid();
int count_neighbors(int r, int c);
void compute_next_generation();

int main() {
    init_grid();

    while (1) {
        display_grid();

        compute_next_generation();

        usleep(DELAY_MICROSECONDS); 
    }

    return 0;
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

    // 3. The Swap
    // Copy the 'next' state into 'current' state for the next loop
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            current_grid[i][j] = next_grid[i][j];
        }
    }
}
