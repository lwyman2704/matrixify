//********************************************************
// matrixify.c
// Author: Lorenzo Wyman
// Date: 10/25/2024
// 
// Purpose: This program prints a specific message to the
// command line. The main purpose of this assignment is to
// check and see whether I can connect to the Codespaces Server.
// Input: from
//
// Output: (to the command line) The size of the matrix that is created,
// the matrices that are created, the addition of those matrices if possible
// and the diagonal sum of the summed matrix if possible.
// //********************************************************

#include <stdio.h>
#include <stdlib.h>

//this section is for the "function prototypes" so the compiler does not error like before.

int *createOneDimensionalArray (int size);
int **createMatrix (int rows, int columns);
int **addMatrices (int **defaultMatrix, int **readInMatrix, int rows, int cols);
int computeDiagonalSum (int **matrix, int rows, int cols);
int readFile(char *fileName);
int **collectBytes(int **matrix, int rows, int cols);

int *createOneDimensionalArray(int size) {

    int *defaultMatrix = (int*)malloc(size * sizeof(int));

    if (defaultMatrix == NULL) {
        printf("Memory allocation failed for 'oneDimensionalArray'.\n");
        return NULL;
    }

    for (int i = 0; i < size; i++) { // initialize the array values to 1
        defaultMatrix[i] = 1;
    }

    printf("Array of size %d is created.\n", size);
    
    return defaultMatrix;
}


int **createMatrix(int rows, int columns) {
    
    if (rows <= 0 || columns <= 0) { // check for invalid dimensions.

        printf("Invalid matrix size: (%d, %d)\n", rows, columns);
        return NULL;
    }

    int **matrix = (int**)malloc(rows * sizeof(int*));

    if (matrix == NULL) {

        printf("Memory allocation failed for matrix rows\n");
        return NULL;
    }

   
    for (int i = 0; i < rows; i++) {  // check for allocation failure

        matrix[i] = (int*)malloc(columns * sizeof(int));

        if (matrix[i] == NULL) {
            
            for (int j = 0; j < i; j++) { // free previously allocated rows if allocation fails
                free(matrix[j]);
            }
            free(matrix);  // 

            printf("Memory allocation failed for matrix row %d\n", i);
            return NULL;
        }

      
        for (int j = 0; j < columns; j++) {
            matrix[i][j] = 1; // initialize values to 1
        }
    }

    printf("Matrix of size (%d, %d) is created\n", rows, columns);
    
    return matrix;
}

    
void freeMatrix(int **matrix, int rows) {

    if (matrix == NULL) {
        return; // nothing to free if matrix is NULL
    }
    for (int i = 0; i < rows; i++) {

        if (matrix[i] != NULL) {
            free(matrix[i]);
        }
    }
    free(matrix);
}

