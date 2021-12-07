#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define maxLen 100

typedef struct searchConditions
{
   char cond[maxLen];
} searchConditions;

int main()
{
   int maxSize = 10;
   int currSize = 0;
   searchConditions *sc = malloc(sizeof(searchConditions) * maxSize);
   bool isFinished = false;
   char tmp[maxLen] = "";
   char projections[maxLen];

   while(fgets(tmp, maxLen, stdin)) {
      tmp[strlen(tmp) - 1] = '\0'; // Remove newline character
      if(strchr(tmp, ';') != NULL) {
         tmp[strlen(tmp) - 1] = '\0'; // Remove ';' from end of string
         strcpy(projections, tmp);
         break;
      }
      if(currSize >= maxSize) {
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

   printf("maxSize: %d\n", maxSize);
   for(int i = 0; i < currSize; i++) {
      printf("Cond %d: %s\n", i, sc[i].cond);
   }
   printf("Projections: %s", projections);

   return 0;
}