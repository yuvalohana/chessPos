#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAXSTEPS 5
#define SIZE 5 


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
	struct treeNodeListCell* next_possible_position;
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
bool visitedAllChild(bool path[][SIZE], chessPosArray* childArr);
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
treeNode* findAllPossibleKnightPathsHelper(chessPos startPos, chessPosArray*** mat, bool path[][SIZE]);
treeNodeListCell* insertToHeadList(treeNodeListCell* childList, treeNode* child);
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
	//printValids(test);

	chessPos cPos;
	cPos[0] = 'A';
	cPos[1] = '1';
	pathTree tr;

	tr = findAllPossibleKnightPaths(&cPos);
/*	treeNodeListCell* cell;
	cell = (treeNodeListCell*)malloc(sizeof(treeNodeListCell));
	cell = tr.root->next_possible_position;

	printf("1: %c 2: %c\n", tr.root->position[0], tr.root->position[1]);
	while (cell->node!=NULL)
	{
		printf("1: %c 2: %c\n", cell->node->position[0], cell->node->position[1]);

		cell->node = cell->next;
	}*/
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
			res[i][j] = (chessPosArray**)malloc(sizeof(chessPosArray*));
			res[i][j]->positions = (chessPos*)malloc(sizeof(chessPos) * SIZE);
			checkAllocation(res[i][j]->positions);
			possibleMoves(res[i][j], i, j);
			//realoc
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

	if (arr->size == 0)
		return NULL;
	else {
		res->node = (treeNode*)malloc(sizeof(treeNode));
		res->next = (treeNodeListCell*)malloc(sizeof(treeNodeListCell));
		res->node->position[0] = arr->positions[arr->size - 1][0];
		res->node->position[1] = arr->positions[arr->size - 1][1];
		arr->size--;


		res->next = arrToTreeListRec(arr);
	}

	return res;
}

pathTree findAllPossibleKnightPaths(chessPos* strartingPosition)
{
	pathTree res;
	chessPosArray*** mat;
	bool path[SIZE][SIZE];
	initMatToFalse(path);
	mat = validKnightMoves();
	res.root = (treeNode*)malloc(sizeof(treeNode));
	res.root = findAllPossibleKnightPathsHelper(*strartingPosition, mat, path);

	return res;
}


treeNode* findAllPossibleKnightPathsHelper(chessPos startPos, chessPosArray*** mat, bool path[][SIZE])
{
	int i, posRow, posCol;
	treeNode* child;
	treeNode* root;

	posRow = startPos[0] - 'A';
	posCol = startPos[1] - '1';

	if (visitedAllChild(path, mat[posRow][posCol]))
		return NULL;

	root = (treeNode*)malloc(sizeof(treeNode));
	checkAllocation(root);
	root->next_possible_position = NULL;

	path[posRow][posCol] = true;
	root->position[0] = startPos[0];
	root->position[1] = startPos[1];


	for (i = 0; i < mat[posRow][posCol]->size; i++)
	{
		if ((path[(mat[posRow][posCol]->positions[i][0]) - 'A'][(mat[posRow][posCol]->positions[i][1]) - '1']) == false)
		{

			child = findAllPossibleKnightPathsHelper(mat[posRow][posCol]->positions[i], mat, path);
			
			root->next_possible_position = insertToHeadList(root->next_possible_position, child); //???
		}

	}
	path[posRow][posCol] = false;
	return root;
}

bool visitedAllChild(bool path[][SIZE], chessPosArray* childArr)
{
	int i;
	for (i = 0; i < childArr->size; i++)
	{
		if (path[childArr->positions[i][0]][childArr->positions[i][1]] == false) /* if didnt visit a child */
			return false;
	}

	return true;
}

treeNodeListCell* insertToHeadList(treeNodeListCell* childList, treeNode* child)
{
	treeNodeListCell* temp;
	temp = (treeNodeListCell*)malloc(sizeof(treeNodeListCell));
	treeNodeListCell* newNode;
	newNode = (treeNodeListCell*)malloc(sizeof(treeNodeListCell));

	newNode->node = child;
	newNode->next = NULL;

	if (childList == NULL)
		childList = newNode;

	else {
		temp = childList;
		childList = newNode;
		newNode->next = temp;
	}

	return childList;
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