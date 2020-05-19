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

char **getLines(FILE *fp, size_t *n_line, cmp_fn_t cmp, bool uniq) {
  int size = 100*MIN_NLINES;
  char **lines = malloc(size * sizeof(char *));
  assert(lines);
  char line[MAX_LINE_LEN];
 
  while (fgets(line, MAX_LINE_LEN, fp) != NULL) {

    if (*n_line == size) {   /* if there isn't enough memory, double the size of the allocation.*/
      size *= 2;
      lines = realloc(lines, size * sizeof(char *));
      //      assert(lines);
    }
    
    if (!uniq) {
      lines[*n_line] = strdup(line);
      ++(*n_line);
    } else {
      char *key = line;

      char **new = binsert(&key, lines, n_line, sizeof(char *), cmp);
      *new = strdup(key);
      }
  }
  return lines;

}

void sort_lines(FILE *fp, cmp_fn_t cmp, bool uniq, bool reverse) {
  // TODO: implement this function
  size_t n_line = 0;
  char **lines = getLines(fp, &n_line, cmp, uniq);
  assert(lines);
  qsort(lines, n_line, sizeof(char *), cmp);

  char **newLines = malloc(n_line * sizeof(char *));
  
  int len = strlen(lines[0]);
  char *str = lines[0];
  int count = 0;
  if (uniq && cmp == cmp_pstr_len) {
    
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
    len = atoi(lines[0]);
    count = 0;
    str = lines[0];
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
    for(int i = 0; i < n_line; i++)
      newLines[i] = lines[i];
  }
  if (reverse) {
    //    for (int i = 0; i < n_line; i++)
    // newLines[i] = lines[n_line - i];
    while (n_line--) {
      printf("%s", newLines[n_line]);
      //      free(newLines[n_line]);
    }
  }
  for (int i = 0; i < n_line; i++) {
    printf("%s", newLines[i]);
    free(newLines[i]);
  }

  
  free(lines); 
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
