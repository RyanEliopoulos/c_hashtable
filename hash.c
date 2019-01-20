/* Going to want a hash function that unpacks all HashEntry nodes into a linear array. */

/* Will then use this array in qsort(). */
/* Will also want a function that unpacks all of the Data pointers into an array so that the table controller  */
/* can can free the relevant Data fields. This unpacking function will free the HashEntry nodes */
#include<stdio.h>
#include"hash.h"

/* Initializes a new hash table. Requires pointer to a function that will be used to compare Data objects. */
/* Internally the table will increment a relevant counter if an identical Data object is inserted into the table */
//HashTable *newTable(unsigned long long int table_size, int (*entryCmpr)(HashEntry*, HashEntry*)) {
HashTable *newTable(unsigned long long int table_size, entryCompareFnx entryCmpr, fnxFreeData freeDataFnx){

    HashTable *new_table= malloc(sizeof(HashTable));
    
    /* Initialize table values */
    new_table->total_entries = 0;
    new_table->unique_entries = 0;
    new_table->highest_collision_count = 0;
    new_table->total_collisions = 0;

    /* Initializing table helper functions */
    new_table->entryCompareFnx = entryCmpr; 
    new_table->freeDataFnx = freeDataFnx;

    /* Initializing table */
    new_table->table_size = table_size;
    new_table->table_directory = malloc(sizeof(HashEntry *) * table_size);

    for (int i = 0; i < table_size; i++) {
        new_table->table_directory[i] == NULL;
    }
    return new_table; 
}


/* Handles inserting new Data into the hash table. */
/* Need to add: state tracking */
void addEntry(HashTable *hash_table, Data *data_entry) {

    unsigned long long int hash = crc64(data_entry->hash_field) % hash_table->table_size; // Calculating hash value
    printf("successfully hashed\n");    
    HashEntry *new_entry = newTableEntry(data_entry); // Creating an entry node for the hash table
   
    /* Placing the new_entry into the bucket */ 
    if (hash_table->table_directory[hash] == NULL) { // Bucket is empty, assign and return
        hash_table->table_directory[hash] = new_entry;
        printf("added new entry to table\n");
        return;
    } 

    /* Bucket is not empty */
    /* Will iterate through bucket list, incrementing a matching entry or adding the new unique entry to the end. */
    HashEntry *temp_entry =  hash_table->table_directory[hash];
    unsigned long int bucket_collisions = 1;
    printf("About to enter the list traversal while loop\n");
    while ( temp_entry->next_node != NULL ) {

        /* Comparing HashEntry values */
        printf("about to compare hash values\n");
        if ( (*hash_table->entryCompareFnx)(temp_entry, new_entry) ) {  // True if entries match.
            temp_entry->occurrences++; 
            return;
        }
        temp_entry = temp_entry->next_node; 
        bucket_collisions++;
    }
    printf("make it to \"Checking last values\"\n");   
    /* Checking the last value in the list */
    if ( (*hash_table->entryCompareFnx)(temp_entry, new_entry) ) {
        temp_entry->occurrences++;
    }
    else {
        temp_entry->next_node = new_entry; // new_entry is unique to the hash table
        if (bucket_collisions > hash_table->highest_collision_count) {
            hash_table->highest_collision_count = bucket_collisions;
        }
        if (hash_table->highest_collision_count >= COLLISION_LIMIT ) {
            // growTable(HashTable *hash_table);
        }
    }
}

/* Helper function for addEntry. Creates a HashEntry object out of a Data object */
HashEntry *newTableEntry(Data *data_entry) {    

    /* Create and initialize new HashEntry */ 
    HashEntry *new_entry = malloc(sizeof(HashEntry));
    new_entry->data = data_entry;
    new_entry->next_node = NULL;
    new_entry->occurrences = 0;

    return new_entry;
}


