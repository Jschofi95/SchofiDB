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

    char tmpStr[100];
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
    int numVal = atoi(strVal);

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
                strcat(retStr, ": ");          // Add ": "

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

// If line1 > line2 return 0 else return 1
int compare(char *line1, char *line2, char fieldName)
{
    const char *ptr1 = strchr(line1, fieldName); // Get pointer to first occurence of fieldname
    int index1 = -1;                             // Stores index of first occurence of a field name
    if (ptr1)
    {
        index1 = (ptr1 - line1) + 3; // The field name is present, get the index
    }

    const char *ptr2 = strchr(line2, fieldName); // Get pointer to first occurence of fieldname
    int index2 = -1;                             // Stores index of first occurence of a field name
    if (ptr2)
    {
        index2 = (ptr2 - line2) + 3; // The field name is present, get the index
    }

    char strVal[100] = "";
    for (int i = 0; i < strlen(line1); i++)
    {
        if (isdigit(line1[index1 + i]))
        {
            strVal[i] = line1[index1 + i];
        }
        else
        {
            break;
        }
    }
    int numVal1 = atoi(strVal);

    strcpy(strVal, ""); // Reset strVal
    for (int i = 0; i < strlen(line2); i++)
    {
        if (isdigit(line2[index2 + i]))
        {
            strVal[i] = line2[index2 + i];
        }
        else
        {
            break;
        }
    }
    int numVal2 = atoi(strVal);

    if (numVal1 < numVal2)
    {
        return 1;
    }
    else if (numVal1 > numVal2)
    {
        return 0;
    }

    return -1;
}

line *sort(char *field, line *lines, int linesSize)
{
    int sortOrder, i, temp, swapped;
    char fieldName = field[0];
    line *tmpLines = malloc(sizeof(line) * linesSize); // Used to store lines where the field is present
    int tmpSize = 0;

    char tmpStr[100];
    strncpy(tmpStr, &field[4], strlen(field) - 3); // Used to store number for sortOrder
    sortOrder = atoi(tmpStr);

    for (int i = 0; i < linesSize; i++) // Build new list only with lines that contain the field name
    {
        char *pPosition = strchr(lines[i].data, fieldName);
        if (pPosition != NULL) // If fieldName exists in line
        {
            strcpy(tmpLines[i].data, lines[i].data);
            tmpLines[i]._id = lines[i]._id;
            tmpSize++;
        }
    }

    if (fieldName != 'A')
    {
        if (sortOrder == 1) // Ascending
        {
            while (1)
            {
                swapped = 0;
                for (i = 0; i < tmpSize - 1; i++)
                {
                    if (compare(tmpLines[i].data, tmpLines[i + 1].data, fieldName) == 0)
                    {
                        line x; // Used to temporarily store data for swap
                        strcpy(x.data, tmpLines[i + 1].data);
                        x._id = tmpLines[i + 1]._id;

                        strcpy(tmpLines[i + 1].data, tmpLines[i].data);
                        tmpLines[i + 1]._id = tmpLines[i]._id;

                        strcpy(tmpLines[i].data, x.data);
                        tmpLines[i]._id = x._id;
                        swapped = 1;
                    }
                }

                if (swapped == 0)
                {
                    printf("break\n");
                    break;
                }
            }
        }

        if (sortOrder == -1) // Descending
        {
            while (1)
            {
                swapped = 0;
                for (i = 0; i < tmpSize - 1; i++)
                {
                    if (compare(tmpLines[i].data, tmpLines[i + 1].data, fieldName) == 1)
                    {
                        line x; // Used to temporarily store data for swap
                        strcpy(x.data, tmpLines[i + 1].data);
                        x._id = tmpLines[i + 1]._id;

                        strcpy(tmpLines[i + 1].data, tmpLines[i].data);
                        tmpLines[i + 1]._id = tmpLines[i]._id;

                        strcpy(tmpLines[i].data, x.data);
                        tmpLines[i]._id = x._id;
                        swapped = 1;
                    }
                }

                if (swapped == 0)
                {
                    break;
                }
            }
        }
    }
    if (fieldName == 'A')
    {
        if (sortOrder == 1)
        {
            while (1)
            {
                for (i = 0; i < linesSize - 1; i++)
                {
                    if (lines[i]._id > lines[i + 1]._id)
                    {
                        line x; // Used to temporarily store data for swap
                        strcpy(x.data, lines[i + 1].data);
                        x._id = lines[i + 1]._id;

                        strcpy(lines[i + 1].data, lines[i].data);
                        lines[i + 1]._id = lines[i]._id;

                        strcpy(lines[i].data, x.data);
                        lines[i]._id = x._id;
                        swapped = 1;
                    }
                }
                if (swapped == 0)
                {
                    break;
                }
            }
        }
        if (sortOrder == -1)
        {
            while (1)
            {
                for (i = 0; i < linesSize - 1; i++)
                {
                    if (lines[i]._id < lines[i + 1]._id)
                    {
                        line x; // Used to temporarily store data for swap
                        strcpy(x.data, lines[i + 1].data);
                        x._id = lines[i + 1]._id;

                        strcpy(lines[i + 1].data, lines[i].data);
                        lines[i + 1]._id = lines[i]._id;

                        strcpy(lines[i].data, x.data);
                        lines[i]._id = x._id;
                        swapped = 1;
                    }
                }
                if (swapped == 0)
                {
                    break;
                }
            }
        }
    }

    if (fieldName == 'A')
    {
        return lines;
    }

    return tmpLines;
}

