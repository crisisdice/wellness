#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define PATH "data/test.txt"
#define TYPE "HKQuantityTypeIdentifierStepCount"
#define START "startDate=\""
#define END "endDate=\""

void ptTst(char* c);

int main() {
  char f = 'f';
  ptTst(&f);
  printf("%c", f);
  return parse();
}

int parse() {
  char c;
  FILE *fptr;

  int nodeLength = strlen(TYPE);
  int nodeIndex = 0;

  int startLength = strlen(START);
  int startIndex = 0;

  int endLength = strlen(END);
  int endIndex = 0;
  
  if ((fptr = fopen(PATH, "r")) == NULL) {
    printf("Error opening file");
    exit(1);
  }
  
  while((c = fgetc(fptr)) != EOF) {
    if (nodeIndex == (nodeLength - 1)) {
      if (c == '\n') {
        nodeIndex = 0;
        continue;
      }

      if (startIndex == startLength) {
        if (c == '"') {
          startIndex = 0;
          printf("\n");
          continue;
        }
        printf("%c", c);
        continue;
      }

      if (c == START[startIndex]) {
        startIndex += 1;
        continue;
      }

      startIndex = 0;
      continue;
    }

    if (c == TYPE[nodeIndex]) {
      nodeIndex += 1;
      continue;
    }
    nodeIndex = 0;
  }

  fclose(fptr);

  return 0;
}

bool in(char tag[], char end, char c, int* index) {
  if (c == end) {
    return false;
  }
  bool inTag = c == tag[*index];
  if (inTag) {
    *index += 1;
  }
  return inTag;
}

bool inNode(char tag[], char c, int* index) {
  return in (tag, '\n', c, index);
}

bool inTag(char tag[], char c, int* index) {
  return in (tag, '\"', c, index);
}

