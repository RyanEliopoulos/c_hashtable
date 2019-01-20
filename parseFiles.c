/* Trying to get the hash_field into _Data.  */

#include<string.h>
#include"parseFiles.h"
#include"getWord.h"

// Debug stuff
Data *debug_array[1000];
int debug_counter = 0;

/* Returns a hash table with all word pairs */
HashTable *parseFiles(int count, int argc, char *argv[]) {

    // DEBUG STATEMENT!
    debug_init(); 
    // END DEBUG STATEMENT

    int i = (count == -1) ? 1: 2;  // *filepath[] index

    HashTable *hash_table = newTable(&entryCompareFunction);
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
                //printPairs_DEBUG(word1, word2);
                processPair(hash_table, word1, word2); // Packages words into struct _Data and ships it off to a hash table.
            }
        }
    }

    if (word2 == NULL) {
        fprintf(stderr, "Files must include words to analyze\n");
        exit(201);
    } 

    if (word1 == NULL) {
        fprintf(stderr, "Only one word found (and so no pairs)\n");
    }
  
    // return hash_table; 

    // DEBUGGING STUFF HERE
    for (int i = 0; i < 1000; i++) {
        Data *temp = debug_array[i];
        if (temp == NULL) break;
        printf("Word1: <%s>, word2: <%s>, hash_field: <%s>\n", temp->string1, temp->string2, temp->hash_field); 
    } 
}


/* Packages the words into a new Data struct and inserts into the hash table via PLACEHOLDER() */
void processPair(HashTable *hash_table, char *word1, char *word2) {

    Data *new_data = malloc(sizeof(Data)); 
    assert(new_data != NULL);

    printf("allocating space for strings in new Data object\n");
    new_data->string1 = malloc( DICT_MAX_WORD_LEN * sizeof(char) );
    new_data->string2 = malloc( DICT_MAX_WORD_LEN * sizeof(char) );

    printf("copying word pair into the structure\n");
    /* load words into the structure */
    strcpy(new_data->string1, word1);
    strcpy(new_data->string2, word2);
   
     
    /* build string used for hashing */
    new_data->hash_field = malloc( (DICT_MAX_WORD_LEN * 2) * sizeof(char)); 
    strcpy(new_data->hash_field, word1);
    strcat(new_data->hash_field, word2);
    
    printf("Adding <%s> to the hash table\n", new_data->hash_field);
    addEntry(hash_table, new_data);  // Testing now
    printf("finished adding to has table\n");

    // DEBUGGING STUFF HERE
    debug_array[debug_counter++] = new_data;
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

void debug_init() {
    for (int i = 0; i < 1000; i++) debug_array[i] = NULL;
}
void printPairs_DEBUG(char *word1, char *word2) {

    printf("word1: <%s>\n", word1);
    printf("word2: <%s>\n\n", word2);

}

