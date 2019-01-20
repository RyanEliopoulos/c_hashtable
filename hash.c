/* Going to want a hash function that unpacks all HashEntry nodes into a linear array. */

/* Will then use this array in qsort(). */
/* Will also want a function that unpacks all of the Data pointers into an array so that the table controller  */
/* can can free the relevant Data fields. This unpacking function will free the HashEntry nodes */
#include<stdio.h>
#include"hash.h"

/* Initializes a new hash table. Requires pointer to a function that will be used to compare Data objects. */
/* Internally the table will increment a relevant counter if an identical Data object is inserted into the table */
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
        new_table->table_directory[i] = NULL;
    }
    return new_table; 
}


/* Handles inserting new Data into the hash table. */
/* Need to add: state tracking */
void addEntry(HashTable *hash_table, Data *data_entry) {

    unsigned long long int hash = crc64(data_entry->hash_field) % hash_table->table_size; // Calculating hash value
    //printf("successfully hashed\n");    
    HashEntry *new_entry = newTableEntry(data_entry); // Creating an entry node for the hash table
   
    /* Placing the new_entry into the bucket */ 
    if (hash_table->table_directory[hash] == NULL) { // Bucket is empty, assign and return
        hash_table->table_directory[hash] = new_entry;
        //printf("added new entry to table\n");
        hash_table->total_entries++;

        // DEBEUG STUFF. Just immediately removes the entry. Prevents chaining
        //printf("Now debugging freeEntry(). SO undoing that\n");
        //hash_table->table_directory[hash] = NULL;
        //freeHashEntry(hash_table->freeDataFnx, new_entry);
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

    /* Create new HashEntry */ 
    HashEntry *new_entry = malloc(sizeof(HashEntry));
    assert(new_entry != NULL);

    /* Initialize new HashEntry */
    new_entry->data = data_entry;
    new_entry->next_node = NULL;
    new_entry->occurrences = 0;

    return new_entry;
}

/* Puts all hash entries into a HashEntry array */
/* Useful for re-hashing a table and qsort() */

/* link traversing may still be broken. We are missing one entry free and its the chained value */
/* LOSING THE CHAIN IN HERE SOMEHOW! */
HashEntry **unpackTableEntries(HashTable *hash_table) {

    /* Initialize pointers for an 'array' of HashEntry pointers */ 
    HashEntry **unpacked_array = malloc( hash_table->table_size * sizeof(HashEntry *) );
    for (int i = 0; i < hash_table->table_size; i++) {
        *(unpacked_array + i) = NULL;
    } 
    int j = 0; /* next free slot in entry_array */

    /* check each bucket for entries */ 
    for (int i = 0; i < hash_table->table_size; i++) {
        /* de-refence the hash table directory */
        HashEntry *list_head = hash_table->table_directory[i];//(*hash_table->table_directory + i);
    
        /* This while loop is whats causing the segfaults */
        while (list_head != NULL) {
            unpacked_array[j++] = list_head;
            list_head = list_head->next_node;
        }        
    }
    return unpacked_array;
}

/* unallocates a HashEntry object and all of its children data structures */
void freeHashEntry(fnxFreeData freeData, HashEntry *hash_entry) {
    freeData(hash_entry->data);
    free(hash_entry);
}

// Also need to add logic to traverse a list, if one is encountered
// This has confirmed all word pairs are put into the table.
void debug_traverseTable(HashTable *hash_table) {
    
    HashEntry **directory = hash_table->table_directory; 
    for (int i = 0; i < hash_table->table_size; i++) {
        HashEntry *entry = *(directory + i);
        HashEntry *temp; // DEBUG - attempt to free all table elements
        while ( entry != NULL) {
            char *string1 = entry->data->string1;
            char *string2 = entry->data->string2;
            printf("<%s>, <%s>\n", string1, string2);
            temp = entry;  // DEBUG - used for attempt to free whole table
            entry = entry->next_node;
            freeHashEntry(hash_table->freeDataFnx, temp); // DEBUG - attempting to free al ltable values
        }
    } 
}
