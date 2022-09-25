#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>

#include <uthash.h>

#define PATH "data/export.xml"
#define TYPE "HKQuantityTypeIdentifierStepCount"
// TODO refactor \" to be in inTag()
#define START "startDate=\""
#define END "endDate=\""

/* map */
struct duration {
  char date[11];
  int seconds;
  UT_hash_handle hh;
};

/* time */
struct tm toTm(char timestamp[]) {
  struct tm tm;
  memset(&tm, 0, sizeof(tm));
  strptime(timestamp, "%Y-%m-%d %H:%M:%S", &tm);
  return tm;
}

int diff(struct tm date1, struct tm date2) {
  return difftime(mktime(&date1), mktime(&date2));
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

/* json */
void format(char date[], int seconds, int ts) {
  char template[] = "{ "
  "\"timestamp\": \"%sT12:00:00\","
  "\"eventDescriptor\": {"
  " \"name\": \"WALKING\" },"
  "\"value\": %d,"
  "\"validicEventId\": \"110867-%lu\","
  "\"customDescription\": null,"
  "\"utcOffset\": 120"
  " }\n";

  printf(template, date, seconds / 60, ts);
}

void printAll(struct duration *allDurations) {
  struct duration *dur;
  struct timeval now;
  gettimeofday(&now, NULL);
  long int ts = (long int)now.tv_sec * 1000 + now.tv_usec / 1000;

  for (dur = allDurations; dur != NULL; dur = dur->hh.next) {
    ts += 1;
    format(dur->date, dur-> seconds, ts);
  }
}

int main(int argc, char **argv) {
  FILE *filePointer;
  char currentChar;

  if (argc == 1) {
    printf("No cutoff specifiedi\n");
    exit(1);
  }

  struct duration *allDurations = NULL;
  struct tm cutoff = toTm(argv[1]);

  if (mktime(&cutoff) < 0) {
    printf("Cutoff must be YYYY-MM-DD\n");
    exit(1);
  }

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

        struct tm startDate, endDate;
        startDate = toTm(start);
        
        // is before cutoff
        if (diff(startDate, cutoff) < 0) {
          continue;
        }

        struct duration *dur;
        char key[11];

        endDate = toTm(end);
        justDate(key, start);

        HASH_FIND_STR(allDurations, key, dur);

        if (dur == NULL) {
          dur = (struct duration *)malloc(sizeof *dur);
          strcpy(dur->date, key);
          dur->seconds = 0;
          HASH_ADD_STR(allDurations, date, dur);
        }

        dur->seconds += diff(endDate, startDate);
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
  printAll(allDurations);
  fclose(filePointer);
  return 0;
}

