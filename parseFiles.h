#include<assert.h>
#include"getWord.h"
#include"hash.h"




HashTable *parseFiles(int, int, char *[]);
void processPair(HashTable *, char *, char *);
int entryCompareFunction(HashEntry *, HashEntry *);
void printPairs_DEBUG(char*, char*);
