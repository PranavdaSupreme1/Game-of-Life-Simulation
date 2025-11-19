#include <stdio.h>
#include <stdlib.h>
#include "Sparse.h" // Custom header file has all function defs and structures

int find(SparseGrid *grid, int row, int col,int totrows, int totcols){
	if (row < 0 || row >= totrows || col < 0 || col >= totcols) return 0;
	CellNode *temp=grid->col[col];
	if (temp==NULL) return 0;
	while (temp->down!=NULL && temp->row!=row){
			temp=temp->down;
	}
	return (temp->row==row);
}

SparseGrid* initialize_grid(SparseGrid *grid, int m, int n) {
	grid=(SparseGrid*) malloc(sizeof(SparseGrid));
    for (int i=0;i<m;i++) grid->row[i]=NULL;
    for (int j=0;j<n;j++) grid->col[j]=NULL;
	return grid;
}

CellNode* create_cell(int row, int col){
	CellNode *new_cell = (CellNode*) malloc(sizeof(CellNode));
    new_cell->row=row;
    new_cell->col=col;
    new_cell->right=NULL;
	new_cell->left=NULL;
	new_cell->up=NULL;
	new_cell->down=NULL;
	return new_cell;
}

SparseGrid* add_cell(SparseGrid *grid, int row, int col,int totrows, int totcols) {
	if (find(grid,row,col,totrows,totcols)) return grid;
    CellNode *new_cell=create_cell(row,col);

    CellNode *current_row=grid->row[row];
    CellNode *current_col=grid->col[col];

    // FOR THE ROWS
	CellNode *temp_row=current_row;
	//Front insertion
	if (temp_row==NULL) grid->row[row]=new_cell;
	else{
		if (temp_row->col>col){
			new_cell->right=temp_row;
			temp_row->left=new_cell;
			grid->row[row]=new_cell;
		}
		else{
			while (temp_row->col<col && temp_row->right!=NULL){
				temp_row=temp_row->right;
			}
			//Back insertion
			if (temp_row->col<col){
				new_cell->left=temp_row;
				temp_row->right=new_cell;
			}
			//Middle insertion
			else{
				new_cell->left=temp_row->left;
				new_cell->right=temp_row;
				(temp_row->left)->right=new_cell;
				temp_row->left=new_cell;
			}
		}
	}
    // FOR THE COLUMNS
	CellNode *temp_col=current_col;
    //Front insertion
	if (temp_col==NULL) grid->col[col]=new_cell;
	else{
		if (temp_col->row>row){
			new_cell->down=temp_col;
			temp_col->up=new_cell;
			grid->col[col]=new_cell;
		}
		else{
			while (temp_col->row<row && temp_col->down!=NULL){
				temp_col=temp_col->down;
			}
            //Back insertion
			if (temp_col->row<row){
				new_cell->up=temp_col;
				temp_col->down=new_cell;
			}
            //Middle insertion
			else{
				new_cell->up=temp_col->up;
				new_cell->down=temp_col;
				(temp_col->up)->down=new_cell;
				temp_col->up=new_cell;
			}
		}
	}
	return grid;
}

void remove_cell(SparseGrid *grid, int row, int col) {
    CellNode *temp=grid->col[col];
    if (temp!=NULL){
        while (temp->row!=row && temp->down!=NULL) temp=temp->down;
        if (temp!=NULL){
            if (temp->row==row){
                if (temp->left==NULL) grid->row[row]=temp->right;
                else (temp->left)->right=temp->right;
                if (temp->right!=NULL) (temp->right)->left=temp->left;
                if (temp->up==NULL) grid->col[col]=temp->down;
                else (temp->up)->down=temp->down;
                if (temp->down!=NULL) (temp->down)->up=temp->up;
                free(temp);
                temp=NULL;
            }
        }
    }
}

int count_neighbors(SparseGrid *grid, int row, int col, int totrows, int totcols) {
    int count;
    count=find(grid,row-1,col-1,totrows,totcols)+find(grid,row-1,col,totrows,totcols)+find(grid,row-1,col+1,totrows,totcols)+find(grid,row,col-1,totrows,totcols)+find(grid,row,col+1,totrows,totcols)+find(grid,row+1,col-1,totrows,totcols)+find(grid,row+1,col,totrows,totcols)+find(grid,row+1,col+1,totrows,totcols);
    return count;
}

void free_grid(SparseGrid *grid, int rows, int cols) {
	CellNode* temp;
	for (int i=0;i<rows;i++){
		temp=grid->row[i];
		while (temp){
			CellNode* new=temp->right;
			remove_cell(grid,i,temp->col);
			temp=new;
		}
		grid->row[i]=NULL;
	}
	for (int j=0;j<cols;j++) grid->col[j]=NULL;
}

SparseGrid* evolve_generation(SparseGrid *current_grid, int rows, int cols) {
    SparseGrid *next_grid = initialize_grid(NULL, rows, cols);
    int count;
	int j;
    for (int i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            count = count_neighbors(current_grid, i, j,rows,cols);
            if (count == 3 || (find(current_grid, i, j,rows,cols) && count == 2)) add_cell(next_grid, i, j,rows,cols);
        }
    }
    return next_grid;
}
/*void print_grid(SparseGrid *grid, int rows, int cols) {
    for (int i = 1; i < rows; i++) {
        CellNode *temp = grid->row[i];
        for (int j = 1; j < cols; j++) {
            if (temp && temp->col == j) {
                printf("* ");
                temp = temp->right;
            } else {
                printf(". ");
            }
        }
        printf("\n");
    }
}
int main(){
	SparseGrid *grid;
	grid=initialize_grid(grid,5,5);
	add_cell(grid,2,3,5,5);
	add_cell(grid,2,4,5,5);
	add_cell(grid,3,3,5,5);
	add_cell(grid,3,2,5,5);
	print_grid(grid,5,5);
	SparseGrid *new;
	new=evolve_generation(grid,5,5);
	print_grid(new,5,5);
	return 0;
}*/
//Commented the main() function because with GUI, the entry point for the program should be RayGUI.c
