/**
 * Main provided by the Teacher. Adapted, but maintaining compatibility
 *
 * @author Luís Augusto Weber Mercado [lawmercado@inf.ufrgs.br]
 * @author Nicholas de Aquino Lau [nalau@inf.ufrgs.br]
 */

#include <stdio.h>
#include <stdlib.h>

extern FILE *yyin;

int yyparse(void);
void initMe(void);
int yylex(void);
int getLineNumber(void);

int main(int argc, char** argv)
{
    int result;

    if(argc < 2)
    {
        printf("Call: %s input \n", argv[0]);
        exit(1);
    }

	if ((yyin = fopen(argv[1], "r")) == 0)
	{
		fprintf(stderr, "Cannot open file %s\n", argv[1]);
		exit(2);
	}

	initMe();

	result = yyparse();

    if( result == 0 )
    {
        fprintf(stderr, "Accepted source code!\n");
        exit(0);
    }
    else
    {
        fprintf(stderr, "sERROR parsing the source code at line %d\n", getLineNumber());
        exit(3);
    }
}
