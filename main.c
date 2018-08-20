#include <stdio.h>
#include <stdlib.h>

// Lex related definitions
int yylex(void);
extern char *yytext;
extern FILE *yyin;

int isRunning(void);
void initMe(void);

int main(int argc, char** argv)
{
    FILE *expectedOutput = 0;
    int token = 0;
    int answer = 0;
    int nota = 0;
    int i = 0;

    if(argc < 3)
    {
        printf("Call: %s input.txt output.txt \n", argv[0]);
        exit(1);
    }

    if((yyin = fopen(argv[1], "r")) == 0)
    {
        printf("Error: Cannot open file %s... \n", argv[1]);
        exit(2);
    }

    if((expectedOutput = fopen(argv[2],"r")) == 0)
    {
        printf("Error: Cannot open file %s... \n", argv[2]);
        exit(2);
    }

    initMe();

    while(isRunning())
    {
        token = yylex();

        if(!isRunning())
        {
            break;
        }

        fscanf(expectedOutput,"%d",&answer);

        if(token == answer)
        {
            fprintf(stderr, "%d = OK(%s) ", i, yytext);
            ++nota;
        }
        else
        {
            fprintf(stderr, "\n%d = ERROR(%s, %d, %d) ", i, yytext, token, answer);
        }

        ++i;
    }

    fprintf(stderr, "\nGRADE %d, %.2f%% of right results\n", nota, ((float)nota/i) * 100);

    return 0;
}
