#include "ast.h"
#include "y.tab.h"

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
        fprintf(stderr, "|  ");
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
}

void astGenerateSource(AST_NODE *node, FILE *file)
{
    int i;

    static int level = 0;

    if(node == 0)
    {
        return;
    }

    switch (node->type)
    {
        case AST_DEC:
        {
            for(i = 0; i < MAX_SONS; i++)
            {
                astGenerateSource(node->son[i], file);
            }

            break;
        }
        case AST_VAR_DEC:
        {
            astGenerateSource(node->son[0], file);

            fprintf(file, " = ");

            astGenerateSource(node->son[1], file);

            fprintf(file, ";\n");

            break;
        }
        case AST_VEC_DEC:
        {
            astGenerateSource(node->son[0], file);

            fprintf(file, " q%sp", node->son[1]->symbol->text);

            if(node->son[2] != 0)
            {
                fprintf(file, ": ");

                astGenerateSource(node->son[2], file);
            }

            fprintf(file, ";\n");

            break;
        }
        case AST_FUNC_DEC:
        {
            astGenerateSource(node->son[0], file);

            fprintf(file, " d ");

            astGenerateSource(node->son[1], file);

            fprintf(file, " b\n");

            astGenerateSource(node->son[2], file);

            fprintf(file, "\n");

            break;
        }
        case AST_PARAM_LIST:
        {
            if(node->son[1] != 0)
            {
                if(node->son[1]->type == AST_PARAM_LIST)
                {
                    astGenerateSource(node->son[1], file);

                    fprintf(file, ", ");
                }
            }

            astGenerateSource(node->son[0], file);

            break;
        }
        case AST_LIT_LIST:
        {
            if(node->son[1] != 0)
            {
                astGenerateSource(node->son[1], file);

                fprintf(file, " ");
            }

            astGenerateSource(node->son[0], file);

            break;
        }
        case AST_VEC_PARAM:
        {
            astGenerateSource(node->son[0], file);
            fprintf(file, " q%sp", node->son[1]->symbol->text);

            break;
        }
        case AST_CMD_LIST:
        {
            astGenerateSource(node->son[0], file);
            fprintf(file, ";\n");
            astGenerateSource(node->son[1], file);

            break;
        }
        case AST_IF:
        {
            for (i = 0; i < level; i++) fprintf(file, "    ");
            fprintf(file, "if ");
            astGenerateSource(node->son[0], file);
            fprintf(file, " then\n");
            for (i = 0; i < level; i++) fprintf(file, "    ");
            astGenerateSource(node->son[1], file);

            break;
        }
        case AST_IFELSE:
        {
            for (i = 0; i < level; i++) fprintf(file, "    ");
            fprintf(file, "if ");
            astGenerateSource(node->son[0], file);
            fprintf(file, " then\n");
            for (i = 0; i < level; i++) fprintf(file, "    ");
            astGenerateSource(node->son[1], file);
            fprintf(file, "\n");
            for (i = 0; i < level; i++) fprintf(file, "    ");
            fprintf(file, "else\n");
            for (i = 0; i < level; i++) fprintf(file, "    ");
            astGenerateSource(node->son[2], file);

            break;
        }
        case AST_WHILE:
        {
            for (i = 0; i < level; i++) fprintf(file, "    ");
            fprintf(file, "while ");
            astGenerateSource(node->son[0], file);
            fprintf(file, "\n");
            for (i = 0; i < level; i++) fprintf(file, "    ");
            astGenerateSource(node->son[1], file);

            break;
        }
        case AST_PRINT:
        {
            for (i = 0; i < level; i++) fprintf(file, "    ");
            fprintf(file, "print ");
            astGenerateSource(node->son[0], file);

            break;
        }
        case AST_RETURN:
        {
            for (i = 0; i < level; i++) fprintf(file, "    ");
            fprintf(file, "return ");
            astGenerateSource(node->son[0], file);

            break;
        }
        case AST_READ:
        {
            for (i = 0; i < level; i++) fprintf(file, "    ");
            fprintf(file, "read ");
            astGenerateSource(node->son[0], file);

            break;
        }
        case AST_ATTRIB:
        {
            for (i = 0; i < level; i++) fprintf(file, "    ");
            astGenerateSource(node->son[0], file);
            fprintf(file, " = ");
            astGenerateSource(node->son[1], file);

            break;
        }
        case AST_VEC_ATTRIB:
        {
            for (i = 0; i < level; i++) fprintf(file, "    ");
            astGenerateSource(node->son[0], file);
            fprintf(file, " q");
            astGenerateSource(node->son[1], file);
            fprintf(file, "p = ");
            astGenerateSource(node->son[2], file);

            break;
        }
        case AST_BLK:
        {
            fprintf(file, "{\n");
            level++;
            astGenerateSource(node->son[0], file);
            level--;
            for (i = 0; i < level; i++) fprintf(file, "    ");
            fprintf(file, "}");

            break;
        }
        case AST_DB:
        {
            fprintf(file, "d ");
            astGenerateSource(node->son[0], file);
            fprintf(file, " b");

            break;
        }
        case AST_VEC:
        {
            astGenerateSource(node->son[0], file);
            fprintf(file, " q");
            astGenerateSource(node->son[1], file);
            fprintf(file, "p");

            break;
        }
        case AST_FUNC_CALL:
        {
            astGenerateSource(node->son[0], file);
            fprintf(file, "d ");
            astGenerateSource(node->son[1], file);
            fprintf(file, " b");

            break;
        }
        case AST_ADD:
        {
            astGenerateSource(node->son[0], file);
            fprintf(file, " + ");
            astGenerateSource(node->son[1], file);

            break;
        }
        case AST_SUB:
        {
            astGenerateSource(node->son[0], file);
            fprintf(file, " - ");
            astGenerateSource(node->son[1], file);

            break;
        }
        case AST_MUL:
        {
            astGenerateSource(node->son[0], file);
            fprintf(file, " * ");
            astGenerateSource(node->son[1], file);

            break;
        }
        case AST_DIV:
        {
            astGenerateSource(node->son[0], file);
            fprintf(file, " / ");
            astGenerateSource(node->son[1], file);

            break;
        }
        case AST_LESS:
        {
            astGenerateSource(node->son[0], file);
            fprintf(file, " < ");
            astGenerateSource(node->son[1], file);

            break;
        }
        case AST_GREATER:
        {
            astGenerateSource(node->son[0], file);
            fprintf(file, " > ");
            astGenerateSource(node->son[1], file);

            break;
        }
        case AST_GEQUAL:
        {
            astGenerateSource(node->son[0], file);
            fprintf(file, " >= ");
            astGenerateSource(node->son[1], file);

            break;
        }
        case AST_LEQUAL:
        {
            astGenerateSource(node->son[0], file);
            fprintf(file, " <= ");
            astGenerateSource(node->son[1], file);

            break;
        }
        case AST_EQUAL:
        {
            astGenerateSource(node->son[0], file);
            fprintf(file, " == ");
            astGenerateSource(node->son[1], file);

            break;
        }
        case AST_AND:
        {
            astGenerateSource(node->son[0], file);
            fprintf(file, " and ");
            astGenerateSource(node->son[1], file);

            break;
        }
        case AST_OR:
        {
            astGenerateSource(node->son[0], file);
            fprintf(file, " or ");
            astGenerateSource(node->son[1], file);

            break;
        }
        case AST_NOT:
        {
            fprintf(file, "not ");
            astGenerateSource(node->son[0], file);

            break;
        }
        case AST_INT_DEF: fprintf(file, "int %s", node->symbol->text); break;
        case AST_FLOAT_DEF: fprintf(file, "float %s", node->symbol->text); break;
        case AST_CHAR_DEF: fprintf(file, "char %s", node->symbol->text); break;
        case AST_SYMBOL: fprintf(file, "%s", node->symbol->text); break;
        default: fprintf(file, "OI"); break;
    }
}

