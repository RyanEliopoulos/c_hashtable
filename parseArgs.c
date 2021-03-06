#include"parseArgs.h"

/*
 *  This function screens the command-line arguments passed to the program to ensure
 *  smooth operation and offer guidance on use as needed.
 *
 *  <count> must be an integer greater than zero and at least one filepath must be supplied.
 *  Program will fail if any of the supplied filepaths are invalid. 
 *  An exit code of 1XX indicates the program has failed this initial screen.
 */ 
int parseArgs(int argc, char *argv[]) {

    /* Checking for insufficient command-line arguments */
    if (argc == 1) {
        fprintf(stderr, "Need to include at least one file path\n");
        exit(100);
    }

    /* Checking for user-supplied <count> value. */ 
    int count = -1; // If -1 persists, no user-specified <count> value

    if (argv[1][0] == '-') {  /* user-supplied <count> value */
        char dash; // Only used to satisfy sscanf 
        sscanf(argv[1], "%c %d", &dash, &count);
        
        if (count <= 0) {  /* <count> is not an integer or is an invalid value */
            fprintf(stderr, "Invalid input. Must be formatted: <-count> filepath1 <filepath2> <filepath3> .. where count is > 0\n");
            exit(101);
        }
        if (argc == 2) { // If Valid <count> but no file paths 
            fprintf(stderr, "Must include at least one filepath\n");
            exit(102);
        }
    }
    
    /* Count will now be > 0 and filepaths will be checked from argv[2] on */
    /* OR count will be -1 and filepaths will be checked from argv[1] on */
    int i = (count == -1) ? 1 : 2; 
    struct stat statbuf; // Holds file information

    int stat_ret;  // Will help evaluate stat() call
    for (i; i < argc; i++) {
        stat_ret = stat(argv[i], &statbuf); // 0 upon success 

        if (stat_ret || !S_ISREG(statbuf.st_mode)) {
            fprintf(stderr, "\"%s\" is not a valid file path\nBe sure to prefix the count with a dash\n", argv[i]);
            exit(103);
        }
    }
    /* All filepaths have now been verified */
    //printf("All filepaths resolved\n"); 
    return count;
}


