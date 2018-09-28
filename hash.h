#ifndef HASH_HEADER
#define HASH_HEADER
/**
 * Hash table definitions
 *
 * @author Luís Augusto Weber Mercado [lawmercado@inf.ufrgs.br]
 * @author Nicholas de Aquino Lau [nalau@inf.ufrgs.br]
 */
 #include <stdlib.h>
 #include <stdio.h>
 #include <string.h>

#define HASH_SIZE 997
#define SYMBOL_LIT_INTEGER 1
#define SYMBOL_LIT_FLOAT 2
#define SYMBOL_LIT_CHAR 3
#define SYMBOL_LIT_STRING 4
#define SYMBOL_TK_IDENTIFIER 5

typedef struct hash_node
{
    int type;
    char *text;
    struct hash_node *next;

} HASH_NODE;

void hashInit (void);

int hashAddress(char *text);

HASH_NODE* hashInsert(int type, char *text);

HASH_NODE* hashFind(char *text);

void hashPrint(void);

#endif
