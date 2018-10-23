#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER

#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "ast.h"

void setDeclaration(AST_NODE *node);

void checkUndeclared();

void checkOperands(AST_NODE *node, int datatype);

void checkDeclaration(AST_NODE *son0, AST_NODE *son1);


#endif
