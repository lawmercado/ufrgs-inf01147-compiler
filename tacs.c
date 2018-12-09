#include "tacs.h"

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
    if(!tac)
    {
        return;
    }

    //Nao printar symbols
    if(tac->type == TAC_SYMBOL || tac->type == TAC_BUFFER)
    {
        return;
    }
    fprintf(stderr, "TAC(");
    switch(tac->type)
    {
        case TAC_ADD : fprintf(stderr, "TAC_ADD"); break;
        case TAC_SUB : fprintf(stderr, "TAC_SUB"); break;
        case TAC_MUL : fprintf(stderr, "TAC_MUL"); break;
        case TAC_DIV : fprintf(stderr, "TAC_DIV"); break;
        case TAC_LESS : fprintf(stderr, "TAC_LESS"); break;
        case TAC_GREATER : fprintf(stderr, "TAC_GREATER"); break;
        case TAC_LEQUAL : fprintf(stderr, "TAC_LEQUAL"); break;
        case TAC_GEQUAL : fprintf(stderr, "TAC_GEQUAL"); break;
        case TAC_EQUAL : fprintf(stderr, "TAC_EQUAL"); break;
        case TAC_OR : fprintf(stderr, "TAC_OR"); break;
        case TAC_AND : fprintf(stderr, "TAC_AND"); break;
        case TAC_NOT : fprintf(stderr, "TAC_NOT"); break;
        case TAC_IF : fprintf(stderr, "TAC_IF"); break;
        case TAC_RETURN : fprintf(stderr, "TAC_RETURN"); break;
        case TAC_WHILE : fprintf(stderr, "TAC_WHILE"); break;
        case TAC_PRINT : fprintf(stderr, "TAC_PRINT"); break;
        case TAC_FUNC_CALL : fprintf(stderr, "TAC_FUNC_CALL"); break;
        case TAC_READ : fprintf(stderr, "TAC_READ"); break;
        case TAC_LABEL : fprintf(stderr, "TAC_LABEL"); break;
        case TAC_JUMP : fprintf(stderr, "TAC_JUMP"); break;
        case TAC_ARG : fprintf(stderr, "TAC_ARG"); break;
        case TAC_VEC_ATTRIB : fprintf(stderr, "TAC_VEC_ATTRIB"); break;
        case TAC_ATTRIB : fprintf(stderr, "TAC_ATTRIB"); break;
        case TAC_ARG_LIST : fprintf(stderr, "TAC_ARG_LIST"); break;
        case TAC_PARAM_LIST : fprintf(stderr, "TAC_PARAM_LIST"); break;
        case TAC_IFZ : fprintf(stderr, "TAC_IFZ"); break;
        case TAC_VEC_WRITE : fprintf(stderr, "TAC_VEC_WRITE"); break;
        case TAC_VEC_READ : fprintf(stderr, "TAC_VEC_READ"); break;
        case TAC_BEGIN_FUNCTION : fprintf(stderr, "TAC_BEGIN_FUNCTION"); break;
        case TAC_END_FUNCTION : fprintf(stderr, "TAC_END_FUNCTION"); break;
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

    if(!l1)
    {
        return l2;
    }
    if(!l2)
    {
        return l1;
    }

    aux = l2;
    while(aux->prev)
    {
        aux = aux->prev;
    }

    aux->prev = l1;

    return l2;
}

