//Game of Life - main file, GUI implementation
/*TO IMPLEMENT:
1. Speed Alteration
2. Preloaded figures (lexicon)
*/
#include "raylib.h"
#include "Sparse.h" //Functions are used from Sparse.c - so remember to compile with that file. 
#include <stdlib.h>

//If you want to change the grid layout, here's the place.
#define CELL_SIZE 20
#define ROWS 50
#define COLS 50

int main() {
    InitWindow(COLS * CELL_SIZE, ROWS * CELL_SIZE, "Game of Life in a Sparse Matrix");
    SetTargetFPS(10); //Increase fps = more speed (+increasing speed might fix the multi-click problem)

    SparseGrid *grid = initialize_grid(NULL, ROWS, COLS); //"grid" will store the current state of the sparse grid at any given instance.
    bool running = false; //Will store start/stop state

    while (!WindowShouldClose()) { //What they call a "Game Loop" - this will loop for every frame until the window is closed
        //Taking mouse input
        Vector2 mouse = GetMousePosition();
        int col = mouse.x / CELL_SIZE;
        int row = mouse.y / CELL_SIZE;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { //Resurrect cell
            add_cell(grid, row, col, ROWS, COLS);
        }
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) { //Kill cell
            remove_cell(grid, row, col);
        }
        if (IsKeyPressed(KEY_SPACE)) { //PAUSE!!
            running = !running;
        }

        if (running) {
            SparseGrid *newGrid = evolve_generation(grid, ROWS, COLS); //Getting new state of grid
            free_grid(grid, ROWS, COLS);    //Cleaning the grid of its previous state
            grid = newGrid;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        //Drawing the grid itself
        for (int i = 0; i <= ROWS; i++)
            DrawLine(0, i * CELL_SIZE, COLS * CELL_SIZE, i * CELL_SIZE, LIGHTGRAY);
        for (int j = 0; j <= COLS; j++)
            DrawLine(j * CELL_SIZE, 0, j * CELL_SIZE, ROWS * CELL_SIZE, LIGHTGRAY);
        //Shading the alive cells
        for (int i = 0; i < ROWS; i++) {
            CellNode *temp = grid->row[i];
            while (temp) {
                DrawRectangle(temp->col * CELL_SIZE, temp->row * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLACK);
                temp = temp->right;
            }
        }
        //Instructions on the top right corner
        DrawText("Left click: Add cell | Right click: Remove | Space: Start/Pause", 10, 10, 20, GREEN);
        EndDrawing();
    }
    //FINALLY, Cleanup:
    free_grid(grid, ROWS, COLS);
    CloseWindow();
    return 0;
}