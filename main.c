/*  Ryan Paulos
 *  CS 360, Assignment 1
 *  
 *  
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */




#include"main.h"


void main(int argc, char* argv[]) {

    /* Figure out what the user wants */    
    int count = parseArgs(argc, argv);

    /* Hash word pairs */
    HashTable *hash_table = parseFiles(count, argc, argv);

    /* Sort word pairs */
    HashEntry **sorted_entries = sortPairs(hash_table);

    /* if count is unspecified or exceeds table entry count, make adjustment */
    if (count == -1 || count > hash_table->unique_entries) count = hash_table->unique_entries;

    /* print word pairs */
    printWords(count, sorted_entries);

    /* Free hash table and all associated structures */
    releaseTable(hash_table);
    free(sorted_entries);
}


/* iterates over the sorted array and prints the word pairs */
void printWords(int count, HashEntry **sorted_array) {

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