TAC *tacReverse(TAC *tac)
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

    if(!node || node->type == AST_PARAM_LIST)
    {
        return 0;
    }

    for(i=0; i<MAX_SONS; i++)
    {
        result[i] = tacGenerate(node->son[i]);
    }

    switch (node->type)
    {
        case AST_INT_DEF:
        case AST_FLOAT_DEF:
        case AST_CHAR_DEF:
        case AST_SYMBOL : return tacCreate(TAC_SYMBOL, node->symbol,0,0); break;
        case AST_ADD : return makeBinOp(TAC_ADD, result[0], result[1]); break;
        case AST_SUB : return makeBinOp(TAC_SUB, result[0], result[1]); break;
        case AST_MUL : return makeBinOp(TAC_MUL, result[0], result[1]); break;
        case AST_DIV : return makeBinOp(TAC_DIV, result[0], result[1]); break;
        case AST_LESS : return makeBinOp(TAC_LESS, result[0], result[1]); break;
        case AST_GREATER : return makeBinOp(TAC_GREATER, result[0], result[1]); break;
        case AST_LEQUAL : return makeBinOp(TAC_LEQUAL, result[0], result[1]); break;
        case AST_GEQUAL : return makeBinOp(TAC_GEQUAL, result[0], result[1]); break;
        case AST_NOT : return makeBinOp(TAC_NOT, result[0], result[1]); break;
        case AST_EQUAL : return makeBinOp(TAC_EQUAL, result[0], result[1]); break;
        case AST_OR : return makeBinOp(TAC_OR, result[0], result[1]); break;
        case AST_AND : return makeBinOp(TAC_AND, result[0], result[1]); break;
        case AST_READ : return tacCreate(TAC_READ, result[0]->res, 0, 0); break;
        case AST_IF : return makeIf(result[0], result[1]); break;
        case AST_IFELSE : return makeIfElse(result[0], result[1], result[2]); break;
        case AST_RETURN : return makeBinOp(TAC_RETURN, result[0], result[1]); break;
        case AST_WHILE : return makeWhile(result[0], result[1]); break;
        case AST_PRINT : return makePrint(node, result[0], result[1]); break;
        case AST_FUNC_CALL : return makeFuncCall(node, result[0], result[1]); break;
        case AST_FUNC_DEC : return makeFuncDec(node, result[0], result[1], result[2]); break;
        case AST_VEC_ATTRIB : return tacJoin(tacJoin(result[0], result[1]), tacCreate(TAC_VEC_WRITE, result[0] ? result[0]->res : 0, result[1] ? result[1]->res : 0, result[2] ? result[2]->res : 0)); break;
        case AST_VEC : return tacCreate(TAC_VEC_READ, makeTemp(), result[0]->res, result[1]->res); break;
        case AST_ATTRIB : return tacJoin(tacJoin(result[0], result[1]), tacCreate(TAC_ATTRIB, result[0] ? result[0]->res : 0, 0, result[1] ? result[1]->res : 0)); break;
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
    TAC *newIfTac = 0;
    TAC *newLabelTac = 0;
    HASH_NODE *newLabel = 0;

    newLabel = makeLabel();
    newIfTac = tacCreate(TAC_IFZ, newLabel, result0 ? result0->res : 0, 0);
    newLabelTac = tacCreate(TAC_LABEL, newLabel, 0, 0);

    return tacJoin(tacJoin(tacJoin(result0, newIfTac), result1), newLabelTac); //ver se e newlabeltac
}

TAC *makeIfElse(TAC *result0, TAC *result1, TAC *result2)
{
    TAC *skipTac = 0;
    TAC *elseLabelTac = 0;
    TAC *ifTac = 0;
    TAC *ifLabelTac = 0;

    HASH_NODE *ifLabel = makeLabel();
    HASH_NODE *elseLabel = makeLabel();

    skipTac = tacCreate(TAC_JUMP, elseLabel, 0, 0);
    elseLabelTac = tacCreate(TAC_LABEL, elseLabel, 0, 0);

    ifTac = tacCreate(TAC_IFZ, ifLabel, result0 ? result0->res : 0, 0);
    ifLabelTac = tacCreate(TAC_LABEL, ifLabel, 0, 0);

    return tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(result0, ifTac), result1), skipTac), ifLabelTac), result2), elseLabelTac);
}

TAC *makeWhile(TAC *result0, TAC*result1)
{
    //TACs
    TAC *whileTac;
    TAC *labelWhileTac;
    TAC *jumpLabelTac;
    TAC *jumpTac;

    //Labels
    HASH_NODE *whileLabel;
    HASH_NODE *jumpLabel;

    whileLabel = makeLabel();
    jumpLabel = makeLabel();

    jumpTac = tacCreate(TAC_JUMP, whileLabel, 0, 0);
    whileTac = tacCreate(TAC_IFZ, jumpLabel, result0 ? result0->res : 0, 0);
    labelWhileTac = tacCreate(TAC_LABEL, whileLabel, 0, 0);
    jumpLabelTac = tacCreate(TAC_LABEL, jumpLabel, 0, 0);

    return  tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(labelWhileTac, result0), whileTac), result1), jumpTac), jumpLabelTac);
}

