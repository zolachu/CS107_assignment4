
#include <dirent.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <string.h>

enum { SORT_BY_NAME, SORT_BY_TYPE };
enum { EXCLUDE_DOT, INCLUDE_DOT };

/* On the myth filesystem, the only file type information that is accurate is
 * directory/not-directory used here. Other type info in struct dirent is 
 * not reliable.
 */
bool is_dir(const struct dirent *dirptr) {
    return dirptr->d_type == DT_DIR;
}

/*
 * Function: sortByType()
 * ---------------
 * Compares directory entries lexicographically and sorts by directory type.
 */

int sortByType(const struct dirent **d1,const struct dirent **d2) {
  if (is_dir(*d1) == is_dir(*d2)){
    return strcmp((*d2)->d_name,(*d1)->d_name);
  }
  if (is_dir(*d1) && !is_dir(*d2)) {
    return 1;
  }
  return -1;
}


/*
 * Function: sortByName()
 *---------------------
 * Compares directory entries lexicographically regardless of 
 * directory or non-directory.
 */

int sortByName(const struct dirent **d1,const struct dirent **d2) {
  return(strcmp((*d2)->d_name,(*d1)->d_name));
}


/*
 * Function: selDir()
 * ------------------
 * returns true if the directory itself is a directory.
 */

int excludeDot(const struct dirent *d) {
  return is_dir(d) ? 0 : 1;
}

void ls(const char *dirpath, int filter, int order) {
    // TODO: implement this function

  struct dirent **names;

  int (*compare)(const struct dirent**, const struct dirent**) = (order == SORT_BY_TYPE ? &sortByType : &sortByName);
  
  int (*filters)(const struct dirent*) = (filter == EXCLUDE_DOT ? &excludeDot : NULL);

  int count = scandir(dirpath, &names, filters, compare);

  while(--count >0) {
    char* name = names[count]->d_name;
    if(filter == 0  && *name == '.') continue;
    printf("%s", names[count]->d_name);
    printf(is_dir(names[count]) ? "/\n" : "\n");

    //    free(names[count]);
  }
  //  free(names);
}


int main(int argc, char *argv[]) {
    int order = SORT_BY_NAME;
    int filter = EXCLUDE_DOT;

    int opt = getopt(argc, argv, "az");
    while (opt != -1) {
        if (opt == 'a') {
            filter = INCLUDE_DOT;
        } else if (opt == 'z') {
            order = SORT_BY_TYPE;
        } else {
            return 1;
        }

        opt = getopt(argc, argv, "az");
    }
    
    if (optind < argc - 1) {
        for (int i = optind; i < argc; i++) {
            printf("%s:\n", argv[i]);
            ls(argv[i], filter, order);
            printf("\n");
        }
    } else {
        ls(optind == argc -1 ? argv[optind] : ".", filter, order);
    }
    
    return 0;
}
