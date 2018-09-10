%{
/**
 * Syntactical analyser for using with yacc
 *
 * @author Luís Augusto Weber Mercado [lawmercado@inf.ufrgs.br]
 * @author Nicholas de Aquino Lau [nalau@inf.ufrgs.br]
 */

#include <stdio.h>
#include <stdlib.h>

int yylex(void);
int yyerror(char *);
int getLineNumber(void);

%}

%union {
	int intValue;
    float floatValue;
    char* stringValue;
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
%token TK_IDENTIFIER
%token<intValue> LIT_INTEGER
%token<floatValue> LIT_FLOAT
%token<intValue> LIT_CHAR
%token<stringValue> LIT_STRING
%token TOKEN_ERROR
%token TOKEN_UNKNOWN

%start program

%%

program
    : command_list
	;

command_list
    : command command_list
	|
	;

command
    : KW_IF expression KW_THEN command
    | KW_IF expression KW_THEN block ';'
    | KW_IF expression KW_THEN command KW_ELSE command
    | KW_IF expression KW_THEN block KW_ELSE command
    | KW_IF expression KW_THEN command KW_ELSE block ';'
    | KW_IF expression KW_THEN block KW_ELSE block ';'
    | KW_WHILE expression command
    | KW_WHILE expression block ';'
    | KW_PRINT parameter_list ';'
    | KW_RETURN expression ';'
    | KW_READ TK_IDENTIFIER ';'
    | type_definition 'd' parameter_definition 'b' block
    | type_definition ';'
    | array_definition ';'
    | attribuition
    | block ';'
    | ';'
    |
    ;

block
    : '{' command_list '}'
	;

type_definition
    : KW_INT TK_IDENTIFIER
	| KW_FLOAT TK_IDENTIFIER
	| KW_CHAR TK_IDENTIFIER
	;

attribuition
    : type_definition '=' expression ';'
	| TK_IDENTIFIER 'q' expression 'p' '=' expression ';'
    | TK_IDENTIFIER '=' expression ';'
	;

array_definition
    :
    type_definition 'q' LIT_INTEGER 'p' array_initialization
    ;

array_initialization
    : ':' array_initialization
	| expression array_initialization
	|
	;

parameter_list
    : expression parameter_list
	| ',' parameter_list
    |
    ;

parameter_definition
    : type_definition parameter_definition
	| ',' parameter_definition
    |
	;

expression
    : LIT_INTEGER
	| LIT_FLOAT
    | LIT_CHAR
	| LIT_STRING
	| TK_IDENTIFIER
	| expression '+' expression
	| expression '-' expression
	| expression '*' expression
	| expression '<' expression
	| expression '>' expression
	| expression OPERATOR_AND expression
	| expression OPERATOR_GE expression
	| expression OPERATOR_LE expression
	| expression OPERATOR_EQ expression
	| expression OPERATOR_NOT expression
	| expression OPERATOR_OR expression
	| 'q' expression 'p'
    | 'd' expression 'b'
	| TK_IDENTIFIER 'd' parameter_list 'b'
	;
%%

int yyerror(char *s)
{
	fprintf(stderr, "ERROR parsing the source code at line %d: %s\n", getLineNumber(), s);
	exit(3);
}