int main()
{
    char *fileName = "data.txt";
    int maxSize = 10;                             // Maximum allocation for array
    int currSize = 0;                             // How many elements are in array at a given time
    line *lines = malloc(sizeof(line) * maxSize); // Used to store
    char tmp[maxLen];                             // Temporary string used to hold input strings
    char operation[maxLen] = "";
    int securityLevel = -1;
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
        lines[index]._id = index;          // Set _id for lines[i]
        currSize++;
        index++;
    }

    int linesSize = currSize; // Save size of lines array
    int count = 1;
    while (1)
    {
        line *tmpLine = malloc(sizeof(line) * linesSize); // Used to save the original lines list
        tmpLine = lines;

        // Reset maxSize and currSize
        maxSize = 10;
        currSize = 0;

        sc = malloc(sizeof(searchConditions) * maxSize); // Reset search conditions
        strcpy(projections, "");                         // Reset projections
        strcpy(tmp, "");                                 // Reset
        securityLevel = -1;

        fgets(tmp, maxLen, stdin);   // Read find/sort operation
        tmp[strlen(tmp) - 1] = '\0'; // Remove newline character

        strncpy(operation, tmp, 4);          // Store type of operation Find/Sort
        strcpy(operation, Lower(operation)); // Convert the operation type (find/sort) to lower case for comparison
        if (strlen(tmp) > 4)                 // If tmp length > 4, then a security level was included. Read the security level
            securityLevel = tmp[strlen(tmp) - 1] - '0';

        while (fgets(tmp, maxLen, stdin)) // Read search conditions and projections
        {
            tmp[strlen(tmp) - 1] = '\0'; // Remove newline character
            if (strchr(tmp, ';') != NULL)
            {
                tmp[strlen(tmp) - 1] = '\0';          // Remove ';' from end of string
                if ((strcmp(operation, "sort") == 0)) // If sort is selected, then there are no projections, add to sc[0]
                {
                    strcpy(sc[0].cond, tmp);
                }
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

        printf("//Query %d\n", count);

        if (strcmp(operation, "find") == 0)
        {
            line *newList = lines;

            if (strcmp(sc[0].cond, "Z") != 0) // If search conditions were given, perform find function, otherwise, keep original list
            {
                newList = matchResults(sc, lines, securityLevel, currSize, linesSize);
                remove_spaces(projections); // Remove spaces for traversal purposes
                // Lower(projections); // Convert string to lower case for input simplicity (No output when this is active for some reason)

                if (projections[0] == 'X' || projections[0] == 'x') // If print all is passed (I.E the letter X and only the letter X)
                {
                    for (int i = 0; i < matchedListIndex; i++)
                    {
                        char result[250] = "";
                        strcpy(result, "A: ");
                        char buffer[100] = "";
                        sprintf(buffer, "%d", newList[i]._id); // Convert _id to a string
                        strcat(result, buffer);
                        strcat(result, " ");
                        strcat(result, newList[i].data);
                        printf("%s\n", result);
                    }
                }
                else // Print selected fields
                {
                    char result[100] = "";
                    for (int i = 0; i < matchedListIndex; i++)
                    {
                        char *pPosition = strchr(projections, 'A'); // Find if 'A' exists in projections
                        if (pPosition == NULL)
                        {
                            project(projections, newList[i].data, result);
                            printf("%s\n", result);
                        }
                        else
                        {
                            // Add formatting for _id field in the result
                            strcpy(result, "A: ");
                            char buffer[100] = "";
                            sprintf(buffer, "%d", newList[i]._id); // Convert _id to a string
                            strcat(result, buffer);
                            strcat(result, " ");

                            char tmpStr[100] = "";
                            strcpy(tmpStr, result);

                            project(projections, newList[i].data, result);
                            strcat(tmpStr, result);
                            printf("%s\n", tmpStr);
                        }
                    }
                }
            }
            else // If no conditions were given
            {
                remove_spaces(projections); // Remove spaces for traversal purposes
                // Lower(projections); // Convert string to lower case for input simplicity (No output when this is active for some reason)

                if (projections[0] == 'X' || projections[0] == 'x') // If print all is passed (I.E the letter X and only the letter X)
                {
                    for (int i = 0; i < linesSize; i++)
                    {
                        char result[250] = "";
                        strcpy(result, "A: ");
                        char buffer[100] = "";
                        sprintf(buffer, "%d", lines[i]._id); // Convert _id to a string
                        strcat(result, buffer);
                        strcat(result, " ");
                        strcat(result, lines[i].data);
                        printf("%s\n", result);
                    }
                }
                else // Print selected fields
                {
                    char result[100] = "";
                    for (int i = 0; i < linesSize; i++)
                    {
                        char *pPosition = strchr(projections, 'A'); // Find if 'A' exists in projections
                        if (pPosition == NULL)
                        {
                            project(projections, lines[i].data, result);
                            if (strlen(result) == 0)
                            {
                                continue;
                            }
                            printf("%s\n", result);
                        }
                        else
                        {
                            // Add formatting for _id field in the result
                            strcpy(result, "A: ");
                            char buffer[100] = "";
                            sprintf(buffer, "%d", lines[i]._id); // Convert _id to a string
                            strcat(result, buffer);
                            strcat(result, " ");

                            char tmpStr[100] = "";
                            strcpy(tmpStr, result);

                            project(projections, lines[i].data, result);
                            if (strlen(result) == 0)
                            {
                                continue;
                            }
                            strcat(tmpStr, result);
                            printf("%s\n", tmpStr);
                        }
                    }
                }
            }
        }
        else if (strcmp(operation, "sort") == 0)
        {
            sc[0].cond[strlen(sc[0].cond)] = '\0';

            printf("//Query %d\n", count);
            if (sc[0].cond[0] == 'A')
            {
                printf("Not working\n");
                continue;
            }


            line *newList = malloc(sizeof(line) * 10000);
            newList = sort(sc[0].cond, lines, linesSize);

            for (int i = 0; i < linesSize; i++)
            {
                if (strlen(newList[i].data) == 0)
                {
                    continue;
                }
                printf("%s\n", newList[i].data);
            }
        }
        else
        {
            printf("Error - No such operation\n");
        }

        // printf("PRINTING LINES\n");
        // for(int i = 0; i < linesSize; i++){
        //     printf("%d: %s\n",lines[i]._id, lines[i].data);
        // }

        printf("\n");
        lines = tmpLine;
        count++;
    }

    fclose(file);

    return 0;
}