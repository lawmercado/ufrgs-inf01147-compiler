#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER

#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "ast.h"

typedef struct list_func_declaration
{
    AST_NODE *ast_node;
 	struct list_func_declaration* next;
} LIST_FUNC_DECL;

LIST_FUNC_DECL* listFuncDecl;

extern int SemanticErrorFlag;

void listFuncDeclInsert(LIST_FUNC_DECL** listFuncDecl, AST_NODE* node);

void setDeclaration(AST_NODE *node);

void checkUndeclared();

void checkOperands(AST_NODE *node, int datatype);

void checkDeclaration(AST_NODE *son0, AST_NODE *son1);

void checkFuncCall(AST_NODE* node);


#endif
