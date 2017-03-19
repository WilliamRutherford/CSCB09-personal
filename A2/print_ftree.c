#include <stdio.h>
#include <stdlib.h>

#include "cmptree.h"
#include "ftree.h"

int main(int argc, char **argv) {
    int mode = -1, retval;
    struct TreeNode *computed = NULL, *expected = NULL;

    if (argc != 3) {
        printf("Usage: ftree <directory|file> <0|1|2>\n");
        exit(EXIT_FAILURE);
    }

    expected = _generate_ftree_(argv[1]);
    computed = generate_ftree(argv[1]);

    /* Compare the two trees. */
    mode = strtol(argv[2], NULL, 10);

    printf("Comparing the computed tree versus the expected tree ...\n");
    fflush(stdout);

    switch(mode) {
    case 0: {
        retval = cmptree(expected, computed, FNAME);
        break;
    }
    case 1: {
        retval = cmptree(expected, computed, PERMISSIONS);
        break;
    }
    case 2: {
        retval = cmptree(expected, computed, HASH);
        break;
    }
    default:
        fprintf(stderr, "Invalid mode: %d!\nExiting ...", mode);
        break;
    }

    if(!retval) {
	printf("The two trees are equal!\n");
	exit(0);
    }
    else
	exit(1);
}

