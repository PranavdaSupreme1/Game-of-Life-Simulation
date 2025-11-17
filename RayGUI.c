//Game of Life - main file, GUI implementation
/*TO IMPLEMENT:
1. Changing count of rows and columns and being able to scroll
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
	int speed = 10;   //This will be the fps that we can change
    SetTargetFPS(speed); //Increase fps = more speed (+increasing speed might fix the multi-click problem)

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
		
		//Speed manipulation - press UP to increase speed, DOWN to reduce speed.
		if (IsKeyPressed(KEY_UP)) {
			speed += 5; //Increments of five per each key up press
			if (speed>100) speed = 100;   //Speed can't be above 100
			SetTargetFPS(speed);
		}
		if (IsKeyPressed(KEY_DOWN)) {
			speed -= 5; //Decrements of five per each key down press
			if (speed < 1) speed = 1; //Speed can't be below 1 fps
			SetTargetFPS(speed);
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
        // --- Zoom slider UI ---
        DrawText("Zoom:", 20, ROWS * cellSize + 15, 20, DARKGRAY);
        Rectangle slider = {90, ROWS * cellSize + 15, zoomSliderWidth, 20};

        // Slider bar (manual implementation)
        DrawRectangleRec(slider, LIGHTGRAY);
        float knobPos = slider.x + ((zoom - zoomMin) / (zoomMax - zoomMin)) * slider.width;
        DrawCircle(knobPos, slider.y + slider.height / 2, 8, GRAY);

        // Mouse control for slider
        if (CheckCollisionPointRec(mouse, slider) && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            float normalized = (mouse.x - slider.x) / slider.width;
            if (normalized < 0) normalized = 0;
            if (normalized > 1) normalized = 1;
            zoom = zoomMin + normalized * (zoomMax - zoomMin);
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
