#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAXSTEPS 8
#define SIZE 8 


typedef char chessPos[2];

typedef struct chessPosArray {
	unsigned int size;
	chessPos* positions;
}chessPosArray;

typedef struct _chessPosCell {
	chessPos position;
	struct _chessPosCell* next;

} chessPosCell;

typedef struct chessPosList {
	chessPosCell* head;
	chessPosCell* tail;
} chessPosList;

typedef struct _treeNode {
	chessPos position;
	struct treeNodeListCell* next;
} treeNode;

typedef struct _treeNodeListCell {
	struct _treeNode* node;
	struct treeNodeListCell* next;
} treeNodeListCell;

typedef struct _pathTree {
	struct _treeNode* root;

} pathTree;

/* function declaration */
bool isValid(int row, int col);
chessPosArray*** validKnightMoves();
void possibleMoves(chessPosArray* posArray, int i, int j);
char intToLetter(int num);

/* display */
void printBoard(int mat[][SIZE]);
void printPosArr(chessPosArray* arr);
void display(chessPosList* lst);

/* List functions */
chessPosCell* listNodeToPrintC(char row, int col, chessPosCell* next);
treeNodeListCell* arrToTreeListRec(chessPosArray* arr);
pathTree findAllPossibleKnightPaths(chessPos* strartingPosition);
treeNode* findAllPossibleKnightPathsHelper(chessPos* startPos, chessPosArray*** mat, bool*** path);
void makeEmptyList(chessPosList* lst);
void deleteCell(chessPosCell* prev);
void displayMovesOnBoard(chessPosList* lst);
void insertNodeToEndList(chessPosList* lst, chessPosCell* tail);
void checkAllocation(void* res);





void printValids(chessPosArray*** c)
{
	int i, j, k;
	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			printf("%c%c:", i + 'A', j + '1');
			for (k = 0; k < c[i][j]->size; k++)
				printf("{%c,%c} ", c[i][j]->positions[k][0], c[i][j]->positions[k][1]);
			printf("\n");
		}
	}
}

/*
void insertDataToEndList(chessPosList* lst, char row, int col)
{
	chessPosCell* newTail;
	newTail = listNodeToPrintC(row, col, NULL);
	insertNodeToEndList(lst, newTail);
}*/
/*
void createList(chessPosList* lst)
{
	int size = 3;
	char i;
	int j;
	while (size > 0)
	{
		scanf("%c", &i);
		scanf("%c", &j);
		getchar();
		insertDataToEndList(lst, i, j);
		size--;
	}
} */

void main()
{

	chessPosArray*** test = validKnightMoves();
	printValids(test);

	chessPos cPos; 
	cPos[0] = 'B';
	cPos[1] = '3';
	pathTree tr; 
	
	tr = findAllPossibleKnightPaths(&cPos);
//	printf("1: %c 2: %c\n",tr.root->position[0],tr.root->position[1]);

	// display();

}


chessPosArray*** validKnightMoves()
{
	int i, j, ChessPosSize;

	chessPosArray*** res;
	res = (chessPosArray***)malloc(sizeof(chessPosArray**) * SIZE);
	checkAllocation(res);

	for (i = 0; i < SIZE; i++)
	{
		res[i] = (chessPosArray**)malloc(sizeof(chessPosArray*) * SIZE);
		checkAllocation(res[i]);

		for (j = 0; j < SIZE; j++)
		{
			res[i][j] = (chessPosArray**)malloc(sizeof(chessPosArray*) * SIZE);
			res[i][j]->positions = (chessPos*)malloc(sizeof(chessPos*) * SIZE);
			checkAllocation(res[i][j]->positions);
			possibleMoves(res[i][j], i, j);
		}
	}

	return res;
}

void possibleMoves(chessPosArray* posArray, int i, int j)
{
	int counter = 0;
	int k, m;
	chessPos* pos;
	pos = posArray->positions;

	for (k = -2; k <= 2; k++)
	{
		for (m = -2; m <= 2; m++)
		{
			if (k != m && k != -m && k != 0 && m != 0 && isValid(i + 1 + k, j + 1 + m))
			{
				pos[counter][0] = intToLetter(i + 1 + k);
				pos[counter][1] = (j + 1 + m) + '0';
				counter++;
			}
		}
	}
	posArray->size = counter;
}


bool isValid(int row, int col)
{
	if (row <= SIZE && row >= 1 && col <= SIZE && col >= 1)
		return true;
	return false;
}

char intToLetter(int num)
{
	char res = 'A';
	res += (num - 1);
	return res;
}
/* init arr to false */
void initMatToFalse(bool arr[][SIZE])
{
	int i, j;
	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			arr[i][j] = false;
		}
	}
}