void astFind(int level, AST_NODE *node, char *text)
{
    int i = 0;
    fprintf(stderr, "[astFind] Entrou astFind.\n");
    if(node == 0)
    {
        return;
    }

    if(node->type == AST_VAR_DEC)
    {
        if((strcmp(node->son[0]->symbol->text, text) == 0 && node->son[0]->symbol->type == SYMBOL_TK_IDENTIFIER))
        {
            hashSetType(text, TK_IDENTIFIER);
        }
    }

    if(node->type == AST_FUNC_DEC || node->type == AST_VEC_DEC || node->type == AST_ARG_LIST)
    {
        fprintf(stderr, "[astFind] Entrou if AST_FUNC_DEC, AST_VEC_DEC, AST_ARG_LIST.\n");
        fprintf(stderr, "[astFind] Entrou hashSetType.\n");
        hashSetType(node->son[0]->symbol->text, TK_IDENTIFIER);
        fprintf(stderr, "[astFind] Saiu hashSetType.\n");

    }

    for(i = 0; i < MAX_SONS; i++)
    {
        astFind(level + 1, node->son[i], text);
    }
}

AST_NODE *astNodeFinder(int level, AST_NODE *node, AST_NODE **son2, char *text, int *flagFound)
{
    int i = 0;

    if(node->type == AST_FUNC_DEC && strcmp(node->son[0]->symbol->text, text) == 0)
    {
        *son2 = node;
        *flagFound = 1;
    }

    if(*flagFound == 1)
    {
        return NULL;
    }

    for(i = 0; i < MAX_SONS; i++)
    {
        if(node->son[i])
        {
            astNodeFinder(level + 1, node->son[i], son2, text, flagFound);
        }
    }
}
