#include<assert.h>
#include<string.h>
#include"getWord.h"
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
/
int entryCompareFunction(HashEntry *, HashEntry *);
void freeData(Data *);
void debug_init();
HashEntry **sortPairs(HashTable *);
int pairCompare(HashEntry *, HashEntry *);
