#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define TIME "2018-08-06 17:19:12 +0200"

int main(void)
{
  printf("%lu\n", strlen(TIME));
  return 0;
  /*  */
  /* struct tm tm; */
  /* char buf[255]; */
  /* memset(&tm, 0, sizeof(tm)); */
  /* strptime(TIME, "%Y-%m-%d %H:%M:%S", &tm); */
  /* strftime(buf, sizeof(buf), "%d %b %Y %H:%M", &tm); */
  /* puts(buf); */
  /*  */
  /* return 0; */
}

