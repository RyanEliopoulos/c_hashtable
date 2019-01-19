
/* Remember to assert after every malloc */
/* Probably should go through the code and change ints to unsigned long, etc. in certain places. Don't want overflow */

#include"parseArgs.h"
#include"parseFiles.h"

void main(int argc, char* argv[]) {
    
    int count = parseArgs(argc, argv);
    printf("Count is :%d\n", count);

    
    parseFiles(count, argc, argv);
    //HashTable *new_table = newTable();

    printf("done\n");
}
