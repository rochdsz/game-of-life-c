#include "grid.h"
#include <stdlib.h>
#include <stdio.h>

int **current_grid;
int **next_grid;

void allocate_grids() {
    // Allocate row pointer arrays
    current_grid = malloc(ROWS * sizeof(int*));
    next_grid    = malloc(ROWS * sizeof(int*));

    if (!current_grid || !next_grid) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    // Allocate a contiguous block for each grid
    current_grid[0] = malloc(ROWS * COLS * sizeof(int));
    next_grid[0]    = malloc(ROWS * COLS * sizeof(int));

    if (!current_grid[0] || !next_grid[0]) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    // Set row pointers
    for (int i = 1; i < ROWS; i++) {
        current_grid[i] = current_grid[0] + i * COLS;
        next_grid[i]    = next_grid[0] + i * COLS;
    }
}


void cleanup_grids() {
    free(current_grid[0]);  // contiguous block
    free(next_grid[0]);

    free(current_grid);     // row pointers
    free(next_grid);
}

void swap_grids() {
    int** temp = current_grid; // 1. Hold the address of the current grid
    current_grid = next_grid;  // 2. Point 'current' to the calculated 'next' data
    next_grid = temp;          // 3. Recycle the old 'current' memory as the new 'next' buffer
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
