#include <stdio.h>
#include <stdlib.h>
#include "Sparse.h" // Custom header file has all function defs and structures

//Changes:
//Stop loops and static objects


int find(SparseGrid *grid, int row, int col,int totrows, int totcols){
	if (col<0 || col>=totcols || row<0 || row>=totrows) return 0;
	CellNode *temp=grid->col[col];
	if (temp==NULL) return 0;
	while (temp->down!=NULL && temp->row!=row){
			temp=temp->down;
	}
	return (temp->row==row);
}

CellNode* find_cell(SparseGrid *grid, int row, int col,int totrows, int totcols){
	if (col<0 || col>=totcols || row<0 || row>=totrows) return NULL;
	CellNode *temp=grid->col[col];
	if (temp==NULL) return NULL;
	while (temp->down!=NULL && temp->row!=row){
			temp=temp->down;
	}
	if (temp->row==row){ return temp;}
    else{ return NULL;}
}

SparseGrid* initialize_grid(SparseGrid *grid, int m, int n) {
	grid=(SparseGrid*) malloc(sizeof(SparseGrid));
    for (int i=0;i<m;i++) grid->row[i]=NULL;
    for (int j=0;j<n;j++) grid->col[j]=NULL;
	return grid;
}

CellNode* create_cell(int row, int col,int count){
	CellNode *new_cell = (CellNode*) malloc(sizeof(CellNode));
    new_cell->row=row;
    new_cell->col=col;
    new_cell->count=count;
    new_cell->right=NULL;
	new_cell->left=NULL;
	new_cell->up=NULL;
	new_cell->down=NULL;
	return new_cell;
}

SparseGrid* add_cell(SparseGrid *grid, int row, int col,int count,int totrows, int totcols) {
	if (find(grid,row,col,totrows,totcols)) return grid;
    CellNode *new_cell=create_cell(row,col,count);

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
	if (col>=0 && col<100 && row>=0 && row<100){
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

SparseGrid* evolve_generation(SparseGrid *current_grid, int rows, int cols) {
    SparseGrid *next_grid = initialize_grid(NULL, rows, cols);
    int count;
	int j;
    CellNode *cell;
    for (int i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
			//printf("%d",j);
            count = count_neighbors(current_grid, i, j,rows,cols);
            cell=find_cell(current_grid, i, j,rows,cols);
            if (cell==NULL){
                if (count==3) add_cell(next_grid, i, j,0,rows,cols);
            }
            else{
                if (find(current_grid, i, j,rows,cols) && ((count == 2) || (count==3))) add_cell(next_grid, i, j,cell->count+1,rows,cols);
            }
			//printf("%d",count);

        }
		//printf("\n");
    }
    return next_grid;
}



 NODE* frontins(NODE *list,SparseGrid *x)
  {
	NODE *newnode;
	  
    newnode = (NODE *)(malloc(sizeof(NODE)));
	newnode->info=x;
	newnode->next=NULL;
	
	if(list==NULL)list=newnode;
	else {
		newnode->next=list;
		list=newnode;
	}
	return list;
  }
  
  
NODE* frontdel(NODE *list, int ROWS, int COLS)
  { NODE *temp;
	if(list==NULL) return list;
	else {
		temp=list;
		list=list->next;
		free_grid(temp->info, ROWS, COLS);
		free(temp);
	}
	return list;
  }
  
NODE* enddel(NODE *list, int ROWS, int COLS)
  { NODE *temp, *prev;
    temp=list;
	if(list==NULL)//printf("Empty List\n");
	else if(list->next==NULL){list=NULL;
	                          free(temp);
							  }
	else { 
		while(temp->next!=NULL)
		{   prev=temp;
			temp=temp->next;
		}
		prev->next=NULL;
		free_grid(temp->info, ROWS, COLS);
        //printf("Uhhh\n");
		free(temp);
	}
	return list;
  }
  
SparseGrid* endsearch(NODE *list)
  { NODE *temp;
    temp=list;
	if(list==NULL)printf("Empty List");
	else { 
		while(temp->next!=NULL)
		{
			temp=temp->next;
		}
	}
	return temp->info;
  }
  
 
void savefile(SparseGrid* grid, int rows, int cols){
    FILE *fp;
    fp=fopen("save.txt","w");
    fprintf(fp,"%d %d\n",rows,cols);
    for (int i=0;i<rows;i++){
        for (int j=0;j<cols;j++){
            if (find(grid,i,j,rows,cols)) fprintf(fp,"*");
            else fprintf(fp,".");
        }
        fprintf(fp,"\n");
    }
    fclose(fp);
}

SparseGrid* loadfile(int *rows, int *cols){
    FILE *fp;
    SparseGrid *save;
    save=NULL;
    fp=fopen("save.txt","r");
    if (fp==NULL){
        printf("No saves!");
    }
    else{
        if (fscanf(fp,"%d %d",rows,cols)==2){
            save=initialize_grid(NULL,*rows,*cols);
            char c;
            int x, y;
            x=0;
            y=0;
            while ((c=fgetc(fp))!=EOF){
                if (c=='*') add_cell(save,x,y,0,*rows,*cols);
                x=(x+1)%(*cols);
                if (c=='\n') y++;
            }
        }
        fclose(fp);
    }
    return save;
}

/*
int find_duplicate(SparseGrid* curr_grid, SparseGrid* prevgrid, int rows, int cols,int totrows, int totcols){
	return ((find(curr_grid,rows,cols) && find(prev_grid,rows,cols)) || (!find(curr_grid,rows,cols) && !find(prev_grid,rows,cols)));
}

int duplicate_neighbours(SparseGrid* curr_grid, SparseGrid* prevgrid, int rows, int cols,int totrows,int totcols){
	int neighbours=find_duplicate(curr_grid, prevgrid, rows-1, cols-1,totrows,totcols)+find_duplicate(curr_grid, prevgrid, rows, cols-1,totrows,totcols)+find_duplicate(curr_grid, prevgrid, rows+1, cols-1,totrows,totcols)+find_duplicate(curr_grid, prevgrid, rows+1, cols,totrows,totcols)+find_duplicate(curr_grid, prevgrid, rows+1, cols+1,totrows,totcols)+find_duplicate(curr_grid, prevgrid, rows, cols+1,totrows,totcols)+find_duplicate(curr_grid, prevgrid, rows-1, cols+1,totrows,totcols)+find_duplicate(curr_grid, prevgrid, rows-1, cols,totrows,totcols)
	return neighbours;
}


SparseGrid* remove_duplicate(SparseGrid *current_grid,SparseGrid *prev_grid, int rows, int cols) {
    int count;
	int j;
    CellNode *cell;
	CellNode *prev;
    for (int i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
			//printf("%d",j);
			found=(find(current_grid,i,j,rows,cols) && find(prev_grid,i,j,rows,cols))
            if (found){
				cell=find_cell(current_grid,i,j,rows,cols);
				if (cell->count>2){
					if (duplicate_neighbours(current_grid,prev_grid,i,j,rows,cols)==8) remove_cell(current_grid,i,j);
				}
			}
			//printf("%d",count);

        }
		//printf("\n");
    }
    return current_grid;
}*/
/*
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
