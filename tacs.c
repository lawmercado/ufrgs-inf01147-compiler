#include "tacs.h"

//CODE GENERATION PROTOTYPES

TAC *makeBinOp(TAC *result0, TAC *result1);

//IMPLEMENTATION

TAC *tacCreate(int type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2)
{
    TAC *newtac;

    newtac = (TAC*) calloc(1,sizeof(TAC));
    newtac->type = type;
    newtac->res = res;
    newtac->op1 = op1;
    newtac->op2 = op2;
    newtac->prev = 0;
    newtac->next = 0;
    return newtac;
}

void tacPrintSingle(TAC *tac)
{
    if(!tac) return;
    fprintf(stderr, "TAC(");
    switch(tac->type)
    {
        case TAC_SYMBOL : fprintf(stderr, "TAC_SYMBOL"); break;
        case TAC_ADD : fprintf(stderr, "TAC_ADD"); break;
        case TAC_SUB : fprintf(stderr, "TAC_SUB"); break;
        case TAC_MUL : fprintf(stderr, "TAC_MUL"); break;
        case TAC_DIV : fprintf(stderr, "TAC_DIV"); break;
        case TAC_IFZ : fprintf(stderr, "TAC_IFZ"); break;
        case TAC_LABEL : fprintf(stderr, "TAC_LABEL"); break;
        default : fprintf(stderr, "TAC_UNKNOWN"); break;
    }

    if (tac->res) fprintf(stderr, ",%s", tac->res->text);
        else fprintf(stderr, ",0");
    if (tac->op1) fprintf(stderr, ",%s", tac->op1->text);
        else fprintf(stderr, ",0");
    if (tac->op2) fprintf(stderr, ",%s", tac->op2->text);
        else fprintf(stderr, ",0");
    fprintf(stderr, ")\n");
}

void tacPrintBackward(TAC *tac)
{
    if(!tac) return;
    tacPrintSingle(tac);
    tacPrintBackward(tac->prev);
}

TAC *tacJoin(TAC *l1, TAC *l2)
{
    TAC *aux;

    if(!l1) return l2;
    if(!l2) return l1;

    for(aux = l2; aux->prev; aux = aux->prev)
        ;
    aux->prev = l1;
    return l2;
}

TAC *tacReverse(TAC *tac);
{
    if(!tac) return 0;
    for(; tac->prev; tac = tac->prev)
    {
        tac->prev->next = tac;
    }
    return tac;
}

void tacPrintForward(TAC *tac)
{
    if(!tac) return;
    tacPrintSingle(tac);
    tacPrintForward(tac->next);
}

TAC *tacGenerate(AST_NODE *node)
{
    int i = 0;
    TAC *result[MAX_SONS];

    if(!node)
        return 0;

    for(i=0; i<MAX_SONS; i++)
    {
        result[i] = tacGenerate(node->son[i]);
    }

    switch (node->type)
    {
        case AST_SYMBOL : return tacCreate(TAC_SYMBOL, node->symbol,0,0); break;
        case AST_ADD : return makeBinOp(TAC_ADD, result[0], result[1]); break;
        case AST_SUB : return makeBinOp(TAC_SUB, result[0], result[1]); break;
        case AST_MUL : return makeBinOp(TAC_MUL, result[0], result[1]); break;
        case AST_LESS : return makeBinOp(TAC_LESS, result[0], result[1]); break;
        case AST_IF : return makeBinOp(result[0], result[1]); break;
        default : return tacJoin(tacJoin(tacJoin(result[0], result[1]), result[2]), result[3]); break;
    }
    return 0;
}

TAC *makeBinOp(int type, TAC *result0, TAC *result1)
{
    return tacJoin(tacJoin(result0, result1), tacCreate(type, makeTemp(), result0 ? result0->res : 0, result1 ? result1->res : 0));
}

TAC *makeIf(TAC *result0, TAC *result1)
{
    TAC *newiftac = 0;
    TAC *newlabeltac = 0;
    HASH_NODE *newlabel = 0;

    newlabel = makeLabel();
    newiftac = tacCreate(TAC_IFZ, newlabel, result0 ? result0->res : 0, 0);
    newlabeltac = tacCreate(TAC_LABEL, newlabel, 0, 0);

    return tacJoin(tacJoin(tacJoin(result0, newiftac), result1), newlabel);
}

//END OF FILE
