#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "grid.h"

#define CELL_SIZE 20
#define WINDOW_WIDTH  (COLS * CELL_SIZE)
#define WINDOW_HEIGHT (ROWS * CELL_SIZE)
#define DELAY_MS 100

void init_grid();
void render_grid(SDL_Renderer *renderer);

int main() {
    // 1. Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // 2. Create Window & Renderer
    SDL_Window *window =
        SDL_CreateWindow(
            "Game of Life",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            0
        );

    if (!window) {
        printf("Window Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer *renderer =
        SDL_CreateRenderer(
            window,
            -1,
            SDL_RENDERER_ACCELERATED
        );

    allocate_grids();
    init_grid();

    bool running = true;
    SDL_Event event;

    while (running) {

        // Event handling
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
        }

        // Clear screen (black)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw cells
        render_grid(renderer);

        // Show frame
        SDL_RenderPresent(renderer);

        compute_next_generation();

        SDL_Delay(DELAY_MS);
    }

    cleanup_grids();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void init_grid() {
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            current_grid[i][j] = 0;

    // Glider pattern
    current_grid[1][2] = 1;
    current_grid[2][3] = 1;
    current_grid[3][1] = 1;
    current_grid[3][2] = 1;
    current_grid[3][3] = 1;
}

void render_grid(SDL_Renderer *renderer) {

    SDL_Rect cell;
    cell.w = CELL_SIZE;
    cell.h = CELL_SIZE;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {

            if (current_grid[i][j]) {

                // Alive = white
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

                cell.x = j * CELL_SIZE;
                cell.y = i * CELL_SIZE;

                SDL_RenderFillRect(renderer, &cell);
            }
        }
    }
}
