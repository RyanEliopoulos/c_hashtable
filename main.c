/* Remember to assert after every malloc */
/* Probably should go through the code and change ints to unsigned long, etc. in certain places. Don't want overflow */

#include"parseArgs.h"
#include"parseFiles.h"
#include<stdlib.h>

void main(int argc, char* argv[]) {
    
    int count = parseArgs(argc, argv);
    printf("Count is :%d\n", count);

    
    HashTable *hash_table = parseFiles(count, argc, argv);
    free(hash_table->table_directory);
    free(hash_table);
    //printf("total entries:%llu\n", hash_table->total_entries);
}
