%option yylineno
%{
#include "tokens.h"
%}

DIGIT               [0-9]
LIT_CH              ('([^'\\\n]|\\.)')|('')
LIT_STR             \"(\\.|[^"\\])*\"
ID                  ([ace-or-zACE-OR-Z_0-9])|(<[0-9]+>[ace-or-zACE-OR-Z_])
SPECIAL_CH          [,;:dbqp\{\}+\-*/<>\=!&\$#]
SL_COMMENT          "//".*
ML_COMMENT          "/*"([^*]|(\*+[^*/]))*"*/"

%%

"char"              { return KW_CHAR; }
"int"               { return KW_INT; }
"float"             { return KW_FLOAT; }
"if"                { return KW_IF; }
"then"              { return KW_THEN; }
"else"              { return KW_ELSE; }
"while"             { return KW_WHILE; }
"read"              { return KW_READ; }
"print"             { return KW_PRINT; }
"return"            { return KW_RETURN; }
"or"                { return OPERATOR_OR; }
"and"               { return OPERATOR_AND; }
"not"               { return OPERATOR_NOT; }
"<="                { return OPERATOR_LE; }
">="                { return OPERATOR_GE; }
"=="                { return OPERATOR_EQ; }
{DIGIT}+            { return LIT_INTEGER; }
{DIGIT}+"."{DIGIT}* { return LIT_FLOAT; }
{LIT_CH}            { return LIT_CHAR; }
{LIT_STR}           { return LIT_STRING; }
{ID}+               { return TK_IDENTIFIER; }
{SPECIAL_CH}+       { return yytext[0]; }
{SL_COMMENT}+
{ML_COMMENT}+
[ \t\n]
.                   { return TOKEN_ERROR; }
%%
