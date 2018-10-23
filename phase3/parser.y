%{
/**
 * Syntactical analyser to use with yacc
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
AST_NODE* getAST(void);

AST_NODE* root = NULL;

%}

%union {
    HASH_NODE *symbol;
    AST_NODE *ast_node;
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
%token<symbol> TK_IDENTIFIER
%token<symbol> LIT_INTEGER
%token<symbol> LIT_FLOAT
%token<symbol> LIT_CHAR
%token<symbol> LIT_STRING
%token TOKEN_ERROR
%token TOKEN_UNKNOWN

%type<ast_node> program
%type<ast_node> declaration
%type<ast_node> type_definition
%type<ast_node> varibales_definition
%type<ast_node> functions_definition
%type<ast_node> parameter_definition_list
%type<ast_node> parameter_definition
%type<ast_node> parameter_list
%type<ast_node> print_parameter_list
%type<ast_node> print_parameter
%type<ast_node> literal_list
%type<ast_node> literal
%type<ast_node> command_list
%type<ast_node> command
%type<ast_node> block
%type<ast_node> expression
%type<ast_node> integer
%type<ast_node> identifier

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
    : program declaration { root = $$ = astCreate(AST_DEC, 0, $1, $2, 0, 0); }
    | declaration
    ;

declaration
    : varibales_definition ';'
    | functions_definition
    ;

varibales_definition
    : type_definition '=' expression { $$ = astCreate(AST_VAR_DEC, 0, $1, $3, 0, 0); }
    | type_definition 'q' integer 'p' { $$ = astCreate(AST_VEC_DEC, 0, $1, $3, 0, 0); }
    | type_definition 'q' integer 'p' ':' literal_list { $$ = astCreate(AST_VEC_DEC, 0, $1, $3, $6, 0); }
    ;

literal_list
    : literal_list literal { $$ = astCreate(AST_LIT_LIST, 0, $2, $1, 0, 0); }
    | literal { $$ = astCreate(AST_LIT_LIST, 0, $1, 0, 0, 0); }
    ;

literal
    : integer
    | LIT_FLOAT { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
    | LIT_CHAR { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
    ;

functions_definition
    : type_definition 'd' parameter_definition_list 'b' block { $$ = astCreate(AST_FUNC_DEC, 0, $1, $3, $5, 0); }
    ;

type_definition
    : KW_INT TK_IDENTIFIER { $$ = astCreate(AST_INT_DEF, $2, 0, 0, 0, 0); }
    | KW_FLOAT TK_IDENTIFIER { $$ = astCreate(AST_FLOAT_DEF, $2, 0, 0, 0, 0); }
    | KW_CHAR TK_IDENTIFIER { $$ = astCreate(AST_CHAR_DEF, $2, 0, 0, 0, 0); }
    ;

parameter_definition_list
    : parameter_definition_list ',' parameter_definition { $$ = astCreate(AST_PARAM_LIST, 0, $3, $1, 0, 0); }
    | parameter_definition { $$ = astCreate(AST_PARAM_LIST, 0, $1, 0, 0, 0); }
    | { $$ = 0; }
    ;

parameter_definition
    : type_definition 'q' integer 'p' { $$ = astCreate(AST_VEC_PARAM, 0, $1, $3, 0, 0); }
    | type_definition
    ;

block
    : '{' command_list '}' { $$ = astCreate(AST_BLK, 0, $2, 0, 0, 0); }
    ;

command_list
    : command ';' command_list { $$ = astCreate(AST_CMD_LIST, 0, $1, $3, 0, 0); }
    | { $$ = 0; }
    ;

command
    : KW_IF expression KW_THEN command { $$ = astCreate(AST_IF, 0, $2, $4, 0, 0); }
    | KW_IF expression KW_THEN command KW_ELSE command { $$ = astCreate(AST_IFELSE, 0, $2, $4, $6, 0); }
    | KW_WHILE expression command { $$ = astCreate(AST_WHILE, 0, $2, $3, 0, 0); }
    | KW_PRINT print_parameter_list { $$ = astCreate(AST_PRINT, 0, $2, 0, 0, 0); }
    | KW_RETURN expression { $$ = astCreate(AST_RETURN, 0, $2, 0, 0, 0); }
    | KW_READ identifier { $$ = astCreate(AST_READ, 0, $2, 0, 0, 0); }
    | identifier 'q' expression 'p' '=' expression { $$ = astCreate(AST_VEC_ATTRIB, 0, $1, $3, $6, 0); }
    | identifier '=' expression { $$ = astCreate(AST_ATTRIB, 0, $1, $3, 0, 0); }
    | block
    | { $$ = 0; }
    ;

parameter_list
    : parameter_list ',' expression { $$ = astCreate(AST_PARAM_LIST, 0, $3, $1, 0, 0); }
    | expression { $$ = astCreate(AST_PARAM_LIST, 0, $1, 0, 0, 0); }
    | { $$ = 0; }
    ;

print_parameter_list
    : print_parameter_list ',' print_parameter { $$ = astCreate(AST_PARAM_LIST, 0, $3, $1, 0, 0); }
    | print_parameter { $$ = astCreate(AST_PARAM_LIST, 0, $1, 0, 0, 0); }
    | { $$ = 0; }
    ;

print_parameter
    : expression
    | LIT_STRING { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
    ;

integer
    : LIT_INTEGER { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
    ;

identifier
    : TK_IDENTIFIER { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
    ;

expression
    : literal
    | identifier
    | expression '+' expression { $$ = astCreate(AST_ADD, 0, $1, $3, 0, 0); }
    | expression '-' expression { $$ = astCreate(AST_SUB, 0, $1, $3, 0, 0); }
    | expression '*' expression { $$ = astCreate(AST_MUL, 0, $1, $3, 0, 0); }
    | expression '/' expression { $$ = astCreate(AST_DIV, 0, $1, $3, 0, 0); }
    | expression '<' expression { $$ = astCreate(AST_LESS, 0, $1, $3, 0, 0); }
    | expression '>' expression { $$ = astCreate(AST_GREATER, 0, $1, $3, 0, 0); }
    | expression OPERATOR_GE expression { $$ = astCreate(AST_GEQUAL, 0, $1, $3, 0, 0); }
    | expression OPERATOR_LE expression { $$ = astCreate(AST_LEQUAL, 0, $1, $3, 0, 0); }
    | expression OPERATOR_EQ expression { $$ = astCreate(AST_EQUAL, 0, $1, $3, 0, 0); }
    | expression OPERATOR_AND expression { $$ = astCreate(AST_AND, 0, $1, $3, 0, 0); }
    | expression OPERATOR_OR expression { $$ = astCreate(AST_OR, 0, $1, $3, 0, 0); }
    | OPERATOR_NOT expression { $$ = astCreate(AST_NOT, 0, $2, 0, 0, 0); }
    | identifier 'd' parameter_list 'b' { $$ = astCreate(AST_FUNC_CALL, 0, $1, $3, 0, 0); }
    | identifier 'q' expression 'p' { $$ = astCreate(AST_VEC, 0, $1, $3, 0, 0); }
    | 'd' expression 'b' { $$ = astCreate(AST_DB, 0, $2, 0, 0, 0); }
    ;
%%

int yyerror(char *s)
{
    fprintf(stderr, "ERROR parsing the source code at line %d: %s\n", getLineNumber(), s);
    exit(3);
}

AST_NODE* getAST()
{
    return root;
}
