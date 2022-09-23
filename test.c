#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define PATH "data/test.txt"
#define TYPE "HKQuantityTypeIdentifierStepCount"
#define START "startDate=\""
#define END "endDate=\""

int parse();
bool inNode(char tag[], char c, int* index);

int main() {
  return parse();
}

int parse() {
  char c;
  FILE *fptr;

  int nodeIndex = 0;
  int startIndex = 0;
  int endIndex = 0;

  int startLength = strlen(START);
  
  if ((fptr = fopen(PATH, "r")) == NULL) {
    printf("Error opening file");
    exit(1);
  }
  
  while((c = fgetc(fptr)) != EOF) {
    bool inNd = inNode(TYPE, c, &nodeIndex);

    if (inNd) {
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
  }

  fclose(fptr);

  return 0;
}

bool in(char tag[], char end, char c, int* index) {
  int length = strlen(tag);

  if (c == end) {
    *index = 0;
    return false;
  }

  if (*index == length) {
    return true;
  }

  if (c == tag[*index]) {
    *index += 1;
    return false;
  }

  *index = 0;
  return false;
}

bool inNode(char tag[], char c, int* index) {
  return in (tag, '\n', c, index);
}

bool inTag(char tag[], char c, int* index) {
  return in (tag, '\"', c, index);
}

