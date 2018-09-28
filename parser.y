%{
/**
 * Syntactical analyser for using with yacc
 *
 * @author Luís Augusto Weber Mercado [lawmercado@inf.ufrgs.br]
 * @author Nicholas de Aquino Lau [nalau@inf.ufrgs.br]
 */

#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "ast.h"

int yylex(void);
int yyerror(char *);
int getLineNumber(void);

%}

%union {
    int intValue;
    float floatValue;
    char* stringValue;
    char charValue;
    struct hash_node* symbol;
    struct ast_node *ast;
}

%token KW_CHAR
%token KW_INT
%token KW_FLOAT
%token KW_IF
%token KW_THEN
%token KW_ELSE
%token KW_WHILE
%token KW_READ
%token KW_RETURN
%token KW_PRINT
%token OPERATOR_LE
%token OPERATOR_GE
%token OPERATOR_EQ
%token OPERATOR_OR
%token OPERATOR_AND
%token OPERATOR_NOT
%token<stringValue> TK_IDENTIFIER
%token<intValue> LIT_INTEGER
%token<floatValue> LIT_FLOAT
%token<charValue> LIT_CHAR
%token<stringValue> LIT_STRING
%token TOKEN_ERROR
%token TOKEN_UNKNOWN

%type<ast> expression
%type<ast> command
%type<ast> command_list
%type<ast> print_parameter_list

%start program

%right '='
%left OPERATOR_NOT OPERATOR_AND OPERATOR_OR
%left OPERATOR_EQ OPERATOR_GE OPERATOR_LE '>' '<'
%left '+' '-'
%left '*' '/'
%right KW_IF KW_THEN KW_ELSE
%right 'd' TK_IDENTIFIER

%%

program
    : program declaration
    | declaration
    ;

declaration
    : varibales_definition ';'
    | functions_definition
    ;

varibales_definition
    : type_definition '=' expression
    | type_definition 'q' LIT_INTEGER 'p' array_initialization
    ;

array_initialization
    : ':' array_initialization
    | expression array_initialization
    |
    ;

functions_definition
    : type_definition 'd' parameter_definition 'b' '{' command_list '}'
    ;

command_list
    : command ';' command_list  { $$ = astCreate(AST_LCMD, 0,$1,$3,0,0); }
    |
    ;

command
    : KW_IF expression KW_THEN command { $$ = astCreate(AST_IF,0,$2,$4,0,0); }
    | KW_IF expression KW_THEN command KW_ELSE command { $$ = astCreate(AST_IFTE,0,$2,$4,0,0); }

    | KW_WHILE expression command { $$ = astCreate(AST_WHILE,0,$2,$3,0,0); }

    | KW_PRINT print_parameter_list { $$ = astCreate(AST_PRINT,0,$2,0,0,0); }

    | KW_RETURN expression { $$ = astCreate(AST_RETURN,0,$2,0,0,0); }

    | KW_READ TK_IDENTIFIER { $$ = astCreate(AST_READ,1,0,0,0,0); }

    | TK_IDENTIFIER 'q' expression 'p' '=' expression
    | TK_IDENTIFIER '=' expression

    | '{' command_list '}'

    |

    ;

parameter_definition
    : type_definition parameter_definition
    | type_definition 'q' LIT_INTEGER 'p' parameter_definition
    | ',' parameter_definition
    |
    ;

parameter_list
    : expression parameter_list
    | ',' parameter_list
    |
    ;

print_parameter_list
    : expression print_parameter_list
    | LIT_STRING print_parameter_list
    | ',' print_parameter_list
    |
    ;

type_definition
    : KW_INT TK_IDENTIFIER
    | KW_FLOAT TK_IDENTIFIER
    | KW_CHAR TK_IDENTIFIER
    ;

expression
    : LIT_INTEGER   { $$ = astCreate(AST_SYMBOL,$1,0,0,0,0); }
    | LIT_FLOAT     { $$ = astCreate(AST_SYMBOL,$1,0,0,0,0); }
    | LIT_CHAR      { $$ = astCreate(AST_SYMBOL,$1,0,0,0,0); }
    | TK_IDENTIFIER { $$ = astCreate(AST_SYMBOL,$1,0,0,0,0); }
    | expression '+' expression     { $$ = astCreate(AST_ADD,0,$1,$3,0,0); }
    | expression '-' expression     { $$ = astCreate(AST_SUB,0,$1,$3,0,0); }
    | expression '*' expression     { $$ = astCreate(AST_MUL,0,$1,$3,0,0); }
    | expression '/' expression     { $$ = astCreate(AST_DIV,0,$1,$3,0,0); }
    | expression '<' expression     { $$ = astCreate(AST_LESS,0,$1,$3,0,0); }
    | expression '>' expression     { $$ = astCreate(AST_GREATER,0,$1,$3,0,0); }
    | expression OPERATOR_GE expression     { $$ = astCreate(AST_GE,0,$1,$3,0,0); }
    | expression OPERATOR_LE expression     { $$ = astCreate(AST_LE,0,$1,$3,0,0); }
    | expression OPERATOR_EQ expression     { $$ = astCreate(AST_EQ,0,$1,$3,0,0); }
    | expression OPERATOR_AND expression    { $$ = astCreate(AST_AND,0,$1,$3,0,0); }
    | expression OPERATOR_OR expression     { $$ = astCreate(AST_OR,0,$1,$3,0,0); }
    | OPERATOR_NOT expression       { $$ = astCreate(AST_NOT,0,$2,0,0,0); }
    | TK_IDENTIFIER 'd' parameter_list 'b'  { $$ = 0; }
    | TK_IDENTIFIER 'q' expression 'p'  { $$ = 0; }
    | 'd' expression 'b'    { $$ = 0; }
    ;
%%

int yyerror(char *s)
{
    fprintf(stderr, "ERROR parsing the source code at line %d: %s\n", getLineNumber(), s);
    exit(3);
}
