#ifndef TACS_HEADER
#define TACS_HEADER

#include "hash.h"
#include "ast.h"

#define TAC_SYMBOL 1
#define TAC_ADD 2
#define TAC_SUB 3
#define TAC_MUL 4
#define TAC_DIV 5
#define TAC_LESS 6
#define TAC_IFZ 7

typedef struct tac_struct
{
    int type;
    HASH_NODE *res;
    HASH_NODE *op1;
    HASH_NODE *op2;
    struct tac_struct *prev;
    struct tac_struct *next;
} TAC;

TAC *tacCreate(int type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2);
void tacPrintSingle(TAC *tac);
TAC *tacReverse(TAC *last);
void tacPrintBackward(TAC *tac);
void tacPrintForward(TAC *tac);

//CODE GENERATION

TAC *tacGenerate(AST_NODE *node);
TAC *tacJoin(TAC *l1, TAC *l2);

#endif
