/**
 * Hash table implementation
 *
 * @author Luís Augusto Weber Mercado [lawmercado@inf.ufrgs.br]
 * @author Nicholas de Aquino Lau [nalau@inf.ufrgs.br]
 */


#include "hash.h"
#include "ast.h"
#include "y.tab.h"

HASH_NODE* Table[HASH_SIZE];

void astFind(int level, AST_NODE *node, char *text);
AST_NODE* getAST();

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
    if(type == 1)
    {
        newnode->datatype = DATATYPE_INT;
    } else if (type == 2)
    {
        newnode->datatype = DATATYPE_FLOAT;
    } else if (type == 3)
    {
        newnode->datatype = DATATYPE_CHAR;
    }
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

void hashSetType(char *text, int type)
{
    HASH_NODE *newnode;

    newnode = hashFind(text);
    newnode->type = type;
}

void hashCheckUndeclared(void)
{
    HASH_NODE *node;
    int i = 0;

    for(i = 0; i < HASH_SIZE; i++)
    {
        for(node = Table[i]; node; node = node->next)
        {
            astFind(0, getAST(), node->text);
            if(node->type == SYMBOL_TK_IDENTIFIER)
            {
                fprintf(stderr, "Undeclared variable %s.\n", node->text);
            }
            else if(node->type == SYMBOL_LIT_CHAR || node->type == SYMBOL_LIT_FLOAT || node->type == SYMBOL_LIT_INTEGER)
            {
                node->type = SYMBOL_SCALAR;
            }
        }
    }
}
