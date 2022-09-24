#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include <uthash.h>

#define PATH "data/test.txt"
#define TYPE "HKQuantityTypeIdentifierStepCount"
// TODO refactor \" to be in inTag()
#define START "startDate=\""
#define END "endDate=\""
#define CUTOFF ""

struct duration {
  char date[25];
  int minutes;
  UT_hash_handle hh;
};

void test(struct duration *durations);

int date(char timestamp[]) {
  struct tm tm;
  memset(&tm, 0, sizeof(tm));
  strptime(timestamp, "%Y-%m-%d %H:%M:%S", &tm);
  return (tm.tm_hour * 60) + tm.tm_min;
}

bool dateIsBefore(char test[], char cutoff[]) {
  // TODO
  return false;
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
        if (dateIsBefore(start, CUTOFF)) {
          continue;
        }

        struct duration *dur;
        HASH_FIND_STR(durations, start, dur);

        if (dur == NULL) {
          dur = (struct duration *)malloc(sizeof *dur);
          strcpy(dur->date, start);
        }

        dur->minutes += (date(end) - date(start));
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

  HASH_FIND_STR(durations, "2018-08-06 17:19:12 +0200", dur);

  if (dur == NULL) {
    printf("whoops");
  }

  printf("%d\n", dur->minutes);
}
