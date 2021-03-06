#include"hash.h"

/* Initializes a new hash table. Requires two function pointers: One function for deciding parity between Data object */
/* values, obviating the need for duplicate data, and another for freeing a Data object and all of its substructures. */
/**********************************************************************************************************************/
HashTable *newTable(unsigned long long int table_size, entryCompareFnx entryCmpr, fnxFreeData freeDataFnx){

    HashTable *new_table = malloc(sizeof(HashTable));
    assert(new_table != NULL);
    
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
    assert(new_table->table_directory != NULL);

    for (int i = 0; i < table_size; i++) {
        new_table->table_directory[i] = NULL;
    }
    return new_table; 
}


/* Insertion logic for the hash table. Does the actual hashing and comparisons                        */
/* distinct from tableInsert() so a table can re-hash already existing HashEntry objects upon re-size */
/* Passing a non-zero int indicates a call from resize, disabling occurrence incrementing             */
/******************************************************************************************************/
void addEntry(int reHash, HashTable *hash_table, HashEntry *new_entry) {

    /* check for possible overflow */
    if (hash_table->unique_entries == ULLONG_MAX) { 
        fprintf(stderr, "entry count exceeds maximum value for ullong\n");
        exit(999);
    }

    unsigned long long int hash = crc64(new_entry->data->hash_field) % hash_table->table_size; 
   
    /* Placing the new_entry into the bucket */ 
    if (hash_table->table_directory[hash] == NULL) { /* Bucket is empty, assign and return */

        hash_table->table_directory[hash] = new_entry;
        if (!reHash) new_entry->occurrences++; 
        hash_table->total_entries++;
        hash_table->unique_entries++;
        return;
    } 

    /* Bucket is not empty */
    /* Will iterate through bucket list, incrementing a matching entry or adding the new unique entry to the end. */
    HashEntry *temp_entry =  hash_table->table_directory[hash];
    unsigned long int bucket_collisions = 1;                   /* inventories the length of a bucket's linked-list */ 
    while ( temp_entry->next_node != NULL ) {

        /* return if a HashEntry match is found */
        if ( (*hash_table->entryCompareFnx)(temp_entry, new_entry) ) {  /* True if entries match. */
            if (!reHash) temp_entry->occurrences++; 

            /* this word pair already exists in the table. this entry is unnecessary */
            freeHashEntry(hash_table->freeDataFnx, new_entry);
            return;
        }
        /* current entry didn't match. Check next one */
        temp_entry = temp_entry->next_node; 
        bucket_collisions++;
    }

    /* Checking the last value in the list */
    if ( (*hash_table->entryCompareFnx)(temp_entry, new_entry) ) {
        if (!reHash) temp_entry->occurrences++;

        /* this word pair already exists in the table. this entry is unnecessary */
        freeHashEntry(hash_table->freeDataFnx, new_entry);
    }

    /* Data object fields don't yet exist within the table                */
    /* add the HashEntry to the table.                                    */
    /* grow the table if the per-bucket collision limit has been exceeded */
    else {
        temp_entry->next_node = new_entry;                              /* new_entry is unique to the hash table */
        if (!reHash) new_entry->occurrences++;
        hash_table->unique_entries++;                                   /* required for table operations */
        if (bucket_collisions > hash_table->highest_collision_count) {  /* update table stats */
            hash_table->highest_collision_count = bucket_collisions;
        }
    }
}

/* resize criteria: maximum per-bucket collision reached by any one bucket          */
/* recursively calls itself if the resized table still violates the collision limit */
/************************************************************************************/
void resizeTable(HashTable *hash_table) {
    //printf("resizing table\n");
    /* retrieve entries for re-hashing */ 
    HashEntry **unpacked_table = unpackTableEntries(hash_table);
    unsigned long long int entry_count = hash_table->unique_entries; /* Length of unpacked_table */

    /* re-size */
    hash_table->highest_collision_count = 0;
    hash_table->unique_entries = 0; /* going to be counted again in addEntry */
    unsigned long long int old_size = hash_table->table_size; /* index for later iteration */
    hash_table->table_size = hash_table->table_size * GROWTH_FACTOR;

    /* manage memory */
    free(hash_table->table_directory);
    hash_table->table_directory = malloc( hash_table->table_size * sizeof(HashEntry *) );
    assert(hash_table->table_directory != NULL);

    /* initialize new directory */
    for (int i = 0; i < hash_table->table_size; i++ ) {
        hash_table->table_directory[i] = NULL;
    }

    /* break outdated linked-lists */
    for (int i = 0; i < entry_count; i++ ) {
        unpacked_table[i]->next_node = NULL;
    }

    /* re-hash existing table elements */
    for ( int i = 0; i < entry_count; i++ ) { 
        addEntry(1, hash_table, unpacked_table[i]);  
    }

    /* No longer need the unpacked table */
    free(unpacked_table); 
    /* But the new table may still have too many collisions */
    if (hash_table->highest_collision_count > COLLISION_LIMIT ) {
        resizeTable(hash_table);
    } 
}

