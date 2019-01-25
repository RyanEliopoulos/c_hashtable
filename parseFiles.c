#include"parseFiles.h"


/* Returns a hash table with all word pairs filed       */
/* This function controls reading words from the files  */
/* and sending them to be processed into the hash table */
/* 2XX errors are thrown if there aren't at least two   */ 
/* words across all of the given files                  */
HashTable *parseFiles(int count, int argc, char *argv[]) {

    int i = (count == -1) ? 1: 2;  /* argv index of first filepath */
    HashTable *hash_table = newTable(INITIAL_TABLE_SIZE, &entryCompareFunction, &freeData);

    /* variables for word reading */
    char *word1 = NULL;
    char *word2 = NULL;
    char *temp = "x"; /* can't be null for below logic. Arbitrarily chosen as 'x' */

    /* Processing files */
    int j = 0; // Facilitates fix: 'getNextWord' character array leak. 
    for ( i; i < argc; i++ ) {
        FILE* current_file = fopen(argv[i], "r");
        assert(current_file != NULL);        

        /* Terminates when no more words exist in the current file */
        while ((temp = getNextWord(current_file)) != NULL) {
            word1 = word2;
            word2 = temp;
            j++; // Facilitates fix of 'getNextWord' mem leak

            if (word1 != NULL && word2 != NULL) {
                processPair(hash_table, word1, word2); // Packages words into struct _Data and ships it off to a hash table.
            }
            /* realized late that getNextWord allocates heap memory. This is a fix */
            if ( word1 != NULL) {
                free(word1);
                word1 = NULL;
            }
        }
        fclose(current_file); 
    }
    /* continuation of getNextWord fix */
    if ( word2 != NULL) {
        free(word2);
        word2 = NULL;
    }

    if ( j == 0 ) {
        fprintf(stderr, "Files must include words to analyze\n");
        exit(201);
    } 

    if ( j == 1) {
        fprintf(stderr, "Only one word found (and so no pairs)\n");
        exit(202);
    }
    return hash_table;
}


/* Packages the words into a new Data struct and inserts into the hash table via tableInsert() */
void processPair(HashTable *hash_table, char *word1, char *word2) {

    /* creating top-level structure */
    Data *new_data = malloc(sizeof(Data)); 
    assert(new_data != NULL);

    /* creating structure elements */
    new_data->string1 = malloc(DICT_MAX_WORD_LEN * sizeof(char));
    assert(new_data->string1 != NULL);
    new_data->string2 = malloc(DICT_MAX_WORD_LEN * sizeof(char));
    assert(new_data->string2 != NULL);

    /* load words into the structure */
    strcpy(new_data->string1, word1);
    strcpy(new_data->string2, word2);
   
    /* combine the strings for hashing */
    new_data->hash_field = malloc((DICT_MAX_WORD_LEN * 2) * sizeof(char)); 
    strcpy(new_data->hash_field, word1);
    strcat(new_data->hash_field, word2);
    
    /* send it of for processing by the hash table */
    tableInsert(hash_table, new_data);  
}

/* This function is passed to the hash table upon creation so it can                */
/* increment word pairs already in the table instead of inserting duplicate pairs   */
/*                                                                                  */ 
/* returns 1 if the string values are commensurate                                  */
int entryCompareFunction(HashEntry *entry1, HashEntry *entry2) {
       
    /* checking string1 */ 
    char *en1str1 = entry1->data->string1;
    char *en2str1 = entry2->data->string1;
    if (strcmp(en1str1, en2str1)) return 0;

    /* checking string2 */
    char *en1str2 = entry1->data->string2;
    char *en2str2 = entry2->data->string2;
    if (strcmp(en1str2, en2str2)) return 0;

    return 1; /* Both strings matched. Word pair will be incremented */
}

/* fnx passed to the hash table to facilitate memory management         */
/* unallocates memory for a Data object and all its malloc'd structures */
void freeData(Data *data) {
    free(data->string1);
    free(data->string2);
    free(data->hash_field);
    free(data);
}

/* a wrapper for qsort() */
HashEntry **sortPairs(HashTable *hash_table) {

    HashEntry **unpacked_array = unpackTableEntries(hash_table);
    qsort((void *)unpacked_array, hash_table->unique_entries, sizeof(HashTable *), comparator);

    return unpacked_array; /* sorted */
}

/* to be used with qsort() to sort the word pairs by count */
int comparator(const void *e1, const void *e2) {

    HashEntry **entry1 = (HashEntry **)e1;
    HashEntry **entry2 = (HashEntry **)e2;

    if ((*entry1)->occurrences > (*entry2)->occurrences) {
        return -1;
    }

    if ((*entry1)->occurrences == (*entry2)->occurrences) {
        return 0;
    }
    return 1;
}


