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
  const char **pstr = (const char **) q;
  const char **qstr = (const char **) q;
  int len_p = strlen(*pstr);
  int len_q = strlen(*qstr);

  if (len_p == len_q) return strcmp(*pstr, *qstr);
  //  return (len_p > len_q) - (len_p < len_q);
  return len_p > len_q ? 1 : -1;
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

    if (*n_line >= size) {   /* if there isn't enough memory, double the size of the allocation.*/
      size *= 2;
      lines = realloc(lines, size * sizeof(char *));
      assert(lines);
    }
    
    if (!uniq) {
      lines[*n_line] = strdup(line);
      (*n_line)++;
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
  char **lines = getLines(fp, &n_line, cmp_pstr, uniq);
  assert(lines);
  //  qsort(lines, n_line, sizeof(char *), cmp_pstr);
    qsort(lines, n_line, sizeof(char *), cmp);
  if (reverse) {
    /* printt the lines in the reverse order */
    while (n_line--) {
      printf("%s", lines[n_line]);
      free(lines[n_line]);
    }
  } else { /* print the lines as it is */
    for (int i = 0; i < n_line; i++) {
       printf("%s", lines[i]);
       free(lines[i]);
    }

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
