// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 1;

// Hash table
node *table[N];

//for counting in load function and returning in size func
unsigned int dict_size;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    //hash word to obtain a hash value
    unsigned int hash_value = hash(word);

    //access linked list at that index an the hash table
    //start with cursor set to first item in linked list
    node *cursor = table[hash_value];

    //traverse linked list looking for the word (strcasecmp)
    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        else
        {
            cursor = cursor->next;
        }
    }
    return false;
}

// Hashes word to a number

//hash option2
// unsigned int hash(const char *word)
// {
//     // Function should take a string and return an index
//     // This hash function adds the ASCII values of all characters in the word together
//     long sum = 0;
//     for (int i = 0; i < strlen(word); i++)
//     {
//         sum += tolower(word[i]);
//     }
//     return sum % N;
// }


unsigned int hash(const char *word)
{
    // TODO function takes a string and returns an index
    //hash with djb2 algorithm

    unsigned long hash = 5381;

    for (const char *ptr = word; *ptr != '\0'; ptr++)
    {
        hash = ((hash << 5) + hash) + tolower(*ptr);
    }

    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    //Open dictionary file
    FILE *dict_file = fopen(dictionary, "r");
    if (dict_file == NULL)
    {
        printf("Error opening dictionary\n");
        return false;
    }

    //Read strings from file one at a time
    char dword [LENGTH + 1];
    while (fscanf(dict_file, "%s", dword) != EOF)
    {
        //create a new node for each word
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            return false;
        }
        //copy a word into node using strcpy
        strcpy(new_node->word, dword);

        //hash word to abtain a hash value - use hash function which takes string and returns an index
        unsigned int hash_value = hash(new_node->word);

        //insert node into hash table at that location
        //if not the first element then set new_nodes *next pointer to the first in a linked list
        //and ONLY THEN can set the head to the new_node i just created
        if (table[hash_value] == NULL)
        {
            new_node->next = NULL;
            table[hash_value] = new_node;
        }
        else
        {
            new_node->next = table[hash_value];
            table[hash_value] = new_node;
        }

        dict_size++;
    }
    // Close file
    fclose(dict_file);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    //keep track of loaded words in a load function and return it here
    return dict_size;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    // go to hash table and free every  linked lists node after node
    // int hash_value = 0;


    for (int i = 0; i < N; i++)
    {
        //assign cursor
        node *cursor = table[i];

        while (cursor != NULL)
        {
            //make tmp
            node *tmp = cursor;
            //point cursor to next element
            cursor = cursor->next;
            free(tmp);

        }

        if (cursor == NULL && i == N - 1)
        {
            return true;
        }
    }

    return false;
}
