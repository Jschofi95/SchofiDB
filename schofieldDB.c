#include <stdio.h>
#include<stdlib.h>
#include <string.h>

// Test

typedef struct line {
    char data[500];
    int _id;
} line;

int main() {
    char *fileName = "data.txt";
    int maxSize = 10; // Maximum allocation for array
    int currSize = 0; // How many elements are in array at a given time
    line *lines = malloc(sizeof(line) * maxSize);

    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error reading file, make sure the name is correct");
        return 1;
    }

    char tmpStr[500]; // Temporarily stores string for fgets
    int index = 0; // Index for accessing array elements
    while(fgets(tmpStr, 500, file)) { // Read contents of file into array
        if (currSize >= maxSize) {
            line *tmp = malloc(sizeof(line) * maxSize);
            maxSize *= 2; // If array capacity is reached, double the maximum capacity

            for(int z = 0; z < currSize; z++) { // Copy content of lines to tmp for reallocation
                strcpy(tmp[z].data, lines[z].data);
                tmp[z]._id = lines[z]._id;
            }

            lines = realloc(lines, sizeof(line) * maxSize); // Reallocate memory that is twice the size of its previous size

            for(int z = 0; z < currSize; z++) { // Copy contents of tmp to lines
                strcpy(lines[z].data, tmp[z].data);
                lines[z]._id = tmp[z]._id;
            }
        }

        strcpy(lines[index].data, tmpStr); // Copy string buffer to lines[i].data
        lines[index]._id = index + 1; // Set _id for lines[i]
        currSize++;
        index++;
    }

    // for(int i = 0; i < currSize; i++) { // Print contents of lines
    //     printf("%d:  %s", lines[i]._id, lines[i].data);
    // }

    fclose(file);

    return 0;
}