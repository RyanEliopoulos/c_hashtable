#include<assert.h>
#include<string.h>
#include"getWord.h"
#include"hash.h"




HashTable *parseFiles(int, int, char *[]);
void processPair(HashTable *, char *, char *);
int entryCompareFunction(HashEntry *, HashEntry *);
void freeData(Data *);
void debug_init();
HashEntry **sortPairs(HashTable *);
int pairCompare(HashEntry *, HashEntry *);
