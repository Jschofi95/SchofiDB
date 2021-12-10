#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#define maxLen 100

int matchedListIndex = 0;

typedef struct searchConditions
{ // Used to store search conditions from input
    char cond[maxLen];
} searchConditions;

typedef struct line
{
    char data[500];
    int _id;
} line;

char *Lower(char *s)
{
    for (int i = 0; i < strlen(s); i++)
    {
        s[i] = tolower(s[i]);
    }
    return s;
}

// Compare one condition to one line, return true if condition is a match
bool isMatch(char *searchCondition, char *line)
{
    char fieldName = searchCondition[0];
    char comparisonOp = searchCondition[2];

    char tmpStr[100];                                                  // C < 12
    strncpy(tmpStr, &searchCondition[4], strlen(searchCondition) - 3); // Used to store sub string for value
    int value = atoi(tmpStr);

    const char *ptr = strchr(line, fieldName); // Get pointer to first occurence of fieldname
    int index = -1;                            // Stores index of first occurence of a field name
    if (ptr)
    {
        index = (ptr - line) + 3; // The field name is present, get the index
    }
    else
    {
        // printf("No val\n"); // If the field name isnt present, then return
        //   return false
        return false;
    }

    // Get field value from line
    char strVal[100] = "";
    for (int i = 0; i < strlen(line); i++)
    {
        if (isdigit(line[index + i]))
        {
            strVal[i] = line[index + i];
        }
        else
        {
            break;
        }
    }
    // printf("strVal: %s\n", strVal);
    int numVal = atoi(strVal);
    // printf("numVal: %d, strVal length: %d\n", numVal, strlen(strVal));

    switch (comparisonOp)
    {
    case '>':
        if (numVal > value)
            return true;
        break;
    case '<':
        if (numVal < value)
            return true;
        break;
    case '=':
        if (numVal == value)
            return true;
        break;
    }

    return false;
}

void remove_spaces(char *s)
{
    char *d = s;
    do
    {
        while (*d == ' ')
        {
            ++d;
        }
    } while (*s++ = *d++);
}

void *project(char *p, char *lines, char *s)
{
   char retStr[maxLen] = "";
   for (int i = 0; i < strlen(p); i++)
   {
      for (int j = 0; j < strlen(lines); j++)
      {
         if (lines[j] == p[i])
         {
            strncat(retStr, &lines[j], 1); // Add field name
            strcat(retStr, ": "); // Add ": "

            char strVal[100] = "";
            for (int k = 0; k < strlen(lines); k++)
            {
               if (isdigit(lines[j + 3 + k]))
               {
                  strVal[k] = lines[j + 3 + k];
               }
            }
            strcat(retStr, strVal);
            strcat(retStr, " ");
         }
      }
   }
   strcpy(s, retStr);
}

line *matchResults(searchConditions *sc, line *lines, int classification, size_t scSize, size_t linesSize)
{
    line *list = malloc(sizeof(line) * 10000);
    int index = 0;

    // "B: 390 Y: 1 C: 5 D: 1 F: 127700"
    // "B > 350"
    // "C < 12"

    // for(int i = 0; i < linesSize; i++){
    //    printf("%s\n", lines[i].data);
    // }

    for (int i = 0; i < linesSize; i++) // Scan through each line
    {
        bool flag = true;
        for (int j = 0; j < scSize; j++) // Scan through each search condition
        {
            if (classification == -1) // If no security level is provided
            {
                if (isMatch(sc[j].cond, lines[i].data) == false)
                {
                    flag = false;
                }
            }
            else // If security level is provided
            {
                // Build security classification string (Format: Y < 3)
                char security[10] = "Y < ";
                char buffer[10];
                snprintf(buffer, 10, "%d", classification + 1);
                strncat(security, buffer, 10);

                // printf("security: %s\n", security);

                if ((isMatch(sc[j].cond, lines[i].data) == false))
                {
                    flag = false;
                }
                else
                {
                    if (isMatch(security, lines[i].data) == false)
                    {
                        flag = false;
                    }
                }
            }
        }
        if (flag == true) // If all search conditions were true, then add the line to list
        {
            strcpy(list[index].data, lines[i].data);
            list[index]._id = lines[i]._id;
            index++;
        }
    }

    matchedListIndex = index;

    return list;
}

