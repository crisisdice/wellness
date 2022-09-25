#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include <uthash.h>

#define PATH "data/export.xml"
#define TYPE "HKQuantityTypeIdentifierStepCount"
// TODO refactor \" to be in inTag()
#define START "startDate=\""
#define END "endDate=\""
// TODO pass as argument to main
#define CUTOFF "2022-09-01"

/* time */
struct tm toTm(char timestamp[]) {
  struct tm tm;
  memset(&tm, 0, sizeof(tm));
  strptime(timestamp, "%Y-%m-%d %H:%M:%S", &tm);
  return tm;
}

int minutes(char timestamp[]) {
  struct tm tm = toTm(timestamp);
  int temp =  (tm.tm_hour * 60) + tm.tm_min;
  return temp;
}

bool dateIsBefore(char test[], struct tm cutoff) {
  struct tm testTm = toTm(test);
  return difftime(mktime(&testTm), mktime(&cutoff)) <= 0;
}

void justDate(char *date, char timestamp[]) {
  memset(date, '\0', sizeof(date));
  strncpy(date, timestamp, 10);
}

/* xml */
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

/* impl */
struct duration {
  char date[11];
  int minutes;
  UT_hash_handle hh;
};

void test(struct duration *allDurations);

int main() {
  FILE *filePointer;
  char currentChar;

  struct duration *allDurations = NULL;
  struct tm cutoff = toTm(CUTOFF);

  // indices for chars of tags
  int nodeIndex, startIndex, endIndex;
  nodeIndex = startIndex = endIndex = 0;
  
  // indices for chars of start and end tag contents
  int startFillIndex, endFillIndex;
  startFillIndex = endFillIndex = 0;

  // containers for start and end tag contents
  char start[26], end[26];
  start[25] = end[25] = '\0';

  if ((filePointer = fopen(PATH, "r")) == NULL) {
    printf("Error opening file");
    exit(1);
  }
  
  while((currentChar = fgetc(filePointer)) != EOF) {
    if (inNode(TYPE, currentChar, &nodeIndex)) {
      bool inStart = inTag(START, currentChar, &startIndex);
      bool inEnd = inTag(END, currentChar, &endIndex);

      // dates filled so check if in range and write difference to map
      if (startFillIndex == 25 && endFillIndex == 25) {
        startFillIndex = endFillIndex = 0;
        
        if (dateIsBefore(start, cutoff)) {
          continue;
        }

        struct duration *dur;
        char key[11];

        justDate(key, start);
        HASH_FIND_STR(allDurations, key, dur);

        if (dur == NULL) {
          dur = (struct duration *)malloc(sizeof *dur);
          strcpy(dur->date, key);
          dur->minutes = 0;
        }
        dur->minutes += (minutes(end) - minutes(start));
        HASH_ADD_STR(allDurations, date, dur);
      }

      if (inStart) {
        start[startFillIndex] = currentChar;
        startFillIndex += 1;
      }

      if (inEnd) {
        end[endFillIndex] = currentChar;
        endFillIndex += 1;
      }
    }
  }
  test(allDurations);
  fclose(filePointer);
  return 0;
}

// TODO remove
void test(struct duration *allDurations) {
  struct duration *dur;

  HASH_FIND_STR(allDurations, "2022-09-05", dur);

  if (dur == NULL) {
    printf("Key not found");
    return;
  }

  printf("%d\n", dur->minutes);
}

