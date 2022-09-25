#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include <uthash.h>

#define PATH "data/co.xml"
#define TYPE "HKQuantityTypeIdentifierStepCount"
// TODO refactor \" to be in inTag()
#define START "startDate=\""
#define END "endDate=\""
#define CUTOFF "2022-09-01"

struct duration {
  char date[11];
  int minutes;
  UT_hash_handle hh;
};

void test(struct duration *durations);

int date(char timestamp[]) {
  struct tm tm;
  memset(&tm, 0, sizeof(tm));
  strptime(timestamp, "%Y-%m-%d %H:%M:%S", &tm);
  int temp =  (tm.tm_hour * 60) + tm.tm_min;
  /* printf("%d\n", temp); */
  return temp;
}

//int dateDiffInMin(char first[], char second[]) {}

bool dateIsBefore(char test[], char cutoff[]) {
  //return false;
  printf(test);
  printf(" - ");
  printf(cutoff);
  printf("\n");
  struct tm first, second;

  memset(&first, 0, sizeof(first));
  memset(&second, 0, sizeof(second));
  strptime(test, "%Y-%m-%d", &first);
  strptime(cutoff, "%Y-%m-%d", &second);

  time_t first_converted, second_converted;

  first_converted = mktime(&first);
  second_converted = mktime(&second);

  double seconds = difftime(first_converted, second_converted);
  /* printf("%f\n", seconds); */

  bool isOver = seconds <= 0;

  printf(isOver ? "true\n" : "false\n");

  return seconds <= 0;
}

bool in(char tag[], char end, char c, int* index) {
  if (*index == strlen(tag)) {
    if (c == end) {
      *index = 0;
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
  struct duration *durations = NULL;

  // indices for chars of tags
  int nodeIndex = 0;
  int startIndex = 0;
  int endIndex = 0;
  
  // indices for chars of tag contents
  int startFillIndex = 0;
  int endFillIndex = 0;

  // strs for start and end date of node
  char start[26];
  char end[26];

  // null terminated strings
  start[25] = end[25] = '\0';

  if ((fptr = fopen(PATH, "r")) == NULL) {
    printf("Error opening file");
    exit(1);
  }
  
  while((c = fgetc(fptr)) != EOF) {
    if (inNode(TYPE, c, &nodeIndex)) {
      bool inStart = inTag(START, c, &startIndex);
      bool inEnd = inTag(END, c, &endIndex);

      // dates filled so check if in range and write difference to map
      if (startFillIndex == 25 && endFillIndex == 25) {
        startFillIndex = endFillIndex = 0;

        char dest[11];
        memset(dest, '\0', sizeof(dest));
        strncpy(dest, start, 10);
        
        if (dateIsBefore(dest, CUTOFF)) {
          continue;
        }

        struct duration *dur;
        HASH_FIND_STR(durations, dest, dur);

        if (dur == NULL) {
          dur = (struct duration *)malloc(sizeof *dur);
          strcpy(dur->date, dest);
          dur->minutes = 0;
        }

        /* printf(start); */
        /* printf("\n"); */

        int temp = (date(end) - date(start));
        /* printf("%d\n", temp); */

        dur->minutes +=  temp;
        /*  */
        /* printf("%d\n", dur->minutes); */

        HASH_ADD_STR(durations, date, dur);
      }

      if (inStart) {
        start[startFillIndex] = c;
        startFillIndex += 1;
      }

      if (inEnd) {
        end[endFillIndex] = c;
        endFillIndex += 1;
      }
    }
  }
  test(durations);
  fclose(fptr);
  return 0;
}

// TODO remove
void test(struct duration *durations) {
  struct duration *dur;

  HASH_FIND_STR(durations, "2022-09-01", dur);

  if (dur == NULL) {
    printf("Key not found");
    return;
  }

  printf("%d\n", dur->minutes);
}

