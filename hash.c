#include "hash.h"

HASH_NODE* Table[HASH_SIZE];

void hashInit (void)
{
    int i;

    for(i = 0; i < HASH_SIZE; ++i)
    {
        Table[i] = 0;
    }
}

int hashAddress(char *text)
{
    int i;
    int address = 1;

    for(i = 0; i < strlen(text); ++i)
    {
        address = (address*text[i])%HASH_SIZE + 1;
    }

    return address - 1;
}

HASH_NODE* hashInsert(int type, char *text)
{
    HASH_NODE *newnode;
    int address;

    address = hashAddress(text);

    if(newnode = hashFind(text))
    {
        return newnode;
    }

    newnode = (HASH_NODE*) calloc(1, sizeof(HASH_NODE));
    newnode->type = type;
    newnode->text = calloc(strlen(text)+1, sizeof(char));
    strcpy(newnode->text, text);
    newnode->next = Table[address];
    Table[address] = newnode;

    return newnode;
}

HASH_NODE* hashFind(char *text)
{
    HASH_NODE *node;
    int address;

    address = hashAddress(text);

    for(node = Table[address]; node; node = node->next)
    {
        if(!strcmp(text, node->text))
        {
            return node;
        }
    }

    return 0;
}

void hashPrint(void)
{
    HASH_NODE *node;
    int i;

    for(i = 0; i < HASH_SIZE; ++i)
    {
        for(node = Table[i]; node; node = node->next)
        {
            fprintf(stderr, "Table[%d] has %s \n", i, node->text);
        }
    }
}
