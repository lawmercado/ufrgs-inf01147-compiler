#ifndef GENCO_HEADER
#define GENCO_HEADER

#include "tacs.h"
//ASM
void printTacInfo(char *type, TAC *tac);
AST_NODE *findDecNode(char *text, AST_NODE* root);
void getParams(AST_NODE *node, AST_NODE *root, FILE* fasm);
void getPrintParams(AST_NODE *node, AST_NODE *root, int *count , FILE *fasm);
void getDec(AST_NODE *node, FILE *fasm);
void generateASM(TAC *tac, char *filename);
void generateVarDec(AST_NODE *node, FILE* fasm);
void generateVecDec(AST_NODE *node, FILE* fasm);
void generateString(AST_NODE *node, FILE* fasm);
void generateVarTemp(AST_NODE *node, FILE* fasm);
void generatePrint(AST_NODE *node, TAC *tac, FILE* fasm);
void generateExpression(TAC *tac, FILE *fasm);
void generateBool(TAC *tac, FILE *fasm);
void generateVecRead(TAC *tac, AST_NODE *root, FILE *fasm);
void generateVecWrite(TAC *tac, AST_NODE *root, FILE *fasm);
void generateParams(AST_NODE *node, AST_NODE *root, FILE* fasm);
void generateVarAttrib(TAC *tac, AST_NODE *root, FILE *fasm);

#endif
