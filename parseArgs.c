#include"parseArgs.h"


int parseArgs(int argc, char *argv[]) {

    /* Checking for insufficient command-line arguments */
    if (argc == 1) {
        printf("Need to include at least one file path\n");
        exit(100);
    }

    /* Checking for user-supplied <count> value. */ 
    int count = -1; // If -1 persists, no user-specified <count> value

    if (argv[1][0] == '-') { 
        char dash; // Only used to satisfy sscanf 
        sscanf(argv[1], "%c %d", &dash, &count);

        if (count <= 0) { 
            printf("Invalid input. Must be formatted: <-count> filepath1 <filepath2> <filepath3> .. where count is > 0\n");
            exit(101);
        }
        if (argc == 2) { // If Valid <count> but no file paths 
            printf("Must include at least one filepath\n");
            exit(102);
        }
    }
    
    /* Count will now be > 0 and filepaths will be checked from argv[2] on */
    /* OR count will be -1 and filepaths will be checked from argv[1] on */
    int i = (count == -1) ? 1 : 2; 
    struct stat statbuf; // Holds file information

    int stat_ret;  // Will help evaluate stat() call
    for ( i ; i < argc ; i++ ) {
        stat_ret = stat(argv[i], &statbuf); // 0 upon success 

        if (stat_ret || !S_ISREG(statbuf.st_mode)) {
            printf("\"%s\" is not a valid file path\n", argv[i]);
            exit(103);
        }
    }
    /* All filepaths have now been verified */

    printf("All filepaths resolved\n"); 
    return count;
}