/* Entry point into the hash table code. This should be                                         */
/* packages the user-defined Data object into a table element and submits it to the hash table  */
/************************************************************************************************/
void tableInsert(HashTable *hash_table, Data *data_entry) {
    HashEntry *new_entry = createHashEntry(data_entry);
    addEntry(0, hash_table, new_entry);
    if ( hash_table->highest_collision_count > COLLISION_LIMIT ) {
        resizeTable(hash_table); 
    }
}

/* Helper function for addEntry. Creates a HashEntry object out of a Data object */
/*********************************************************************************/
HashEntry *createHashEntry(Data *data_entry) {    

    /* Create new HashEntry */ 
    HashEntry *new_entry = malloc(sizeof(HashEntry));
    assert(new_entry != NULL);

    /* Initialize new HashEntry */
    new_entry->data = data_entry;
    new_entry->next_node = NULL;
    new_entry->occurrences = 0;

    return new_entry;
}

/* creates a 1D array by iterating over the table buckets and their chains  */
/* Useful for re-hashing a table and qsort()                                */
/****************************************************************************/
HashEntry **unpackTableEntries(HashTable *hash_table) {

    /* Prepare destination of 'flattened' hash table */
    HashEntry **unpacked_array = malloc( hash_table->unique_entries * sizeof(HashEntry *) );
    assert(unpacked_array != NULL);

    /* initialize values */
    for (int i = 0; i < hash_table->unique_entries ; i++) {
        unpacked_array[i] = NULL;
    } 

    /* begining traversal of the hash table */ 
    int j = 0; /* index for the unpacked_array */

    /* check each bucket in the table for HashEntry */ 
    for (int i = 0; i < hash_table->table_size; i++) {
        HashEntry *list_head = hash_table->table_directory[i];

        /* iterate through the list chain, if it exists */
        while (list_head != NULL) {
            unpacked_array[j++] = list_head; 
            list_head = list_head->next_node;
        }        
    }
    return unpacked_array;
}

/* unallocates a HashEntry object and all of its children data structures */
/* For this program that is:                                              */
/*                          HashEntry->Data object                        */
/*                          Data object->string1                          */
/*                          Data object->string2                          */
/*                          Data object->hash_field                       */
/**************************************************************************/
void freeHashEntry(fnxFreeData freeData, HashEntry *hash_entry) {
    freeData(hash_entry->data);
    free(hash_entry);
}


/* frees the hash table and all substructures from memory       */
/* this relies on the user-supplied free function to properly   */
/* free all Data substructures                                  */
/****************************************************************/
void releaseTable(HashTable *hash_table) {

    HashEntry **unpacked_table = unpackTableEntries(hash_table);
    /* HashEntry and sub-structures */
    for (int i = 0; i < hash_table->unique_entries; i++) {
        freeHashEntry(hash_table->freeDataFnx, unpacked_table[i]);
    }

    /* remaining table structures */
    free(hash_table->table_directory);
    free(hash_table);

    /* release locally used  memory */
    free(unpacked_table);
}

// This has confirmed all word pairs are put into the table.
void debug_traverseTable(HashTable *hash_table) {
    
    HashEntry **directory = hash_table->table_directory; 
    for (int i = 0; i < hash_table->table_size; i++) {
        HashEntry *entry = *(directory + i);
        HashEntry *temp; // DEBUG - attempt to free all table elements
        while ( entry != NULL) {
            char *string1 = entry->data->string1;
            char *string2 = entry->data->string2;
            temp = entry;  // DEBUG - used for attempt to free whole table
            entry = entry->next_node;
            freeHashEntry(hash_table->freeDataFnx, temp); // DEBUG - attempting to free al ltable values
        }
    } 
}
