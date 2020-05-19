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
 * Compares strings lexicographically 
 */
int cmp_pstr(const void *p, const void *q) {
  // TODO: implement this function (and remove the line below)
  const char **ip = (const char **)p;
  const char **iq = (const char **)q;
  return strcmp(*ip, *iq);
}

/*
 * Compares strings by length (not lexicographically)
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
 * Compares strings numerically. Example "3"<"5".
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
 * Reads line by line from fp file descriptor and returns a pointer to array of
 * unique strings read from the file.
 * Uses cmp comparison function and binsert in order to sort the array.
 */

char **getUniqueLines(FILE *fp, size_t *n_line, cmp_fn_t cmp) {
  size_t size = MIN_NLINES;
  char **lines = (char**)malloc(size * sizeof(char *));
  assert(lines);
  char line[MAX_LINE_LEN];
 
  while (fgets(line, MAX_LINE_LEN, fp) != NULL) {
    
    if (*n_line == size - 1) {   /* if there isn't enough memory, double the size of the allocation. */
      size *= 2;
      lines = realloc(lines, size * sizeof(char *));
      assert(lines);
    }
    char *key = line;
    char **new = binsert(&key, lines, n_line, sizeof(char *), cmp);
    if (key == *new) *new = strdup(key);
  }
  return lines;
}

/*                                                                                                                                                                                      
 * Reads line by line from fp file descriptor and returns a pointer to array of                                                                                                         
 * strings read from the file.The order of the strings is as it is read from the file.                                                                                                 
 */

char **getLines(FILE *fp, size_t *n_line) {
  int size = MIN_NLINES;
  char **lines = malloc(size * sizeof(char *));
  assert(lines);
  char line[MAX_LINE_LEN];
 
  while (fgets(line, MAX_LINE_LEN, fp) != NULL) {
    
    if (*n_line == size - 1) {   /* if there isn't enough memory, double the size of the allocation.*/
      size *= 2;
      lines = realloc(lines, size * sizeof(char *));
      assert(lines);
    }

    lines[*n_line] = strdup(line);
    ++(*n_line); 
  }
  return lines;
}


/*
 * Sorts all the lines read from file descriptor fp.
 * If uniq is true, it prints out lines with no duplicates.
 * If uniq is true AND the comparison function is "compare by length"
 * then the strings with equal length are considered duplicates.
 * If uniq is true AND the comparison function is "compare by numeric"
 * then the strings with equal numerical values are considered duplicates.
 */
void sort_lines(FILE *fp, cmp_fn_t cmp, bool uniq, bool reverse) {
  // TODO: implement this function
  size_t n_line = 0;
  char **lines = uniq ? getUniqueLines(fp, &n_line, cmp) : getLines(fp, &n_line); // This makes repeated calls to binsert ONLY if uniq == TRUE.
  assert(lines);
  
  if (!uniq) qsort(lines, n_line, sizeof(char *), cmp);

  char **newLines = malloc(n_line * sizeof(char *));  // This pointer is a pointer to an array where there is no duplicates, if any.
  // lines array might have duplicates. we will remove all the duplicates if uniq is set to true.
  int count = 0;
  if (uniq && cmp == cmp_pstr_len) {            // If uniq is true and strings are compared by length 
    count = 0;
    int len = strlen(lines[0]);
    char* str = lines[0];
    count = 0;
    for (int i = 0; i < n_line; i++) {          // by comparing the string lengths, populate newLines with the very last strings of the same lengths.
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
  } else if (uniq && cmp == cmp_pstr_numeric){  // If uniq is true and strings are compared by its numerical value
    int len = atoi(lines[0]);                   // convert string lines[0] to its numerical value.
    count = 0;
    char* str = lines[0];      
    for (int i = 0; i < n_line; i++) {          // by comparing the string lengths, populate newLines with the very last strings of the same lengths. 
      if (i == n_line - 1)                      // If the i is the very last index
	newLines[count] = lines[i];             // the very last element of newLines is set to the last string of lines array.
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
      free(newLines[i]);
      free(lines[i]);
  }

  free(lines);
  free(newLines);
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
