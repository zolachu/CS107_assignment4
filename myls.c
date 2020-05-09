#include <dirent.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>


enum { SORT_BY_NAME, SORT_BY_TYPE };
enum { EXCLUDE_DOT, INCLUDE_DOT };

/* On the myth filesystem, the only file type information that is accurate is
 * directory/not-directory used here. Other type info in struct dirent is 
 * not reliable.
 */
bool is_dir(const struct dirent *dirptr) {
    return dirptr->d_type == DT_DIR;
}

void ls(const char *dirpath, int filter, int order) {
    // TODO: implement this function
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
