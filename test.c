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


bool in(char tag[], char end, char c, int* index) {
  if (*index == strlen(tag)) {
    if (c == end) {
      *index = 0;
      //printf("\n");
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

  int startFillIndex = 0;
  int endFillIndex = 0;
  char start[26];
  char end[26];

  struct duration *durations = NULL;
  
  while((c = fgetc(fptr)) != EOF) {
    if (inNode(TYPE, c, &nodeIndex)) {
      bool inStart = inTag(START, c, &startIndex);
      bool inEnd = inTag(END, c, &endIndex);

      if (startFillIndex == 25 && endFillIndex == 25) {
        startFillIndex = 0;
        endFillIndex = 0;

        // TODO filter nodes
        bool dateNotInRange = false;

        if (dateNotInRange) {
          continue;
        }

        start[25] = '\0';
        end[25] = '\0';

        struct duration *dur;

        HASH_FIND_STR(durations, start, dur);

        int difference = date(end) - date(start);

        //printf(start);

        if (dur) {
          dur->minutes += difference;
          HASH_ADD_STR(durations, date, dur);
        } else {
          dur = (struct duration *)malloc(sizeof *dur);
          strcpy(dur->date, start);
          dur->minutes = difference;
          HASH_ADD_STR(durations, date, dur);
        }
      }

      if (inStart) {
        //printf("%c", c);
        start[startFillIndex] = c;
        startFillIndex += 1;
      }

      if (inEnd) {
        //printf("%c", c);
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
