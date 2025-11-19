//Game of Life - main file, GUI implementation
#include "raylib.h"
#include "Sparse.h" //Functions are used from Sparse.c - so remember to compile with that file. 
#include <stdlib.h>
#include <stdio.h>

int main() {
    //If you want to change the grid layout, here's the place.
	int ROWS=0;
	int COLS=0;
    int CELL_SIZE = 20;
	while (ROWS < 20 || ROWS > 100 || COLS < 20 || COLS > 100) {
        printf("Enter the number of rows (20 to 100): ");
        scanf("%d", &ROWS);
        printf("Enter the number of columns (20 to 100): ");
        scanf("%d", &COLS);
        if (ROWS < 20 || ROWS > 100 || COLS < 20 || COLS > 100) printf("The rows and Column's can't exceed 100 and can't be bellow 20\n");
    }
    //In case the grid becomes too large for a 1080p screen or too small for the instructions to be visible, change cell size:
    if (ROWS>60 || COLS>60) CELL_SIZE /= 2;
    else if (ROWS<35 || COLS<35) CELL_SIZE *= 2;
    
    InitWindow(COLS * CELL_SIZE, ROWS * CELL_SIZE, "Game of Life in a Sparse Matrix");
    
	int speed = 15;   //This will be the fps that we can change
    SetTargetFPS(speed); //Increase fps = more speed (+increasing speed might fix the multi-click problem)

    SparseGrid *grid = initialize_grid(NULL, ROWS, COLS); //"grid" will store the current state of the sparse grid at any given instance.
    bool running = false; //Will store start/stop state

    while (!WindowShouldClose()) { //What they call a "Game Loop" - this will loop for every frame until the window is closed
        //Taking mouse input
        Vector2 mouse = GetMousePosition();
        int col = mouse.x / CELL_SIZE;
        int row = mouse.y / CELL_SIZE;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { //Resurrect cell
            add_cell(grid, row, col,0, ROWS, COLS);
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
        //Instructions on the top right corner
        DrawText("Left click: Add cell | Right click: Remove | Space: Start/Pause", 10, 10, 20, GREEN);
        EndDrawing();
    }
    //FINALLY, Cleanup:
    free_grid(grid, ROWS, COLS);
    CloseWindow();
    return 0;
}
