#ifndef AST_HEADER
#define AST_HEADER

#include "hash.h"
#include <stdio.h>

#define MAX_SONS 4

enum AST_NODE_TYPE {
    AST_DEC,
    AST_INT_DEF,
    AST_FLOAT_DEF,
    AST_CHAR_DEF,
    AST_VAR_DEC,
    AST_VEC_DEC,
    AST_FUNC_DEC,
    AST_PARAM_LIST,
    AST_ARG_LIST,
    AST_VEC_PARAM,
    AST_LIT_LIST,
    AST_CMD_LIST,
    AST_IF,
    AST_IFELSE,
    AST_WHILE,
    AST_PRINT,
    AST_RETURN,
    AST_READ,
    AST_SYMBOL,
    AST_ADD,
    AST_SUB,
    AST_MUL,
    AST_DIV,
    AST_LESS,
    AST_GREATER,
    AST_GEQUAL,
    AST_LEQUAL,
    AST_EQUAL,
    AST_AND,
    AST_OR,
    AST_NOT,
    AST_VEC,
    AST_FUNC_CALL,
    AST_ATTRIB,
    AST_VEC_ATTRIB,
    AST_BLK,
    AST_DB,
    AST_DATATYPE_INT,
    AST_DATATYPE_FLOAT,
    AST_DATATYPE_BOOL,
};

typedef struct ast_node
{
    int type;
    int datatype;
    struct ast_node *son[MAX_SONS];
    HASH_NODE *symbol;

} AST_NODE;

AST_NODE* astCreate(int type, HASH_NODE *symbol, AST_NODE *son0, AST_NODE *son1, AST_NODE *son2, AST_NODE *son3);

void astPrint(AST_NODE *node, int level);

void astGenerateSource(AST_NODE *node, FILE *file);

void astFind(int level, AST_NODE *node, char *text);

AST_NODE *astNodeFinder(int level, AST_NODE *node, AST_NODE **son2, char *text, int *flagFound);

#endif
