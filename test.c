#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define PATH "data/test.txt"
#define TYPE "HKQuantityTypeIdentifierStepCount"
// TODO refactor \" to be in inTag()
#define START "startDate=\""
#define END "endDate=\""

bool in(char tag[], char end, char c, int* index) {
  if (*index == strlen(tag)) {
    if (c == end) {
      *index = 0;
      printf("\n");
      return false;
    }
    return true;
  }
  *index = c == tag[*index] ? *index + 1 : 0;
  return false;
}

bool inNode(char tag[], char c, int* index) {
  return in(tag, '\n', c, index);
}

bool inTag(char tag[], char c, int* index) {
  return in(tag, '"', c, index);
}

int main() {
  char c;
  FILE *fptr;

  int nodeIndex = 0;
  int startIndex = 0;
  int endIndex = 0;
  
  if ((fptr = fopen(PATH, "r")) == NULL) {
    printf("Error opening file");
    exit(1);
  }
  
  while((c = fgetc(fptr)) != EOF) {
    if (inNode(TYPE, c, &nodeIndex)) {
      bool inStart = inTag(START, c, &startIndex);
      bool inEnd = inTag(END, c, &endIndex);

      if (inStart) {
        printf("%c", c);
      }
      if (inEnd) {
        printf("%c", c);
      }
    }
  }

  fclose(fptr);
  return 0;
}
