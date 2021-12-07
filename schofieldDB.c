#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define maxLen 100

typedef struct searchConditions
{ // Used to store search conditions from input
    char cond[maxLen];
} searchConditions;

typedef struct line
{
    char data[500];
    int _id;
} line;

int main()
{
    char *fileName = "data.txt";
    int maxSize = 10; // Maximum allocation for array
    int currSize = 0; // How many elements are in array at a given time
    line *lines = malloc(sizeof(line) * maxSize);
    char tmp[maxLen]; // Temporary string used to hold input strings
    char operation[maxLen] = "";
    int securityLevel;
    bool isFinished = false;
    searchConditions *sc = malloc(sizeof(searchConditions) * maxSize); // Used to store search conditions
    char projections[maxLen] = ""; // Used to store fields to project

    FILE *file = fopen(fileName, "r");
    if (file == NULL)
    {
        printf("Error reading file, make sure the name is correct");
        return 1;
    }

    char tmpStr[500]; // Temporarily stores string for fgets
    int index = 0;    // Index for accessing array elements
    while (fgets(tmpStr, 500, file))
    { // Read contents of file into array
        if (currSize >= maxSize)
        {
            line *tmp = malloc(sizeof(line) * maxSize);
            maxSize *= 2; // If array capacity is reached, double the maximum capacity

            for (int z = 0; z < currSize; z++)
            { // Copy content of lines to tmp for reallocation
                strcpy(tmp[z].data, lines[z].data);
                tmp[z]._id = lines[z]._id;
            }

            lines = realloc(lines, sizeof(line) * maxSize); // Reallocate memory that is twice the size of its previous size

            for (int z = 0; z < currSize; z++)
            { // Copy contents of tmp to lines
                strcpy(lines[z].data, tmp[z].data);
                lines[z]._id = tmp[z]._id;
            }
        }

        strcpy(lines[index].data, tmpStr); // Copy string buffer to lines[i].data
        lines[index]._id = index + 1;      // Set _id for lines[i]
        currSize++;
        index++;
    }

    // for(int i = 0; i < currSize; i++) { // Print contents of lines
    //     printf("%d:  %s", lines[i]._id, lines[i].data);
    // }

    // Reset maxSize and currSize
    maxSize = 10;
    currSize = 0;

    fgets(tmp, maxLen, stdin);   // Read find/sort operation
    tmp[strlen(tmp) - 1] = '\0'; // Remove newline character

    strncpy(operation, tmp, 4); // Store type of operation Find/Sort
    if (strlen(tmp) > 4)        // If tmp length > 4, then a security level was included. Read the security level
        securityLevel = tmp[strlen(tmp) - 1] - '0';

    while(fgets(tmp, maxLen, stdin)) { // Read search conditions and projections
      tmp[strlen(tmp) - 1] = '\0'; // Remove newline character
      if(strchr(tmp, ';') != NULL) {
         tmp[strlen(tmp) - 1] = '\0'; // Remove ';' from end of string
         strcpy(projections, tmp);
         break;
      }
      if(currSize >= maxSize) { // If sc is filled, double its size
         searchConditions *tmpSC = malloc(sizeof(searchConditions) * maxSize);
         maxSize *= 2;
         for(int i = 0; i < currSize; i++) {
            strcpy(tmpSC[i].cond, sc[i].cond);
         }

         sc = realloc(sc, sizeof(searchConditions) * maxSize);

         for(int i = 0; i < currSize; i++) {
            strcpy(sc[i].cond, tmpSC[i].cond);
         }
      }
      strcpy(sc[currSize].cond, tmp);
      currSize++;
   }

    // Print contents of sc
    // for(int i = 0; i < currSize; i++) {
    //     printf("Cond %d: %s\n", i + 1, sc[i].cond);
    // }
    // printf("Projections: %s", projections);

    fclose(file);

    return 0;
}