int readFile(char *fileName) { 

    int rows;
    int cols;
    
    printf("\n");
    printf("Attempting to open file: %s\n", fileName);

    int *myArray = createOneDimensionalArray(5); //read the output of the 1D array function

    FILE *reader = fopen(fileName, "r");
    if (reader == NULL) {
        perror("Error opening file.\n");
        return 1; 
    }

    if (fscanf(reader, "%d %d", &rows, &cols) != 2) { // reads the dimensions for the first (default) matrix
        printf("Error reading matrix size from file: %s\n", fileName);
        fclose(reader);
        return 1;
    }

    int readInRows;
    int readInCols;

    if (fscanf(reader, "%d %d", &readInRows, &readInCols) != 2) { // reads the dimensions for the second (readIn) matrix
        printf("Error reading matrix size from file: %s\n", fileName);
        fclose(reader);
        return 1;
    }

    printf("Matrix of size (%d, %d) is found\n", readInRows, readInCols); // print the found size of the matrix

    int **defaultMatrix = createMatrix(rows, cols); // make matrix size of (5, 5)
    if (defaultMatrix == NULL) {

        printf("Memory allocation failed for 'defaultMatrix'\n");
        fclose(reader);

        return 1;
    }

    printf("'defaultMatrix' Values:\n");

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {

            printf("%d ", defaultMatrix[i][j]);
        }
        printf("\n");
    }

    int **readInMatrix = createMatrix(readInRows, readInCols); // read the matrix that is given in the file (readInMatrix)

    if (readInMatrix == NULL) {

        printf("Memory allocation failed for 'readInMatrix'\n");
        freeMatrix(defaultMatrix, rows); // free defaultMatrix before exiting. (so it doesnt error)
        fclose(reader);

        return 1;
    }
   
   printf("'readInMatrix' Values:\n");

    for (int i = 0; i < readInRows; i++) {
        for (int j = 0; j < readInCols; j++) {

            if (fscanf(reader, "%d", &readInMatrix[i][j]) != 1) {

                printf("Error reading matrix values at [%d][%d]\n", i, j);

                freeMatrix(defaultMatrix, rows); // clean up before exitting.
                freeMatrix(readInMatrix, readInRows);
                fclose(reader);

                return 1;
            }
            printf("%d ", readInMatrix[i][j]);
        }
        printf("\n");
    }

    fclose(reader);


   int **summedMatrix = addMatrices(defaultMatrix, readInMatrix, rows, cols);
    if (summedMatrix != NULL) {
        printf("Summed Matrix Values:\n");
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                printf("%d ", summedMatrix[i][j]);
            }
            printf("\n");
        }

        computeDiagonalSum(summedMatrix, rows, cols); //call the other functions
        collectBytes(summedMatrix, rows, cols);

    } if (rows != cols){

        printf("Matrix is not square. Cannot compute diagonal sum\n");
        printf("Matrices were not able to be added together\n");
    } 

    freeMatrix(defaultMatrix, rows);
    freeMatrix(readInMatrix, readInRows);
    freeMatrix(summedMatrix, rows);

    free(myArray);

    return 0; 
}


int **addMatrices(int **defaultMatrix, int **readInMatrix, int rows, int cols) {
    
    if (defaultMatrix == NULL || readInMatrix == NULL) { //check to see if the matrices are equal size

        printf("Matrices are not of the same size. Cannot add\n");
        return NULL;
    }

    
    int **matrix = createMatrix(rows, cols);

    if (matrix == NULL) {
        return NULL;
    }

    for (int i = 0; i < rows; i++) {

        for (int j = 0; j < cols; j++) {

            matrix[i][j] = defaultMatrix[i][j] + readInMatrix[i][j];

            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
  
    return matrix; // return the summed matrix
}

int computeDiagonalSum(int **matrix, int rows, int cols) {
    
    if (rows != cols) {

        printf("Matrices were not able to be added together\n");

    } else {

        int diagonalSum = 0;

        for (int i = 0; i < rows; i++) {

            diagonalSum += matrix[i][i];
        }

        printf("Diagonal Sum is: %d\n", diagonalSum);
    }

    return 5; // Difficulty level
}

       
int **collectBytes(int **matrix, int rows, int cols){

    int totalBytes = rows * cols * sizeof(int);  

    printf("Total bytes of matrix: %d\n", totalBytes);
    
    return 0;
}

/*int functionCaller(int argc, char **argv){
    
    char filePath[256];
    char *fileName = filePath;
    snprintf(filePath, sizeof(filePath), "%s", argv[1]);

    int rows = 0;  
    int cols = 0;
    

    // Read the matrix from the file

    int result = readFile(fileName);

        if (result != 3){
            printf("Failed to read the matrix.\n");
            return 1;
        }

    // Check if matrix is square and

    int **defaultMatrix = createMatrix(rows, cols);
    int **readInMatrix = createMatrix(rows, cols);
    
    if(computeDiagonalSum(defaultMatrix, rows, cols) != 0){

        printf("Matrices were not able to be added together.\n");
    }
    for (int i = 0; i < rows; i++){
        free(defaultMatrix[i]);
        free(readInMatrix[i]);
    }
    free(defaultMatrix);
    free(readInMatrix);
    
    return 0;
*/

int main(int argc, char **argv){

    char *fileName = argv[1];
    readFile(fileName);
    
    return 0;
}
