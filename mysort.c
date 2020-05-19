#include "samples/prototypes.h"
#include <errno.h>
#include <error.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <search.h>
#include <assert.h>

#define MAX_LINE_LEN 4096
#define MIN_NLINES 100

typedef int (*cmp_fn_t)(const void *p, const void *q);

/*
 */
int cmp_pstr(const void *p, const void *q) {
  // TODO: implement this function (and remove the line below)
  const char **ip = (const char **)p;
  const char **iq = (const char **)q;
  return strcmp(*ip, *iq);
}


/*                                                                                                                                                                                      
 *                                                                                                                                                                                     
 *                                                                                                                                                                                     
 */

int cmp_pstr_len(const void *p, const void *q) {
  // TODO: implement this function (and remove the line below)
  const char *ip = *(const char * const *)p;
  const char *iq = *(const char * const *)q;
  size_t len1 = strlen(ip);
  size_t len2 = strlen(iq);
  return len1 > len2 ? 1 : -1;
}

/*
 *
 */

int cmp_pstr_numeric(const void *p, const void *q) {
  // TODO: implement this function (and remove the line below)
  const char *s1 = *(const char **) p;
  const char *s2 = *(const char **) q;
  int num1 = atoi(s1);
  int num2 = atoi(s2);
  return num1 > num2 ? 1 : -1;
}

/*                                                                                                                                                                                      
 *                                                                                                                                                                                      
 *                                                                                                                                                                                    
 */

char **getUniqueLines(FILE *fp, size_t *n_line, cmp_fn_t cmp) {
  size_t size = MIN_NLINES;
  char **lines = (char**)malloc(size * sizeof(char **));
  //  assert(lines);
  char line[MAX_LINE_LEN];
  
  while (fgets(line, MAX_LINE_LEN, fp) != NULL) {
    printf("%lu is supposed to be 100 \n", size);
    if (sizeof(lines)/sizeof(*lines) == size) {   /* if there isn't enough memory, double the size of the allocation.*/
      size *= 2;
      printf("%lu hi \n", size);
      lines = (char**)realloc(lines, size * sizeof(char **));     
     
    }

    char *key = line;
    char **new = binsert(&key, lines, n_line, sizeof(char *), cmp);
    if (key == *new) *new = strdup(key);
  }
  return lines;
}


/*
 *
 *
 */

char **getLines(FILE *fp, size_t *n_line) {
  int size = MIN_NLINES;
  char **lines = malloc(1000*size * sizeof(char *));
  assert(lines);
  char line[MAX_LINE_LEN];
 
  while (fgets(line, MAX_LINE_LEN, fp) != NULL) {
    
    if (*n_line == size) {   /* if there isn't enough memory, double the size of the allocation.*/
      //      size *= 2;
      //  lines = realloc(lines, size * sizeof(char *));
      //assert(lines);
    }

    lines[*n_line] = strdup(line);
    ++(*n_line); 
  }
  return lines;
}


void sort_lines(FILE *fp, cmp_fn_t cmp, bool uniq, bool reverse) {
  // TODO: implement this function
  size_t n_line = 0;
  char **lines = uniq ? getUniqueLines(fp, &n_line, cmp) : getLines(fp, &n_line); // This makes repeated calls to binsert ONLY if uniq == TRUE.
  //  assert(lines);
  
  if (!uniq) qsort(lines, n_line, sizeof(char *), cmp);

  char **newLines = malloc(n_line * sizeof(char *));
  int count = 0;
  if (uniq && cmp == cmp_pstr_len) {
    count = 0;
    int len = strlen(lines[0]);
    char* str = lines[0];
    count = 0;
    for (int i = 0; i < n_line; i++) {
      if (i == n_line - 1)
	newLines[count] = lines[i];
      if (strlen(lines[i]) == len) {
	str = lines[i];
      } else {
	len = strlen(lines[i]);
	newLines[count] = str;
	count++;
      }
    }
  } else if (uniq && cmp == cmp_pstr_numeric){
    int len = atoi(lines[0]);
    count = 0;
    char* str = lines[0];
    for (int i = 0; i < n_line; i++) {
      if (i == n_line - 1)
	newLines[count] = lines[i];
      if (atoi(lines[i]) == len) {
	str = lines[i];
      } else {
	len = atoi(lines[i]);
	newLines[count] = str;
	count++;
      }
    }
  } else {
    for(int i = 0; i < n_line; i++) newLines[i] = lines[i];
  }
  
  if (reverse) {
    while (n_line--) {
      if (newLines[n_line] != NULL)
      	printf("%s",newLines[n_line]);
    }
    return;
  }
  for (int i = 0; i < n_line; i++) {
    if (newLines[i] != NULL)
      printf("%s", newLines[i]);
    //free(newLines[i]);
    //free(lines[i]);
  }

  //  free(lines);
  //free(newLines);
}

int main(int argc, char *argv[]) {
    cmp_fn_t cmp = cmp_pstr;
    bool uniq = false;
    bool reverse = false;

    int opt = getopt(argc, argv, "lnru");
    while (opt != -1) {
        if (opt == 'l') {
            cmp = cmp_pstr_len;
        } else if (opt == 'n') {
            cmp = cmp_pstr_numeric;
        } else if (opt == 'r') {
            reverse = true;
        } else if (opt == 'u') {
            uniq = true;
        } else {
            return 1;
        }

        opt = getopt(argc, argv, "lnru");
    }

    FILE *fp = stdin;
    if (optind < argc) {
        fp = fopen(argv[optind], "r");
        if (fp == NULL) {
            error(1, 0, "cannot access %s", argv[optind]);
        }
    }
    sort_lines(fp, cmp, uniq, reverse);
    fclose(fp);
    return 0;
}
