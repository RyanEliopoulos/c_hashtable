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


void addEntry(HashTable *hash_table, Data *data_entry) {

    unsigned long long int hash = crc64(data_entry->hash_field) % hash_table->table_size; // Calculating hash value
    
    HashEntry *new_entry = newTableEntry(data_entry); // Creating an entry node for the hash table

    if (hash_table->table_directory[hash] == NULL) { // Bucket is empty, assign and return
        hash_table->table_directory[hash] = new_entry;
        return;
    } 

    /* Bucket is not empty */
    /* Will iterate through bucket list, incrementing a matching entry or adding the new unique entry to the end. */
    HashEntry *temp_entry =  hash_table->table_directory[hash];

    while ( temp_entry->next_node != NULL ) {

        /* Comparing HashEntry values */
        if ( (*hash_table->entryCompareFnx)(temp_entry, new_entry) ) {  // True if entries match.
            temp_entry->occurrences++; 
            return;
        }
        temp_entry = temp_entry->next_node; 
    }
   
    /* Checking the last value in the list */
    if ( (*hash_table->entryCompareFnx)(temp_entry, new_entry) ) {
        temp_entry->occurrences++;
    }
    else {
       temp_entry->next_node = new_entry; // new_entry is unique to the hash table
    }
}


HashEntry *newTableEntry(Data *data_entry) {    

    /* Create and initialize new HashEntry */ 
    HashEntry *new_entry = malloc(sizeof(HashEntry));
    new_entry->data = data_entry;
    new_entry->next_node = NULL;
    new_entry->occurrences = 0;

    return new_entry;
}
