/*
   Assigned by:
   Gilad Bahar 318939469
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

/* Constant definitions */

#define N 3
#define COLS 4
#define ROWS 4


/* Structs definitions */

typedef struct number
{
	unsigned long long num;
	int sum;
} Number;

typedef struct triad
{
	int i, j, value;
} Triad;

typedef struct item
{
	Triad data;
	struct item* next;
} Item;

/* Functions declarations */

void Ex1();
void Ex2();
void Ex3();

Number* primeSums(unsigned long long n1, unsigned long long n2, int* p_size);
int** matrixMaxNeighbor(int A[][COLS], int rows, int cols);
void CreateThreeList(int** A, int rows, int cols, Item** pL1, Item** pl2);



/* Declarations of auxiliary functions */
int isPrime(int num);
int digitSum(unsigned long long num);

int neighborMax(int A[][COLS], int rows, int cols, int i, int j);
int** Alloc_matrix(int rows, int cols);

Triad createThree(int i, int j, int value);
void inserttostart(Item** head, Triad data);


void input_matrix(int a[][COLS], int rows, int cols);
void inputDynamicMatrix(int** a, int rows, int cols);
void free_matrix(int** c, int n);
void print_Static_matrix(int a[][N], int rows, int cols);
void print_dynamic_matrix(int** a, int rows, int cols);


Item* createitem(Triad data);
void PrintList(Item** head);
void DeleteFromStart(Item** head);
void FreeList(Item** head);


void printArray(Number* arr, int size);

/* Main function */
int main()
{
	int select = 0, i, all_Ex_in_loop = 0;
	printf("Run menu once or cyclically?\n(Once - enter 0, cyclically - enter other number) ");
	if (scanf("%d", &all_Ex_in_loop) == 1)
		do
		{
			for (i = 1; i <= N; i++)
				printf("Ex%d--->%d\n", i, i);
			printf("EXIT-->0\n");
			do {
				select = 0;
				printf("please select 0-%d : ", N);
				scanf_s("%d", &select);
			} while ((select < 0) || (select > N));
			switch (select)
			{
			case 1: Ex1(); break;
			case 2: Ex2(); break;
			case 3: Ex3(); break;
			}
		} while (all_Ex_in_loop && select);
		return 0;
}

void Ex1()
{
	int* arr = NULL;
	long long num1, num2;
	int psize;
	printf("Enter 2 positive numbers called n1, n2 with a space between them.\nAttention! n2 must be greater than n1 (n2 > n1): ");
	scanf_s("%llu %llu", &num1, &num2);
	arr = primeSums(num1, num2, &psize);
	printArray(arr, psize);
	free(arr);
}

void Ex2()
{
	int A[ROWS][COLS];
	int** B = NULL;

	input_matrix(A, ROWS, COLS);
	printf("Original matrix:\n");
	print_Static_matrix(A, ROWS, COLS);
	B = matrixMaxNeighbor(A, ROWS, COLS);
	printf("New matrix: \n");
	print_dynamic_matrix(B, ROWS, COLS);
	free_matrix(B, ROWS);
}

void Ex3()
{
	int** arr = NULL;
	int rows, cols;
	Item* list1 = NULL;
	Item* list2 = NULL;

	printf("Enter how much rows: ");
	scanf_s("%d", &rows);
	printf("Enter how much cols: ");
	scanf_s("%d", &cols);

	arr = Alloc_matrix(rows, cols);
	inputDynamicMatrix(arr, rows, cols);
	print_dynamic_matrix(arr, rows, cols);
	CreateThreeList(arr, rows, cols, &list1, &list2);

	printf("List1 is: ");
	PrintList(&list1);

	printf("List2 is: ");
	PrintList(&list2);

	FreeList(&list1);
	FreeList(&list2);
	free_matrix(arr, rows);
}

/*Ex1*/
int isPrime(int num)
{
	int i;
	if (num == 2)
		return 1;
	if (num < 2 || num % 2 == 0)
		return 0;
	for (i = 3; i * i <= num; i += 2)
	{
		if (num % i == 0)
			return 0;
	}
	return 1;
}
int digitSum(unsigned long long num)
{
	int sum = 0;
	while (num != 0)
	{
		sum += num % 10;
		num = num / 10;
	}
	return sum;
}
/* This function get 2 natural numbers and calculate all of
   the prime numbers in the range between them. It returns a pointer
   to a an array with all of the numbers. */
Number* primeSums(unsigned long long n1, unsigned long long n2, int* p_size)
{
	*p_size = 0;
	long long i;
	Number* arr = NULL, * temp = NULL;
	int size = 1;
	int j = 0;
	int sum;
	arr = (Number*)malloc(size * sizeof(Number));
	if (!arr)return NULL;
	for (i = n1; i <= n2; i++)
	{
		sum = digitSum(i);
		if (isPrime(sum))
		{
			if (j == size)
			{
				size *= 2;
				temp = (Number*)realloc(arr, size * sizeof(Number));
				if (!temp)return NULL;
				arr = temp;
			}
			arr[j].num = i;
			arr[j].sum = sum;
			j++;
		}
	}
	temp = (Number*)realloc(arr, j * sizeof(Number));
	if (!temp || j == 0)
	{
		arr = NULL;
		return arr;
	}
	arr = temp;
	*p_size = j;
	return arr;

}

