#ifndef GRID_H
#define GRID_H

#define ROWS 20
#define COLS 40

extern int **current_grid;
extern int **next_grid;

// Memory management
void allocate_grids();
void cleanup_grids();
void swap_grids();

// Simulation rules
int count_neighbors(int r, int c);
void compute_next_generation();

#endif
