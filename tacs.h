#ifndef TACS_HEADER
#define TACS_HEADER

#include "hash.h"
#include "ast.h"

#define TAC_SYMBOL 1
#define TAC_ADD 2
#define TAC_SUB 3
#define TAC_MUL 4
#define TAC_DIV 5
#define TAC_DEC 6
#define TAC_ATTRIB 7
#define TAC_LESS 8
#define TAC_GREATER 9
#define TAC_GEQUAL 10
#define TAC_LEQUAL 11
#define TAC_EQUAL 12
#define TAC_AND 13
#define TAC_OR 14
#define TAC_NOT 15
#define TAC_VEC 16
#define TAC_IFZ 17
#define TAC_IF 18
#define TAC_IFELSE 19
#define TAC_WHILE 20
#define TAC_PRINT 21
#define TAC_RETURN 22
#define TAC_READ 23
#define TAC_LABEL 24
#define TAC_JUMP 25
#define TAC_INC 26
#define TAC_FUNC_CALL 27
#define TAC_ARG 28 //tac push
#define TAC_VEC_ATTRIB 29
#define TAC_BUFFER 30
#define TAC_BEGIN_FUNCTION 31
#define TAC_END_FUNCTION 32
#define TAC_BEGIN_BLK 33
#define TAC_END_BLK 34
#define TAC_ARG_LIST 35
#define TAC_PARAM_LIST 36
#define TAC_VEC_WRITE 37
#define TAC_VEC_READ 38

typedef struct tac_struct
{
    int type;
    HASH_NODE *res;
    HASH_NODE *op1;
    HASH_NODE *op2;
    AST_NODE *node;
    struct tac_struct *prev;
    struct tac_struct *next;
} TAC;

TAC *tacCreate(int type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2, AST_NODE *node);
void tacPrintSingle(TAC *tac);
TAC *tacReverse(TAC *last);
void tacPrintBackward(TAC *tac);
void tacPrintForward(TAC *tac);

//CODE GENERATION

TAC *tacGenerate(AST_NODE *node);
TAC *tacJoin(TAC *l1, TAC *l2);

//CODE GENERATION PROTOTYPES

TAC *makeBinOp(int type, TAC *result0, TAC *result1, AST_NODE *node);
TAC *makeIf(TAC *result0, TAC *result1, AST_NODE *node);
TAC *makeIfElse(TAC *result0, TAC *result1, TAC *result2, AST_NODE *node);
TAC *makeWhile(TAC *result0, TAC *result1, AST_NODE *node);
TAC* makePrint(AST_NODE* node, TAC* result0, TAC* result1);
TAC *makeFuncDec(AST_NODE *node, TAC *result0, TAC *result1, TAC *result2);
TAC *makeFuncCall(AST_NODE *node, TAC *result0, TAC *result1);



#endif
