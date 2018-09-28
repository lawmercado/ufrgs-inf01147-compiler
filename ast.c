#include "ast.h"

AST*astCreate(int type, HASH_NODE *symbol, AST *son0, AST *son1, AST *son2, AST *son3)
{
    AST *newnode;
    newnode = (AST*) calloc(1,sizeof(AST));
    newnode->type = type;
    newnode->symbol = symbol;
    newnode->son[0] = son0;
    newnode->son[1] = son1;
    newnode->son[2] = son2;
    newnode->son[3] = son3;
    return newnode;
}

void astPrint(AST *node, int level)
{
    int i;
    if (node == 0)
    {
        return;
    }

    for (i = 0; i < level; i++)
    {
        fprintf(stderr, "   ");
    }

    fprintf(stderr, "AST(");

    switch (node->type)
    {
        case AST_SYMBOL: fprintf(stderr, "AST_SYMBOL, "); break;
        case AST_ADD: fprintf(stderr, "AST_ADD, "); break;
        case AST_SUB: fprintf(stderr, "AST_SUB, "); break;
        case AST_MUL: fprintf(stderr, "AST_MUL, "); break;
        case AST_DIV: fprintf(stderr, "AST_DIV, "); break;
        case AST_LESS: fprintf(stderr, "AST_LESS, "); break;
        case AST_GREATER: fprintf(stderr, "AST_GREATER, "); break;
        case AST_GE: fprintf(stderr, "AST_GE, "); break;
        case AST_LE: fprintf(stderr, "AST_LE, "); break;
        case AST_EQ: fprintf(stderr, "AST_EQ, "); break;
        case AST_AND: fprintf(stderr, "AST_AND, "); break;
        case AST_OR: fprintf(stderr, "AST_OR, "); break;
        case AST_NOT: fprintf(stderr, "AST_NOT, "); break;
        case AST_LCMD: fprintf(stderr, "AST_LCMD, "); break;
        case AST_IF: fprintf(stderr, "AST_IF, "); break;
        case AST_IFTE: fprintf(stderr, "AST_IFTE, "); break;
        case AST_WHILE: fprintf(stderr, "AST_WHILE, "); break;
        case AST_PRINT: fprintf(stderr, "AST_PRINT, "); break;
        case AST_RETURN: fprintf(stderr, "AST_RETURN, "); break;
        case AST_READ: fprintf(stderr, "AST_READ, "); break;
        default: fprintf(stderr, "AST_UNKNOWN, "); break;
    }

    if (node->symbol != 0)
    {
        fprintf(stderr, "%s\n", node->symbol->text);
    }
    else
    {
        fprintf(stderr, "0, \n");
    }

    for (i = 0; i < MAX_SONS; i++)
    {
        astPrint(node->son[i], level+1);
    }

    fprintf(stderr, "    );\n");
}
