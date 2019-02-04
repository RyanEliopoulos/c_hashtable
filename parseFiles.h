#include<stdio.h>
#include<assert.h>
#include<string.h>
#include<stdlib.h>
#include"hash.h"



/* Returns a hash table with all word pairs filed       */
/* This function controls reading words from the files  */
/* and sending them to be processed into the hash table */
/* 2XX errors are thrown if there aren't at least two   */ 
/* words across all of the given files                  */
HashTable *parseFiles(int, int, char *[]);

/* Packages the words into a new Data struct and inserts into the hash table via tableInsert() */
void processPair(HashTable *, char *, char *);

/* This function is passed to the hash table upon creation so it can                */
/* increment word pairs already in the table instead of inserting duplicate pairs   */
/*                                                                                  */ 
/* returns 1 if the string values are commensurate                                  */
int entryCompareFunction(HashEntry *, HashEntry *);

/* fnx passed to the hash table to facilitate memory management         */
/* unallocates memory for a Data object and all its malloc'd structures */
void freeData(Data *);

/* a wrapper for qsort() */
HashEntry **sortPairs(HashTable *);

/* to be used with qsort() to sort the word pairs by count */
int comparator(const void *, const void *); 