TAC* makePrint(AST_NODE* node, TAC* result0, TAC* result1)
{
    int count = 1;
    char countStr[2];

    TAC *printTac = 0;
    TAC *secondTac = 0;

    AST_NODE* symbolNode = 0;
    TAC * expNode = 0;

    node = node->son[0];
    symbolNode = node->son[0];

    if(symbolNode->type == AST_ADD || symbolNode->type == AST_SUB || symbolNode->type == AST_MUL || symbolNode->type == AST_DIV || symbolNode->type == AST_LESS || symbolNode->type == AST_GREATER || symbolNode->type == AST_LEQUAL || symbolNode->type == AST_NOT || symbolNode->type == AST_EQUAL || symbolNode->type == AST_OR || symbolNode->type == AST_AND || symbolNode->type == AST_VEC)
    {
        expNode = tacGenerate(symbolNode);
    }

    sprintf(countStr, "%d", count);
    HASH_NODE *countNode = hashInsert(SYMBOL_SCALAR, countStr);

    printTac = tacCreate(TAC_PRINT, (node->son[0]->type == AST_SYMBOL || node->son[0]->type == AST_STRING) ? node->son[0]->symbol : expNode->res, 0, countNode);

    if(symbolNode->type == AST_ADD || symbolNode->type == AST_SUB || symbolNode->type == AST_MUL || symbolNode->type == AST_DIV || symbolNode->type == AST_LESS || symbolNode->type == AST_GREATER || symbolNode->type == AST_LEQUAL || symbolNode->type == AST_NOT || symbolNode->type == AST_EQUAL || symbolNode->type == AST_OR || symbolNode->type == AST_AND || symbolNode->type == AST_VEC)
    {
        expNode = tacJoin(result0, expNode);
        printTac = tacJoin(expNode, printTac);
    }
    else
    {
        printTac = tacJoin(result0, printTac);
    }

    if(node->son[1])
    while(node->son[1])
    {
        count++;
        sprintf(countStr, "%d", count);

        HASH_NODE *countNode = hashInsert(SYMBOL_SCALAR, countStr);

        node = node->son[1];
        symbolNode = node->son[0];

        if(symbolNode->type == AST_VEC)
        {
            symbolNode = symbolNode->son[0];
        }

        else if(symbolNode->type == AST_ADD || symbolNode->type == AST_SUB || symbolNode->type == AST_MUL || symbolNode->type == AST_DIV || symbolNode->type == AST_LESS || symbolNode->type == AST_GREATER || symbolNode->type == AST_LEQUAL || symbolNode->type == AST_NOT || symbolNode->type == AST_EQUAL || symbolNode->type == AST_OR || symbolNode->type == AST_AND || symbolNode->type == AST_VEC)
        {
            expNode = tacGenerate(symbolNode);
        }

        secondTac = tacCreate(TAC_PRINT, (node->son[0]->type == AST_SYMBOL || node->son[0]->type == AST_STRING) ? node->son[0]->symbol : expNode->res, 0, countNode);

        if(symbolNode->type == AST_ADD || symbolNode->type == AST_SUB || symbolNode->type == AST_MUL || symbolNode->type == AST_DIV || symbolNode->type == AST_LESS || symbolNode->type == AST_GREATER || symbolNode->type == AST_LEQUAL || symbolNode->type == AST_NOT || symbolNode->type == AST_EQUAL || symbolNode->type == AST_OR || symbolNode->type == AST_AND || symbolNode->type == AST_VEC)
        {
            //expNode = tacJoin(printTac, expNode);
            //printTac = tacJoin(expNode, secondTac);
            expNode = tacJoin(expNode, secondTac);
            secondTac = tacJoin(expNode, printTac);
        }
        else
        {
            //printTac = tacJoin(printTac, secondTac);
            secondTac = tacJoin(secondTac, printTac);
        }

    }
    else
    {
        return tacJoin(printTac, result1);
    }

    return tacJoin(secondTac, result1);
}

TAC *makeFuncCall(AST_NODE *node, TAC *result0, TAC *result1)
{
    int count = 0;
    char countStr[2];

    TAC *resultParam;
    TAC *bufferTac = 0;
    TAC *funcCallTac = 0;
    TAC *firstJoin = 0;
    TAC *paramTac = 0;

    bufferTac = tacCreate(TAC_BUFFER, result0 ? result0->res : 0, 0, 0);
    bufferTac->res = makeTemp();

    funcCallTac = tacCreate(TAC_FUNC_CALL, bufferTac ? bufferTac->res : 0, result0 ? result0->res : 0, 0);

    firstJoin = tacJoin(tacJoin(result0, funcCallTac), result1);

    while(node->son[1])
    {
        count++;
        sprintf(countStr, "%d", count);

        HASH_NODE *countNode = hashInsert(SYMBOL_SCALAR, countStr);

        node = node->son[1];

        resultParam = tacGenerate(node->son[0]);
        paramTac = tacCreate(TAC_ARG_LIST, resultParam ? resultParam->res : 0, result0 ? result0->res : 0, countNode);
        tacJoin(paramTac, firstJoin);
    }

    return firstJoin;
}

TAC *makeFuncDec(AST_NODE *node, TAC *result0, TAC *result1, TAC *result2)
{
    int count = 0;
    char countStr[2];

    TAC* beginTac = 0;
    TAC* endTac = 0;
    TAC* firstJoin = 0;
    TAC* paramTac = 0;
    TAC* resultParam = 0;

    beginTac = tacCreate(TAC_BEGIN_FUNCTION, result0 ? result0->res : 0, 0,  0);
    endTac = tacCreate(TAC_END_FUNCTION, result0 ? result0->res : 0, 0, 0);

    firstJoin = tacJoin(tacJoin(tacJoin(beginTac, result1), result2 ), endTac);

    while(node->son[1])
    {
        count++;
        node = node->son[1];

        sprintf(countStr, "%d", count);
        HASH_NODE* countNode = hashInsert(SYMBOL_SCALAR, countStr);

        resultParam = tacGenerate(node->son[0]);
        paramTac = tacCreate(TAC_PARAM_LIST, resultParam ? resultParam->res : 0, result0 ? result0->res : 0, countNode);
        tacJoin(paramTac, firstJoin);
    }

    return firstJoin;
}

//END OF FILE