int main()
{
    char *fileName = "data.txt";
    int maxSize = 10; // Maximum allocation for array
    int currSize = 0; // How many elements are in array at a given time
    line *lines = malloc(sizeof(line) * maxSize);
    char tmp[maxLen]; // Temporary string used to hold input strings
    char operation[maxLen] = "";
    int securityLevel = -1;
    bool isFinished = false;
    searchConditions *sc = malloc(sizeof(searchConditions) * maxSize); // Used to store search conditions
    char projections[maxLen] = "";                                     // Used to store fields to project

    strcpy(sc[0].cond, "NULL"); // Assign to null to check if search conditions are passed or not

    FILE *file = fopen(fileName, "r");
    if (file == NULL)
    {
        printf("Error reading file, make sure the name is correct");
        return 1;
    }

    char tmpStr[500];                // Temporarily stores string for fgets
    int index = 0;                   // Index for accessing array elements
    while (fgets(tmpStr, 500, file)) // Read contents of file into array
    {
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
        tmpStr[strlen(tmpStr) - 1] = '\0'; // Remove newline character from string
        strcpy(lines[index].data, tmpStr); // Copy string buffer to lines[i].data
        lines[index]._id = index + 1;      // Set _id for lines[i]
        currSize++;
        index++;
    }

    int linesSize = currSize; // Save size of lines array

    // Reset maxSize and currSize
    maxSize = 10;
    currSize = 0;

    fgets(tmp, maxLen, stdin);   // Read find/sort operation
    tmp[strlen(tmp) - 1] = '\0'; // Remove newline character

    strncpy(operation, tmp, 4); // Store type of operation Find/Sort
    if (strlen(tmp) > 4)        // If tmp length > 4, then a security level was included. Read the security level
        securityLevel = tmp[strlen(tmp) - 1] - '0';

    while (fgets(tmp, maxLen, stdin)) // Read search conditions and projections
    {
        tmp[strlen(tmp) - 1] = '\0'; // Remove newline character
        if (strchr(tmp, ';') != NULL)
        {
            tmp[strlen(tmp) - 1] = '\0'; // Remove ';' from end of string
            strcpy(projections, tmp);
            break;
        }
        if (currSize >= maxSize)
        { // If sc is filled, double its size
            searchConditions *tmpSC = malloc(sizeof(searchConditions) * maxSize);
            maxSize *= 2;
            for (int i = 0; i < currSize; i++)
            {
                strcpy(tmpSC[i].cond, sc[i].cond);
            }

            sc = realloc(sc, sizeof(searchConditions) * maxSize);

            for (int i = 0; i < currSize; i++)
            {
                strcpy(sc[i].cond, tmpSC[i].cond);
            }
        }
        strcpy(sc[currSize].cond, tmp);
        currSize++;
    }

    // Print contents of lines
    // for(int i = 0; i < linesSize; i++) {
    //     printf("%d:  %s\n", lines[i]._id, lines[i].data);
    // }

    // Print contents of sc
    // for (int i = 0; i < currSize; i++)
    // {
    //     printf("Cond %d: %s\n", i + 1, sc[i].cond);
    // }
    // printf("Projections: %s\n", projections);
    // printf("currSize: %d\n", currSize);

    strcpy(operation, Lower(operation)); // Convert the operation type (find/sort) to lower case for comparison in following if statement
    if (strcmp(operation, "find") == 0)
    {
        line *newList = lines;

        if (strcmp(sc[0].cond, "NULL") != 0) // If search conditions were given, perform find function, otherwise, keep original list
        {
            newList = matchResults(sc, lines, securityLevel, currSize, linesSize);
            remove_spaces(projections);

            char result[100] = "";
            for(int i = 0; i < matchedListIndex; i++) {
                project(projections, newList[i].data, result);
                printf("%s\n", result);
            }
        }

        // Print newList
        // for (int i = 0; i < matchedListIndex; i++)
        // {
        //     printf("%d:  %s\n", newList[i]._id, newList[i].data);
        // }
    }

    fclose(file);

    return 0;
}