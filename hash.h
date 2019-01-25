#include<stdlib.h>
#include<assert.h>
#include"crc64.h"

#define INITIAL_TABLE_SIZE 277//227 // The 50th prime number. definitely not large enough.
#define COLLISION_LIMIT 50
#define GROWTH_FACTOR 3

typedef struct _Data {
    char *string1;
    char *string2;
    char *hash_field; // Value used in the hash function.
} Data;


typedef struct _HashEntry {
    struct _Data *data; // User-defined data structure for use in the hash table. Must have field: char* hash_field
    struct _HashEntry *next_node; 
    int occurrences; // incremented if identical value is added to the hash table
} HashEntry;


typedef struct _HashTable {
    unsigned long long int total_entries; // May not need this
    unsigned long long int unique_entries; // May not need this

    unsigned long int total_collisions;
    unsigned int highest_collision_count; // Collision count of bucket with most collisions 
    unsigned long long int table_size; // Tracks the table size. Used in the mod part of the hash process
    int (*entryCompareFnx)(HashEntry *, HashEntry *); // Function used to compare the values of the hash table
    void (*freeDataFnx)(Data *); /* responsible for free-ing all a Data object and all of its data structures. */
    HashEntry **table_directory; 
} HashTable;

typedef void (*fnxFreeData)(Data *);
typedef int (*entryCompareFnx)(HashEntry *, HashEntry *);


HashTable *newTable(unsigned long long int , entryCompareFnx, fnxFreeData); 
void addEntry(HashTable *, HashEntry *);
void resizeTable(HashTable *);
void tableInsert(HashTable *, Data *);
HashEntry *createHashEntry(Data*);
HashEntry **unpackTableEntries(HashTable *);
//void freeTable(HashTable *);
void freeHashEntry(fnxFreeData, HashEntry *);
void releaseTable(HashTable *);
void debug_traverseTable(HashTable *);
