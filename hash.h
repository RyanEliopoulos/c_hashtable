#include<stdlib.h>
#include<assert.h>
#include<stdio.h>
#include<limits.h>

/* the initial number of buckets available to the hash table */
#define INITIAL_TABLE_SIZE 1000 

/* Threshold of table growth. Once any one bucket reaches 50 collisions the table resizes.  */
/* However, the collision limit may temporarily exceed 50 while a table is growing.         */
#define COLLISION_LIMIT 50// No more than 50 nodes to a linked-list 

/* once the collision limit is reached the table will grow in size by this factor           */
#define GROWTH_FACTOR 5


//*****      REQUIRED USER-SPECIFIED STRUCTURE     *****/

/* the user must organize their data within a Data object with a hash_field attribute in order.  */
/* to be processed by the hash table. Each Data object represents an entry within the table      */ 
/* and is held within a HashEntry object                                                         */
/*************************************************************************************************/
typedef struct _Data {
    char *string1;
    char *string2;
    char *hash_field; // field used to generate the hash
} Data;


/* ---- internal structure                             */
/* A container object used internally by the HashTable */
/*******************************************************/
typedef struct _HashEntry {
    struct _Data *data;           // User-defined data structure for use in the hash table. Must have field: char* hash_field
    struct _HashEntry *next_node; // because we are using separate-chaining
    int occurrences;              // appearance count of a word pair
} HashEntry;



/* ---- internal structure          */
typedef struct _HashTable {
    unsigned long long int total_entries; 
    unsigned long long int unique_entries; /* HashEntry objects in the table. required for table operations */

    unsigned long int total_collisions;
    unsigned int highest_collision_count; /* Collision count of bucket with most collisions  */

    unsigned long long int table_size; /*  Number of buckets */
    int (*entryCompareFnx)(HashEntry *, HashEntry *); // Function used to compare the values of the hash table
    void (*freeDataFnx)(Data *); /* responsible for freeing a Data object and all of its data structures. */
    HashEntry **table_directory; /* The array of buckets */
} HashTable;



/*****           REQUIRED USER-SPECIFIED FUNCTIONS             *****/
/*****                                                         *****/
/*****                                                         *****/

/* The hash table requires the user to provide two functions  */
/* 1). fnxFreeData is used upon program conclusion to release */
/*  all Data objects. The function provides the logic         */
/*  for releasing all sub-structures of struct _Data          */
/*                                                            */
/* 2). entryCompareFnx provides the logic to compare          */
/*  Data objects and determine if they are represent the same */
/*  information. This allows the table to track multiple      */
/*  occurrences of the same information into the table        */


/* a required user-supplied function to facilitate memory management.   */
/* Logic should free the Data object and all substructures              */
/************************************************************************/
typedef void (*fnxFreeData)(Data *);


/* a required user-supplied function used to determine parity between Data objects. */
/* Necessary in order to count multiple occurrences of Data.                        */
/************************************************************************************/
typedef int (*entryCompareFnx)(HashEntry *, HashEntry *);




/*****    INTERFACE FUNCTIONS   *****/
/************************************/


/* ---- Interface function                                                                                            */
/* Initializes a new hash table. Requires two function pointers: One function for deciding parity between Data object */
/* values, obviating the need for duplicate data, and another for freeing a Data object and all of its substructures. */
/**********************************************************************************************************************/
HashTable *newTable(unsigned long long int , entryCompareFnx, fnxFreeData); 


/* ---- Interface function                                                          */
/* Entry point into the hash table code. Packages the user-defined Data object into */ 
/* a table element and submits it to the hash table                                 */
/************************************************************************************/
void tableInsert(HashTable *, Data *);


/* ---- Interface function                                                  */
/* creates a 1D array by iterating over the table buckets and their chains  */
/* Useful for re-hashing a table and qsort()                                */
/****************************************************************************/
HashEntry **unpackTableEntries(HashTable *);


/* ---- Interface function                                      */
/* frees the hash table and all substructures from memory       */
/* this relies on the user-supplied free function to properly   */
/* free all Data substructures                                  */
/****************************************************************/
void releaseTable(HashTable *);




/*****  INTERNAL FUNCTIONS  ****/


/* Insertion logic for the hash table. Does the actual hashing and comparisons                        */
/* distinct from tableInsert() so a table can re-hash already existing HashEntry objects upon re-size */
/* Passing a non-zero int indicates a call from resize, disabling occurrence incrementing             */
/******************************************************************************************************/
void addEntry(int, HashTable *, HashEntry *);

/* resize criteria: maximum per-bucket collision reached by any one bucket          */
/* recursively calls itself if the resized table still violates the collision limit */
/************************************************************************************/
void resizeTable(HashTable *);

/* Helper function for addEntry. Creates a HashEntry object out of a Data object */
/*********************************************************************************/
HashEntry *createHashEntry(Data*);

/* unallocates a HashEntry object and all of its children data structures */
/**************************************************************************/
void freeHashEntry(fnxFreeData, HashEntry *);
