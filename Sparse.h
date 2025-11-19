// Sparse.h - defining here all the functions and structs we'll be using for the Game of Life

//Data structures
typedef struct CellNode {
    int row;
    int col;
    int count;
    struct CellNode *right, *left, *up, *down;
} CellNode;

typedef struct {
    CellNode *row[100];
    CellNode *col[100];	// Head of a master linked list of all live cells, sorted
} SparseGrid;

//Note: Only the functions used in RayGUI.c and Sparse.c are declared here.
SparseGrid* initialize_grid(SparseGrid *grid, int m, int n);    //Allocates memory, sets all cells to dead (NULL)
SparseGrid* add_cell(SparseGrid *grid, int row, int col, int totrows, int totcols); //Resurrects cell at (row,col)
void remove_cell(SparseGrid *grid, int row, int col); //Kills cell at (row,col).
int find(SparseGrid *grid, int row, int col, int totrows, int totcols); //Returns 1 if cell at (row,col) is alive, else 0
SparseGrid* evolve_generation(SparseGrid *current_grid, int rows, int cols); //Calculate next frame using Conway's rules
void free_grid(SparseGrid *grid, int rows, int cols); //Kills all cells, clears sparse grid.
CellNode* find_cell(SparseGrid *grid, int row, int col ,int totrows, int totcols);
