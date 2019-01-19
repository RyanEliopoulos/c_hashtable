#include<stdlib.h>

#define INITIAL_TABLE_SIZE 227 // The 50th prime number. definitely not large enough.


typedef struct _HashEntry {
    struct Data *data; // User-defined data structure for use in the hash table
     
    struct _HashEntry *next_node; 
    unsigned long long int occurences; // incremented if identical value is added to the hash table
} HashEntry;


typedef struct _HashTable {
    unsigned long long int total_entries; // May not need this
    unsigned long long int unique_entries; // May not need this

    unsigned long int total_collisions;
    unsigned int highest_collision_count; // Collision count of bucket with most collisions 
    unsigned long long int table_size; // Tracks the table size. Used in the mod part of the hash process
    int (*tableCompareFnx)(HashEntry*, HashEntry*); // Function used to compare the values of the hash table
    struct HashEntry *table_directory[]; 
} HashTable;



HashTable *newTable();
