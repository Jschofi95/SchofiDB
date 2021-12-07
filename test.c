#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct searchConditions {
   char cond[10];
} searchConditions;

int main() {
    
   char str[100];
   int i;
   char tmp[100]; // Temporary string used to hold input strings
   char operation[50] = "";
   int securityLevel;
   bool isFinished = false;
   searchConditions *sc = malloc(sizeof(searchConditions) * 10); // Used to store search conditions
   char projections[100] = ""; // Used to store fields to project

   printf( "Start:\n");
   
   fgets(tmp, 100, stdin); // Read find/sort operation
   tmp[strlen(tmp) - 1] = '\0'; // Remove newline character

   printf("TEMP: %s\n", tmp);

   strncpy(operation, tmp, 4); // Store type of operation Find/Sort
   if(strlen(tmp) > 4) // If tmp length > 4, then a security level was included. Read the security level
      securityLevel = tmp[strlen(tmp) - 1] - '0';

   // while (isFinished == false) {
      
   // }

   printf("strlen(tmp): %d\n", strlen(tmp));


   if(strlen(tmp) == 4) {
      printf("Operation: %s\n", operation);
   } else {
      printf("Operation: %s    Security: %d\n", operation, securityLevel);
   }

   return 0;
}