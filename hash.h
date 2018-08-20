/**
 * Hash table definitions
 *
 * @author Luís Augusto Weber Mercado [lawmercado@inf.ufrgs.br]
 * @author Nicholas de Aquino Lau [nalau@inf.ufrgs.br]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 997

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
