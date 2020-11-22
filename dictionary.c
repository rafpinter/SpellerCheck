// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dictionary.h"
#include <ctype.h>
#include <strings.h>

int counter = 0;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N  = 65536;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // is this word in the dictionary or not?
    int index_c = hash(word);

    for (node *cursor = table[index_c]; cursor != NULL; cursor = cursor->next)
    {
        if (strcasecmp(word, cursor->word) == 0)
            return true;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: take a word and run a hash function on it, and return some number that corresponds to that word
    // hash function adapted from: https://cs50.stackexchange.com/questions/37209/pset5-speller-hash-function

    unsigned int hash = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
        hash = (hash << 2) ^ tolower(word[i]);
    return hash % N;

}


// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *dic = fopen(dictionary, "r");

    if (dic == NULL)
    {
        return false;
    }
    int end = 0;
    int index;
    char *new_word = malloc((LENGTH + 1) * sizeof(char));

    while (end != EOF)
    {
        end = fscanf(dic, "%s", new_word);
        node *new_node = malloc(sizeof(node));

        if(new_node == NULL)
        {
            return false;
        }

        new_node->next = NULL;
        strcpy(new_node->word, new_word);
        index = hash(new_word);
        counter++;
        new_node->next = table[index];
        table[index] = new_node;
    }
    fclose(dic);
    free(new_word);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size()
{
    // how many words are in the dictionary
    return counter - 1;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // free all the memory that was allocated inside of the data structure
    node *pointer;
    node *tmp;

    for (int i = 0; i < N; i++)
    {
        pointer = table[i];
        tmp = table[i];

        for (; pointer != NULL; tmp = pointer)
        {
            pointer = pointer->next;
            free(tmp);
        }
    }
    return true;
}