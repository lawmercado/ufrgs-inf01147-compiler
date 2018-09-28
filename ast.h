#ifndef AST_HEADER
#define AST_HEADER

#include "hash.h"
#include <stdio.h>

#define MAX_SONS 4
#define AST_SYMBOL 1
#define AST_ADD 2
#define AST_SUB 3
#define AST_MUL 4
#define AST_DIV 5
#define AST_LESS 6
#define AST_GREATER 7
#define AST_GE 8
#define AST_LE 9
#define AST_EQ 10
#define AST_AND 11
#define AST_OR 12
#define AST_NOT 13
#define AST_LCMD 14
#define AST_IF 15
#define AST_IFTE 16
#define AST_WHILE 17
#define AST_PRINT 18
#define AST_RETURN 19
#define AST_READ 20


typedef struct ast_node
{
    int type;
    struct ast_node *son[MAX_SONS];
    HASH_NODE *symbol;

} AST;

AST* astCreate(int type, HASH_NODE *symbol, AST *son0, AST *son1, AST *son2, AST *son3);

void astPrint(AST *node, int level);

#endif
