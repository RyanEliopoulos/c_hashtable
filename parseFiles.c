/* Trying to get the hash_field into _Data.  */


#include"parseFiles.h"
#include"getWord.h"
#include<string.h>

/* Returns a hash table with all word pairs */
HashTable *parseFiles(int count, int argc, char *argv[]) {
    int i = (count == -1) ? 1: 2;  // *filepath[] index

    HashTable *hash_table = newTable();
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
}


/* Packages the words into a new Data struct and inserts into the hash table via PLACEHOLDER() */
void processPair(HashTable *hash_table, char *word1, char *word2) {

    Data *new_data = malloc(sizeof(Data)); 
    assert(new_data != NULL);
    
    
    /* load words into the structure */
    strcpy(new_data->string1, word1);
    strcpy(new_data->string2, word2);
    //new_data->string1 = word1; // Does this actually have the strings persist? I don't think so..
    //new_data->string2 = word2;   

    /* build string used for hashing */
    char *hash_field  = malloc( (DICT_MAX_WORD_LEN * 2) * sizeof(*hash_field) );
    strcpy(hash_field, word1);   
    strcat(hash_field, word2); 
    new_data->hash_field = hash_field;    
    
    //addEntry(hash_table, new_data); Have not begun testing at this point yet.
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

void printPairs_DEBUG(char *word1, char *word2) {

    printf("word1: <%s>\n", word1);
    printf("word2: <%s>\n\n", word2);

}