void display(chessPosList* lst)
{
	bool mat[SIZE][SIZE];

	chessPosCell* chessCell = lst->head;
	chessPosCell* prevCell = NULL;
	initMatToFalse(mat);

	while (chessCell != NULL)
	{
		int row = chessCell->position[0] - 'A'; /* transform pos chars to board size to start from row and col 0 to 8 ) */
		int col = chessCell->position[1] - '1';
		if (mat[row][col] == true)
			deleteCell(prevCell);
		else
			mat[row][col] = true;

		prevCell = chessCell;
		chessCell = chessCell->next;
	}

	displayMovesOnBoard(lst);
	free(mat);
}

void displayMovesOnBoard(chessPosList* lst)
{
	int i = 1;
	int posInRow, posInCol;
	int mat[SIZE][SIZE];

	chessPosCell* cell = lst->head;
	while (cell != NULL)
	{
		posInRow = cell->position[0] - 'A';
		posInCol = cell->position[1] - '1';
		mat[posInRow][posInCol] = i;
		i++;
	}
	printBoard(mat);
}

void deleteCell(chessPosCell* prev)
{
	chessPosCell* del = prev->next;
	prev->next = del->next;
	free(del);
}


void makeEmptyList(chessPosList* lst)
{
	lst->head = lst->tail = NULL;
}


void printBoard(int mat[][SIZE])
{
	int i, j;


	for (i = 1; i <= 8; i++)
	{
		printf("%3d", i);
	}
	printf("\n");
	for (i = 1; i <= 8; i++)
	{
		printf("%c", intToLetter(i));
		for (j = 1; j <= 8; j++)
		{
			printf("%c", mat[i][j]);
			printf(" |");

		}
		printf("\n");
	}

}

treeNodeListCell* arrToTreeListRec(chessPosArray* arr)
{
	int i;
	treeNodeListCell* res;
	res = (treeNodeListCell**)malloc(sizeof(treeNodeListCell*));
//	res->node = (treeNode**)malloc(sizeof(treeNode*));
//	res->next = (treeNodeListCell**)malloc(sizeof(treeNodeListCell*));
//	checkAllocation(res->node);

	if (arr->size == 0)
		return NULL;

	else {
		res->node = (treeNode*)malloc(sizeof(treeNode));
		res->node->position[0] = arr->positions[arr->size-1][0];
		res->node->position[1] = arr->positions[arr->size-1][1];
		arr->size--;
		res->next = arrToTreeListRec(arr);
	}

	return res;
}



bool visitedAllChild(chessPos* startPos, treeNodeListCell* childList,bool path[][SIZE])
{
	treeNodeListCell* current = childList;

	int childPosRow, childPosCol; 
	while (current != NULL)
	{
		childPosRow = current->node->position[0] - 'A'; /* cast to int */
		childPosCol = current->node->position[1] - '1';
		printf("check 2 : %d %d\n", childPosRow,childPosCol);
		if (path[childPosRow][childPosCol] == false)
			return false;
		else 
			current = current->next;
	}
	return true;
	
}
pathTree findAllPossibleKnightPaths(chessPos* strartingPosition)
{
	pathTree res;
	chessPosArray*** mat;
	bool path[SIZE][SIZE];
	initMatToFalse(path);
	mat = validKnightMoves();

	res.root = findAllPossibleKnightPathsHelper(strartingPosition, mat, path);

	return res;
}

treeNode* findAllPossibleKnightPathsHelper(chessPos* startPos, chessPosArray*** mat, bool path[][SIZE])
{
	int i, posRow, posCol;
	treeNodeListCell* childList;
	pathTree tr; 
	tr.root = (treeNode*)malloc(sizeof(treeNode));
	checkAllocation(tr.root);

	posRow = startPos[0][0] - 'A';
	posCol = startPos[0][1] - '1';
	printf("YUVALI %c %c", startPos[0][0], startPos[0][1]);
	childList = arrToTreeListRec(mat[posRow][posCol]);

	if (visitedAllChild(startPos, childList, path ))
		 return NULL;
	else
	{
		tr.root->position[0] = startPos[0];
		tr.root->position[1] = startPos[1];

		tr.root->next = childList;

		path[posRow][posCol] = true;

		while (childList != NULL)
		{
			childList->node->next = findAllPossibleKnightPathsHelper(childList, mat, path)->next;
			childList = childList->next;
		}
		path[posRow][posCol] = false;
		
	}
	return tr.root;
}

//chessPosCell* createNewListNode(chessPosList* listNode)
//{
//	chessPosCell* res;
//	res = (chessPosCell*)malloc(sizeof(chessPosCell));
//	
//}

chessPosCell* listNodeToPrintC(char row, int col, chessPosCell* next)
{
	chessPosCell* res;
	res = (chessPosCell*)malloc(sizeof(chessPosCell));
	res->position[0] = row;
	res->position[1] = col;
	res->next = next;
	return res;
}
void insertNodeToEndList(chessPosList* lst, chessPosCell* tail)
{
	if (lst->head == NULL)
		lst->head = lst->tail = tail;
	else
	{
		lst->tail->next = tail;
		lst->tail = tail;
	}
	tail->next = NULL;
}

void checkAllocation(void* res)
{
	if (!res)
	{
		fprintf(stderr, "Allocation failure: \n");
		exit(1);
	}
}