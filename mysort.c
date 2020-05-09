#include "samples/prototypes.h"
#include <errno.h>
#include <error.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_LINE_LEN 4096
#define MIN_NLINES 100

typedef int (*cmp_fn_t)(const void *p, const void *q);

int cmp_pstr(const void *p, const void *q) {
    // TODO: implement this function (and remove the line below)
    return 0;
}

int cmp_pstr_len(const void *p, const void *q) {
    // TODO: implement this function (and remove the line below)
    return 0;
}

int cmp_pstr_numeric(const void *p, const void *q) {
    // TODO: implement this function (and remove the line below)
    return 0;
}

void sort_lines(FILE *fp, cmp_fn_t cmp, bool uniq, bool reverse) {
    // TODO: implement this function
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
