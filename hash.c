#include"hash.h"




HashTable *newTable(int (*entryCmpr)(HashEntry*, HashEntry*)) {

    HashTable *new_table= malloc(sizeof(HashTable));
    
    /* Initialize table */
    new_table->total_entries = 0;
    new_table->unique_entries = 0;
    new_table->highest_collision_count = 0;
    new_table->total_collisions = 0;
    new_table->entryCompareFnx = entryCmpr; 
    new_table->table_size = INITIAL_TABLE_SIZE;
    new_table->table_directory[INITIAL_TABLE_SIZE];   
 
    for (int i = 0; i < INITIAL_TABLE_SIZE; i++) {
        new_table->table_directory[i] == NULL;
    }
    return new_table; 
}


void addEntry(HashTable *hash_table, HashEntry *new_entry) {

    // unsigned long long int hash = hash(new_entry->hash_field) % hash_table->table_size;

    
}
