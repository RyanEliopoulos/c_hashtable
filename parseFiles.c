#include"parseFiles.h"
#include"getWord.h"
#include<string.h>

/* Returns a hash table with all word pairs */
HashTable *parseFiles(int count, int argc, char *argv[]) {
    int i = (count == -1) ? 1: 2;  // *filepath[] index

    //HashTable *hash_table = newTable()
    char *word1 = NULL;
    char *word2 = NULL;
    char *temp;

    /* Processing files */
    for ( i; i < argc; i++ ) {
        FILE* current_file = fopen(argv[i], "r");
        assert(current_file != NULL);        
        
        while ((temp = getNextWord(current_file)) != NULL) {
            word1 = word2;
            word2 = temp;
            
            if (word1 != NULL && word2 != NULL) {
                printPairs_DEBUG(word1, word2);
                //processPair(word1, word2); // Packages words into struct _Data and ships it off to a hash table.
            }
        }
    }

    if (word1 == NULL) {
        fprintf(stderr, "Files must include words to analyze\n");
        exit(201);
    } 

    if (word2 == NULL) {
        fprintf(stderr, "Only one word found (and so no pairs)\n");
    }
   
    // return hash_table; 
}


void printPairs_DEBUG(char *word1, char *word2) {

    printf("word1: <%s>\n", word1);
    printf("word2: <%s>\n\n", word2);

}

int entryCompareFunction(HashEntry *entry1, HashEntry *entry2) {
        
    char *en1str1 = entry1->data->string1;
    char *en2str1 = entry2->data->string1;
    if (strcmp(en1str1, en2str1)) return 0;

    char *en1str2 = entry1->data->string2;
    char *en2str2 = entry2->data->string2;
    if (strcmp(en1str2, en2str2)) return 0;

    return 1;
}
