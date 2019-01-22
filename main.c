/* Remember to assert after every malloc */
/* Probably should go through the code and change ints to unsigned long, etc. in certain places. Don't want overflow */

#include"parseArgs.h"
#include"parseFiles.h"
#include<stdlib.h>

void main(int argc, char* argv[]) {
    
    int count = parseArgs(argc, argv);
    printf("Count is :%d\n", count);

    
    HashTable *hash_table = parseFiles(count, argc, argv);



    HashEntry **unpacked_entry_array = unpackTableEntries(hash_table);
    // BEGIN: these three lines free all elements of the table and sub-structures.
    /* So, the logc within the unpackTableEntries() is likely causing the segfault */
    //debug_traverseTable(hash_table);
    //free(hash_table->table_directory);
    //free(hash_table);
    //free(unpacked_entry_array);
    // END
    
    // DEBUG - should be getting a hash table with complete set of values.
    // Now will test the unpack function

    //printf("in main, beginning to free\n");        
    //printf("Unique entries: %llu\n", hash_table->unique_entries);
    for (int i = 0; i < hash_table->unique_entries; i++) {
        if (unpacked_entry_array[i] != NULL) {
            printf("<%s>\n", unpacked_entry_array[i]->data->hash_field);
            freeHashEntry(hash_table->freeDataFnx, unpacked_entry_array[i]);
       }
        else {
            printf("Got a null value somehow\n");
        }
    }    
    free(hash_table->table_directory);
    free(hash_table);
    free(unpacked_entry_array);
    //for (int i = 0; i < hash_table->table_size; i++) {
        //if (*(unpacked_entry_array + i) != NULL) {
            //printf("hello!\n");
        //}
    //} 
    
    //free(hash_table->table_directory);
    //free(hash_table);
    //printf("total entries:%llu\n", hash_table->total_entries);
}
