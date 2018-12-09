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
#define SYMBOL_SCALAR 6
#define SYMBOL_VECTOR 7
#define SYMBOL_FUNCTION 8
#define SYMBOL_LABEL 9
#define SYMBOL_TEMP 10

#define DATATYPE_INT 9
#define DATATYPE_CHAR 10
#define DATATYPE_FLOAT 11
#define DATATYPE_TEMP 12

typedef struct hash_node
{
    int type;
    int datatype;
    int isLiteral;
    int assemblyLabel;
    char *text;
    struct hash_node *next;

} HASH_NODE;

HASH_NODE* Table[HASH_SIZE];

void hashInit (void);

int hashAddress(char *text);

HASH_NODE* hashInsert(int type, char *text);

HASH_NODE* hashFind(char *text);

void hashPrint(void);

void hashSetType(char *text, int type);

void hashCheckUndeclared();

HASH_NODE *makeTemp();

HASH_NODE *makeLabel();

#endif
