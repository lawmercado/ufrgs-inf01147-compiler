#include <stdlib.h>
#include <stdio.h>
#include "tokens.h"

// Lex related definitions
int yylex(void);
void hashPrint(void);
extern char *yytext;
extern FILE *yyin;

int isRunning(void);
int initMe(void);

int main (int argc, char **argv)
{
    int token = 0;

    if(argc < 2)
    {
        fprintf(stderr, "Call: a.out + file_name \n");
        exit(1);
    }

    if((yyin = fopen(argv[1], "r")) == 0)
    {
        fprintf(stderr, "Error: Cannot open file \"%s\"\n", argv[1]);
        exit(2);
    }

    initMe();

    fprintf(stderr, "\nTokens: \n");

    while(isRunning())
    {
        token = yylex();

        if(!isRunning())
        {
            break;
        }

        if(token < 256)
        {
            fprintf(stderr, "[%c] ", token);
        }
        else
        {
            switch(token)
            {
                case KW_IF: fprintf(stderr, "[KW_IF] "); break;
                case KW_INT: fprintf(stderr, "[KW_INT] ");break;
                case KW_CHAR: fprintf(stderr, "[KW_CHAR] ");break;
                case KW_ELSE: fprintf(stderr, "[KW_ELSE] ");break;
                case KW_READ: fprintf(stderr, "[KW_READ] ");break;
                case KW_THEN: fprintf(stderr, "[KW_THEN] ");break;
                case KW_FLOAT: fprintf(stderr, "[KW_FLOAT] ");break;
                case KW_PRINT: fprintf(stderr, "[KW_PRINT] ");break;
                case KW_WHILE: fprintf(stderr, "[KW_WHILE] ");break;
                case KW_RETURN: fprintf(stderr, "[KW_RETURN] ");break;
                case OPERATOR_EQ: fprintf(stderr, "[OPERATOR_EQ] ");break;
                case OPERATOR_GE: fprintf(stderr, "[OPERATOR_GE] ");break;
                case OPERATOR_LE: fprintf(stderr, "[OPERATOR_LE] ");break;
                case OPERATOR_OR: fprintf(stderr, "[OPERATOR_OR] ");break;
                case OPERATOR_AND: fprintf(stderr, "[OPERATOR_AND] ");break;
                case OPERATOR_NOT: fprintf(stderr, "[OPERATOR_NOT] ");break;
                case LIT_CHAR: fprintf(stderr, "[LIT_CHAR] ");break;
                case LIT_FLOAT: fprintf(stderr, "[LIT_FLOAT] ");break;
                case LIT_STRING: fprintf(stderr, "[LIT_STRING] ");break;
                case LIT_INTEGER: fprintf(stderr, "[LIT_INTEGER] ");break;
                case TK_IDENTIFIER: fprintf(stderr, "[TK_IDENTIFIER] ");break;
                case TOKEN_ERROR: fprintf(stderr, "[TOKEN_ERROR] ");break;
                default: fprintf(stderr, "[UNKNOWN] "); break;
            }
        }
    }

    fprintf(stderr, "\n\nHash table data: \n");
    hashPrint();

    return 0;
}