/*Ex2*/
int** Alloc_matrix(int rows, int cols)
{
	int** c = NULL, i;
	c = (int**)calloc(rows, sizeof(int*));
	if (!c)
		return NULL;
	for (i = 0; i < rows; i++)
	{
		c[i] = (int*)calloc(cols, sizeof(int));
		if (c[i] == NULL)
		{
			free_matrix(c, i);
			return NULL;
		}
	}
	return c;
}
/* This function recieve 2D array, num of cols, num of rows, and index of a number in the matrix A.
It returns the maximum neighbor of the index. */
int neighborMax(int A[][COLS], int rows, int cols, int i, int j)
{
	int max = INT_MIN;;
	if (i - 1 >= 0 && A[i - 1][j] > max)
		max = A[i - 1][j];
	if (i + 1 < rows && A[i + 1][j] > max)
		max = A[i + 1][j];
	if (j + 1 < cols && A[i][j + 1] > max)
		max = A[i][j + 1];
	if (j - 1 >= 0 && A[i][j - 1] > max)
		max = A[i][j - 1];
	return max;
}

/* This function create a new matrix 'B' and return a pointer to the adress of it. */
int** matrixMaxNeighbor(int A[][COLS], int rows, int cols)
{
	int** matrix = Alloc_matrix(rows, cols);
	int i, j;
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			matrix[i][j] = neighborMax(A, rows, cols, i, j);
		}
	}
	return matrix;
}

/*Ex3*/
/* This function get 3 parameters, insert them to the parameter 'Triad' and return it. */
Triad createThree(int i, int j, int value)
{
	Triad new_triad;
	new_triad.i = i;;
	new_triad.j = j;
	new_triad.value = value;
	return new_triad;
}
void inserttostart(Item** head, Triad data)
{
	Item* newitem = createitem(data);
	newitem->next = *head;
	*head = newitem;
}
/* This function recieve a matrix, and seperate it into two groups.
It return by reference 2 linked lists - 1 for each group.
Group 1 - The elements in the matrix are equal to the coordinate values.
Group 2 - The members of the matrix whose values constitutes an inovice series. */
void CreateThreeList(int** A, int rows, int cols, Item** pL1, Item** pl2)
{
	int i, j;
	for (i = 0; i < rows; i++)
		for (j = 0; j < cols; j++)
		{
			if (i + j == A[i][j])
				inserttostart(pL1, createThree(i, j, A[i][j]));
			if (j - i == A[i][j] - j)
				inserttostart(pl2, createThree(i, j, A[i][j]));
		}
}


/* Auxiliary functions */
void printArray(Number* arr, int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		printf("\nThe number is: %llu\n", arr[i].num);
		printf("The sum is: %d\n", arr[i].sum);
	}
}


void input_matrix(int a[][COLS], int rows, int cols)
{
	int i, j;

	printf("Enter the matrix %d X %d\n", rows, cols);
	for (i = 0; i < rows; i++)//rows
		for (j = 0; j < cols; j++)//cols
			scanf_s("%d", &(a[i][j]));
}

void inputDynamicMatrix(int** a, int rows, int cols)
{
	int i, j;
	printf("Enter the matrix %d X %d\n", rows, cols);
	for (i = 0; i < rows; i++)//rows
		for (j = 0; j < cols; j++)//cols
			scanf_s("%d", &(a[i][j]));
}
void free_matrix(int** c, int n)//full matrice
{
	int i;
	for (i = 0; i < n; i++)
	{
		free(c[i]);
		c[i] = NULL;
	}
	free(c);
	c = NULL;
}
void print_Static_matrix(int a[][COLS], int rows, int cols)
{
	int i, j;
	for (i = 0; i < rows; i++)//rows
	{
		for (j = 0; j < cols; j++)//cols
			printf("%5d", a[i][j]);
		printf("\n");
	}
}
void print_dynamic_matrix(int** a, int rows, int cols)
{
	int i, j;
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
			printf("%8d", a[i][j]);
		printf("\n");
	}
}



Item* createitem(Triad data)
{
	Item* newnode;
	newnode = (Item*)malloc(sizeof(Item));
	newnode->data = data;
	newnode->next = NULL;
	return newnode;
}
void PrintList(Item** head)
{
	Item* curr = *head;
	if (*head == NULL)
		printf("The list is empty\n");
	else
	{
		while (curr != NULL)
		{
			printf("%d, ", curr->data.i);
			printf("%d, ", curr->data.j);
			printf("%d ", curr->data.value);
			if (curr->next != NULL)
				printf("-> ");
			curr = curr->next;
		}
	}
	printf("\n");
}
void DeleteFromStart(Item** head)
{
	Item* delnode = *head;
	if (delnode == NULL)
		printf("The list is Empty");
	else
	{
		*head = (*head)->next;
		free(delnode);
	}
}
void FreeList(Item** head)
{
	while (*head != NULL)
		DeleteFromStart(head);
}
