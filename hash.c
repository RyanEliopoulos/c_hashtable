#include"hash.h"




HashTable *newTable(int (*tableCompareFnx)(HashEntry*, HashEntry*)) {

    HashTable *new_table= malloc(sizeof(HashTable));
    
    /* Initialize table */
    new_table->total_entries = 0;
    new_table->unique_entries = 0;
    new_table->highest_collision_count = 0;
    new_table->total_collisions = 0;
    new_table->tableCompareFnx = tableCompareFnx; 

    return new_table; 
}



