/* Probably should go through the code and change ints to unsigned long, etc. in certain places. Don't want overflow */


/* anticipate reading in too much information that oveflows values in the hash table? */

#include"parseArgs.h"
#include"parseFiles.h"
#include<stdlib.h>


void printWords(int, HashEntry **);

void main(int argc, char* argv[]) {
    
    int count = parseArgs(argc, argv);
    //printf("Count is :%d\n", count);

    HashTable *hash_table = parseFiles(count, argc, argv);
    HashEntry **sorted_entries = sortPairs(hash_table);

    /* if count is unspecified or exceeds entry count, make adjustment */
    if (count == -1 || count > hash_table->unique_entries) count = hash_table->unique_entries;

    printWords(count, sorted_entries);

    releaseTable(hash_table);
    free(sorted_entries);
    /* Printing out "sorted" word pairs */
    /*
    char *string1;
    char *string2;  
    //for (int i = 0; i < hash_table->unique_entries; i++ ) {
    for (int i = 0; i < count; i++ ) {
        string1 = sorted_entries[i]->data->string1;
        string2 = sorted_entries[i]->data->string2;
        printf("<%s>, <%s>, %llu\n", string1, string2, sorted_entries[i]->occurrences);

    } */
    //HashEntry **unpacked_entry_array = unpackTableEntries(hash_table);

    // This is where the sortPairs() call goes
    // sortPairs(hash_table); // calls unpackTableEntries and qsort()
    // BEGIN: these three lines free all elements of the table and sub-structures.
    /* So, the logc within the unpackTableEntries() is likely causing the segfault */
    //debug_traverseTable(hash_table);
    //free(hash_table->table_directory);
    //free(hash_table);
    //free(sorted_entries);
    // END
    
    // DEBUG - should be getting a hash table with complete set of values.
    // Now will test the unpack function

    //printf("in main, beginning to free\n");        
    //printf("Unique entries: %llu\n", hash_table->unique_entries);
    //for (int i = 0; i < hash_table->unique_entries; i++) {
        //if (unpacked_entry_array[i] != NULL) {
            //printf("<%s>\n", unpacked_entry_array[i]->data->hash_field);
            //freeHashEntry(hash_table->freeDataFnx, unpacked_entry_array[i]);
       //}
        //else {
            //printf("Got a null value somehow\n");
        //}
    //}    
    //free(hash_table->table_directory);
    //free(hash_table);
    //free(unpacked_entry_array);
    //for (int i = 0; i < hash_table->table_size; i++) {
        //if (*(unpacked_entry_array + i) != NULL) {
            //printf("hello!\n");
        //}
    //} 
    
    //free(hash_table->table_directory);
    //free(hash_table);
    //printf("total entries:%llu\n", hash_table->total_entries);
}


void printWords(int count, HashEntry **sorted_array) {

    //printf("<%s> <%s> %d\n", sorted_array[0]->data->string1, sorted_array[0]->data->string2, sorted_array[0]->occurrences);
    for (int i = 0; i < count; i++) {
            
        /* retrieve the entry's string values */ 
        char *string1 = sorted_array[i]->data->string1;
        char *string2 = sorted_array[i]->data->string2;        
     
        /* Transfer the strings into a single string */
        char final_string[DICT_MAX_WORD_LEN * 2];
        strcpy(final_string, string1);
        char space[2] = " ";
        strcat(final_string, space);
        strcat(final_string, string2);
        
        printf("%10d %s\n", sorted_array[i]->occurrences, final_string);
            
    }
}
