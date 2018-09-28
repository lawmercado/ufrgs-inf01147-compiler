%option yylineno
%{
/**
 * Lexical analyser for using with flex
 *
 * @author Luís Augusto Weber Mercado [lawmercado@inf.ufrgs.br]
 * @author Nicholas de Aquino Lau [nalau@inf.ufrgs.br]
 */


#include "hash.h"
#include "ast.h"
#include "y.tab.h"

void initMe(void);
int isRunning(void);
int getLineNumber(void);

int st_isRunning = 0;
%}

DIGIT               [0-9]
LIT_CH              ('([^'\\\n]|\\.)')|('')
LIT_STR             \"(\\.|[^"\\])*\"
ID                  ([ace-or-zACE-OR-Z_0-9]+)|(<[0-9]+>[ace-or-zACE-OR-Z_]]+)
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
{DIGIT}+            {
                        yylval.symbol = hashInsert(SYMBOL_LIT_INTEGER, yytext);
                        yylval.intValue = atoi(yytext);
                        return LIT_INTEGER;
                    }
{DIGIT}+"."{DIGIT}* {
                        yylval.symbol = hashInsert(SYMBOL_LIT_FLOAT, yytext);
                        yylval.floatValue = atof(yytext);
                        return LIT_FLOAT;
                    }
{LIT_CH}            {
                        yylval.symbol = hashInsert(SYMBOL_LIT_CHAR, yytext);
                        yylval.charValue = yytext[0];
                        return LIT_CHAR;
                    }
{LIT_STR}           {
                        yylval.symbol = hashInsert(SYMBOL_LIT_STRING, yytext);
                        yylval.stringValue = yytext;
                        return LIT_STRING;
                    }
{ID}                {
                        yylval.symbol = hashInsert(SYMBOL_TK_IDENTIFIER, yytext);
                        yylval.stringValue = yytext;
                        return TK_IDENTIFIER;
                    }
{SPECIAL_CH}        { return yytext[0]; }
{SL_COMMENT}
{ML_COMMENT}
[ \t\n]+
.                   { return TOKEN_ERROR; }

%%

int yywrap(void)
{
    st_isRunning = 0;

    return 1;
}

void initMe(void)
{
    st_isRunning = 1;
    hashInit();
}

int isRunning(void)
{
    return st_isRunning;
}

int getLineNumber(void)
{
    return yylineno;
}
