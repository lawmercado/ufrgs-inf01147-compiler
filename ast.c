#include "ast.h"

AST_NODE* astCreate(int type, HASH_NODE *symbol, AST_NODE *son0, AST_NODE *son1, AST_NODE *son2, AST_NODE *son3)
{
    AST_NODE *newnode;
    newnode = (AST_NODE*) calloc(1,sizeof(AST_NODE));
    newnode->type = type;
    newnode->symbol = symbol;
    newnode->son[0] = son0;
    newnode->son[1] = son1;
    newnode->son[2] = son2;
    newnode->son[3] = son3;
    return newnode;
}

void astPrint(AST_NODE *node, int level)
{
    int i;
    if (node == 0)
    {
        return;
    }

    for (i = 0; i < level; i++)
    {
        fprintf(stderr, "|   ");
    }

    fprintf(stderr, "|");

    switch (node->type)
    {
        case AST_DEC: fprintf(stderr, "AST_DEC, "); break;
        case AST_INT_DEF: fprintf(stderr, "AST_INT_DEF, "); break;
        case AST_FLOAT_DEF: fprintf(stderr, "AST_FLOAT_DEF, "); break;
        case AST_CHAR_DEF: fprintf(stderr, "AST_CHAR_DEF, "); break;
        case AST_VAR_DEC: fprintf(stderr, "AST_VAR_DEC, "); break;
        case AST_VEC_DEC: fprintf(stderr, "AST_VEC_DEC, "); break;
        case AST_FUNC_DEC: fprintf(stderr, "AST_FUNC_DEC, "); break;
        case AST_FUNC_CALL: fprintf(stderr, "AST_FUNC_CALL, "); break;
        case AST_PARAM_LIST: fprintf(stderr, "AST_PARAM_LIST, "); break;
        case AST_LIT_LIST: fprintf(stderr, "AST_LIT_LIST, "); break;
        case AST_VEC_PARAM: fprintf(stderr, "AST_VEC_PARAM, "); break;
        case AST_CMD_LIST: fprintf(stderr, "AST_CMD_LIST, "); break;
        case AST_IF: fprintf(stderr, "AST_IF, "); break;
        case AST_IFELSE: fprintf(stderr, "AST_IFELSE, "); break;
        case AST_WHILE: fprintf(stderr, "AST_WHILE, "); break;
        case AST_PRINT: fprintf(stderr, "AST_PRINT, "); break;
        case AST_RETURN: fprintf(stderr, "AST_RETURN, "); break;
        case AST_READ: fprintf(stderr, "AST_READ, "); break;
        case AST_SYMBOL: fprintf(stderr, "AST_SYMBOL, "); break;
        case AST_ADD: fprintf(stderr, "AST_ADD, "); break;
        case AST_SUB: fprintf(stderr, "AST_SUB, "); break;
        case AST_MUL: fprintf(stderr, "AST_MUL, "); break;
        case AST_DIV: fprintf(stderr, "AST_DIV, "); break;
        case AST_LESS: fprintf(stderr, "AST_LESS, "); break;
        case AST_GREATER: fprintf(stderr, "AST_GREATER, "); break;
        case AST_GEQUAL: fprintf(stderr, "AST_GEQUAL, "); break;
        case AST_LEQUAL: fprintf(stderr, "AST_LEQUAL, "); break;
        case AST_EQUAL: fprintf(stderr, "AST_EQUAL, "); break;
        case AST_AND: fprintf(stderr, "AST_AND, "); break;
        case AST_OR: fprintf(stderr, "AST_OR, "); break;
        case AST_NOT: fprintf(stderr, "AST_NOT, "); break;
        case AST_VEC: fprintf(stderr, "AST_VEC, "); break;
        case AST_ATTRIB: fprintf(stderr, "AST_ATTRIB, "); break;
        case AST_VEC_ATTRIB: fprintf(stderr, "AST_VEC_ATTRIB, "); break;
        case AST_BLK: fprintf(stderr, "AST_BLK, "); break;
        case AST_DB: fprintf(stderr, "AST_DB, "); break;
        default: fprintf(stderr, "AST_UNKNOWN, "); break;
    }

    if (node->symbol != 0)
    {
        switch(node->symbol->type)
        {
            case SYMBOL_LIT_INTEGER: fprintf(stderr, "(int) "); break;
            case SYMBOL_LIT_FLOAT: fprintf(stderr, "(float) "); break;
            case SYMBOL_LIT_CHAR: fprintf(stderr, "(char) "); break;
            case SYMBOL_LIT_STRING: fprintf(stderr, "(str) "); break;
            case SYMBOL_TK_IDENTIFIER: fprintf(stderr, "(id) "); break;
        }

        fprintf(stderr, "%s\n", node->symbol->text);
    }
    else
    {
        fprintf(stderr, "<no symbol>\n");
    }

    for (i = 0; i < MAX_SONS; i++)
    {
        astPrint(node->son[i], level+1);
    }

    // for (i = 0; i < level; i++)
    // {
    //     fprintf(stderr, "   ");
    // }
    //
    // fprintf(stderr, ");\n");
}
