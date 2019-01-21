/* Trying to get the hash_field into _Data.  */

#include"parseFiles.h"


/* Returns a hash table with all word pairs */
HashTable *parseFiles(int count, int argc, char *argv[]) {

    int i = (count == -1) ? 1: 2;  // *filepath[] index

    HashTable *hash_table = newTable(INITIAL_TABLE_SIZE, &entryCompareFunction, &freeData);
    char *word1 = NULL;
    char *word2 = NULL;
    char *temp = "x";

    /* Processing files */
    int j = 0; // Facilitates fix: 'getNextWord' character array leak
    for ( i; i < argc; i++ ) {
        FILE* current_file = fopen(argv[i], "r");
        assert(current_file != NULL);        
        while ((temp = getNextWord(current_file)) != NULL) {
            word1 = word2;
            word2 = temp;
            j++; // Facilitate fix of 'getNextWord' mem leak
            if (word1 != NULL && word2 != NULL) {
                processPair(hash_table, word1, word2); // Packages words into struct _Data and ships it off to a hash table.
            }
            /* realized late that getNextWord allocates heap memory. This is a fix */
            if ( word1 != NULL) {
                free(word1);
                word1 = NULL;
            }
        }
        /* continuation of getNextWord fix */
        if ( word2 != NULL) {
            free(word2);
            word2 = NULL;
        }
        fclose(current_file); 
    }

    if ( j == 0 ) {
        fprintf(stderr, "Files must include words to analyze\n");
        exit(201);
    } 

    if ( j == 1) {
        fprintf(stderr, "Only one word found (and so no pairs)\n");
    }
    return hash_table;
}


/* Packages the words into a new Data struct and inserts into the hash table via PLACEHOLDER() */
void processPair(HashTable *hash_table, char *word1, char *word2) {

    //printf("allocating space for strings in new Data object\n");
    Data *new_data = malloc(sizeof(Data)); 
    new_data->string1 = malloc( DICT_MAX_WORD_LEN * sizeof(char) );
    new_data->string2 = malloc( DICT_MAX_WORD_LEN * sizeof(char) );

    assert(new_data != NULL);
    assert(new_data->string1 != NULL);
    assert(new_data->string2 != NULL);
    

    //printf("copying word pair into the structure\n");
    /* load words into the structure */
    strcpy(new_data->string1, word1);
    strcpy(new_data->string2, word2);
   
     
    /* build string used for hashing */
    new_data->hash_field = malloc( (DICT_MAX_WORD_LEN * 2) * sizeof(char)); 
    strcpy(new_data->hash_field, word1);
    strcat(new_data->hash_field, word2);
    
    //printf("Adding <%s> to the hash table\n", new_data->hash_field);
    // DEBUGGING so turning the hash table off

    /* free's and allocs match until we start inserting into the table */
    //freeData(new_data);
    tableInsert(hash_table, new_data);  // Testing now

    // DEBUGGING STUFF HERE
    //debug_array[debug_counter++] = new_data;
   // printf("highest collisions:%u\n", hash_table->highest_collision_count);
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

/* unallocate memory for a Data object and all its malloc'd structures */
void freeData(Data *data) {
    free(data->string1);
    free(data->string2);
    free(data->hash_field);
    free(data);
}


