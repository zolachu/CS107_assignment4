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

int cmp_pstr(const void *p, const void *q) {
  // TODO: implement this function (and remove the line below)
  const char *pstr = *(const char **) p;
  const char *qstr = *(const char **) q;

  return strcmp(pstr, qstr);
}

int cmp_pstr_len(const void *p, const void *q) {
  // TODO: implement this function (and remove the line below)
  const char *pstr = *(const char **) q;
  const char *qstr = *(const char **) q;
  int len_p = strlen(pstr);
  int len_q = strlen(qstr);

  return (len_p == len_q) ? cmp_pstr(pstr, qstr) : ((len_p > len_q) - (len_p < len_q));
}

int cmp_pstr_numeric(const void *p, const void *q) {
  // TODO: implement this function (and remove the line below)
  const char *s1 = *(const char **) p;
  const char *s2 = *(const char **) q;
  int num1 = atoi(s1);
  int num2 = atoi(s2);
  return (num1 > num2) - (num1 < num2);
}

/*
 *
 *
 */

char **getLines(FILE *fp, size_t *n_line, cmp_fn_t cmp, bool uniq) {
  int size = MIN_NLINES;
  char **lines = malloc(size * sizeof(char *));
  assert(lines);
  char line[MAX_LINE_LEN];
 
  while (fgets(line, MAX_LINE_LEN, fp) != NULL) {
  //  for (int i = 0; i < 2; i++){
  fgets(line, MAX_LINE_LEN, fp) ;
  printf("%lu", *n_line);
  printf("%d", size);
  if (*n_line >= size) {   /* if there isn't enough memory, double the size of the allocation.*/

    size *= 2;
    lines = realloc(lines, size * sizeof(char *));
    // assert(lines != NULL);
    }
    
    if (!uniq) {
      lines[*n_line] = strdup(line);
      (*n_line)++;
    } else {
      char *key = line;
      //      assert(lines);
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
  //  printf("%s", *lines);
  qsort(lines, n_line, sizeof(char *), cmp);
  if (reverse) {
    while (n_line--) {
      printf("%s", lines[n_line]);
      //free(lines[n_line]);
    }
  } else {
    for (int i = 0; i < n_line; i++) {
       printf("%s", lines[i]);
       //       free(lines[i]);
    }

  }
  //  free(lines); 
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
