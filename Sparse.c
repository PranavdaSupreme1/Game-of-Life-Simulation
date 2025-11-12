#include <stdio.h>
#include <stdlib.h>
#define SIZE 100
// Data Structures
typedef struct CellNode {
    int row;
    int col;
    struct CellNode *right;
	struct CellNode *left;
	struct CellNode *up;
	struct CellNode *down;
} CellNode;

typedef struct {
    CellNode *row[SIZE];
	CellNode *col[SIZE];	// Head of a master linked list of all live cells, sorted
} SparseGrid;

int find(SparseGrid *grid, int row, int col){
	if (col<0 || col>=SIZE || row<0 || row>=SIZE) return 0;
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



SparseGrid* add_cell(SparseGrid *grid, int row, int col) {
	if (find(grid,row,col)) return grid;
    CellNode *new_cell=create_cell(row,col);

    CellNode *current_row=grid->row[row];
    CellNode *current_col=grid->col[col];

	CellNode *temp_row=current_row;
	//front insertion
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
			//back insertion
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
	CellNode *temp_col=current_col;
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
			if (temp_col->row<row){
				new_cell->up=temp_col;
				temp_col->down=new_cell;
			}
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
	if (col>=0 && col<SIZE && row>=0 && row<SIZE){
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
}



int count_neighbors(SparseGrid *grid, int row, int col) {
    int count;
    count=find(grid,row-1,col-1)+find(grid,row-1,col)+find(grid,row-1,col+1)+find(grid,row,col-1)+find(grid,row,col+1)+find(grid,row+1,col-1)+find(grid,row+1,col)+find(grid,row+1,col+1);
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


SparseGrid* evolve_generation(SparseGrid *current_grid, SparseGrid *next_grid, int rows, int cols) {
    int count;
	next_grid=initialize_grid(next_grid,rows,cols);
	for (int i=0;i<rows;i++){
		for (int j=0;j<cols;j++){
			count=count_neighbors(current_grid,i,j);
			if (count==3 || (find(current_grid,i,j) && count==2)) add_cell(next_grid,i,j);
		}
	}
	free_grid(current_grid,rows,cols);
	free(current_grid);
	current_grid=NULL;
	return next_grid;
}





void print_grid(SparseGrid *grid, int rows, int cols) {
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
	add_cell(grid,2,3);
	add_cell(grid,2,4);
	add_cell(grid,3,3);
	add_cell(grid,3,2);
	print_grid(grid,5,5);
	return 0;
}
