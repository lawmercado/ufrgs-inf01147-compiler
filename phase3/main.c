/**
 * Main of the compiler. Used to test its functionality
 *
 * @author Luís Augusto Weber Mercado [lawmercado@inf.ufrgs.br]
 * @author Nicholas de Aquino Lau [nalau@inf.ufrgs.br]
 */

#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern FILE *yyin;

int yyparse(void);
void initMe(void);
int yylex(void);
int getLineNumber(void);
void hashPrint(void);
void astPrint(AST_NODE *ast, int level);
AST_NODE* getAST(void);

int main(int argc, char** argv)
{
    FILE *outputFile;
    int result;

    if(argc < 3)
    {
        printf("Call: ./%s <input> <output>\n", argv[0]);

        exit(1);
    }

    if ((yyin = fopen(argv[1], "r")) == 0)
    {
        fprintf(stderr, "Cannot open file %s\n", argv[1]);

        exit(2);
    }

    if ((outputFile = fopen(argv[2], "w")) == 0)
    {
        fprintf(stderr, "Cannot open file %s\n", argv[1]);

        exit(2);
    }

    initMe();

    result = yyparse();

    if( result == 0 )
    {
        fprintf(stderr, "The hash table for the symbols of the input file is: \n");
        hashPrint();
        fprintf(stderr, "\nThe syntactical tree for the input file is: \n");
        astPrint(getAST(), 0);
        astGenerateSource(getAST(), outputFile);

        fprintf(stderr, "\nAccepted source code!\n");
        fprintf(stderr, "\nThe generated source code is in the '%s' file.\n", argv[2]);

        exit(0);
    }
    else
    {
        fprintf(stderr, "ERROR parsing the source code at line %d\n", getLineNumber());

        exit(3);
    }
}
