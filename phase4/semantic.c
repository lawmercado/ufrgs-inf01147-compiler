#include "semantic.h"
#include "y.tab.h"
#include "hash.h"

AST_NODE *getAST();
int SemanticErrorFlag = 0;

void setDeclaration(AST_NODE *root)
{
    AST_NODE *node;
    AST_NODE *son0;
    AST_NODE *son1;
    AST_NODE *son2;

    for(node = root; node; node = node->son[1])
    {
        if(node->type == AST_DEC)
        {
            setDeclaration(node->son[0]);
            node = node->son[1];
        }

        if(node->type == AST_FUNC_DEC)
        {
            setDeclaration(node->son[2]);

        }

        if(node->type == AST_BLK)
        {
            setDeclaration(node->son[0]);
        }

        if(node->type == AST_CMD_LIST)
        {
            setDeclaration(node->son[0]);
        }

        if(node->type == AST_FUNC_CALL)
        {
            setDeclaration(node->son[0]);
        }

        son0 = node->son[0];
        son1 = node->son[1];

        if(son0)
        {
                switch(node->type)
                {
                    case AST_DEC:
                    {
                        setDeclaration(son0);
                        break;
                    }

                    case AST_VAR_DEC:
                    {
                        if(son0->symbol)
                        {
                            if(son0->symbol->type != TK_IDENTIFIER)
                            {
                                fprintf(stderr, "Symbol %s redeclared.\n", son0->symbol->text);
                                SemanticErrorFlag = 1;
                            }
                        }
                        son0->symbol->type = SYMBOL_SCALAR;

                        if(son0->type == AST_INT_DEF || son0->type == AST_CHAR_DEF)
                        {
                            son0->symbol->datatype = DATATYPE_INT;
                        }
                        else
                        {
                            son0->symbol->datatype = DATATYPE_FLOAT;
                        }
                        if(son1)
                        {
                            if(son1->type == AST_SYMBOL)
                            {
                                checkDeclaration(son0,son1);
                            }
                            else if(son1->type == AST_LESS || son1->type == AST_GREATER || son1->type == AST_LEQUAL || son1->type == AST_GEQUAL || son1->type == AST_EQUAL || son1->type == AST_OR || son1->type == AST_AND || son1->type == AST_NOT || son1->type == AST_ADD || son1->type == AST_SUB || son1->type == AST_MUL || son1->type == AST_DIV)
                            {
                                checkOperands(son1, son0->symbol->datatype);
                            }
                        }
                        break;
                    }

                    case AST_FUNC_DEC:
                    {
                        if(son0->symbol)
                        {
                            if(son0->symbol->type != TK_IDENTIFIER)
                            {
                                fprintf(stderr, "Symbol %s redeclared.\n", son0->symbol->text);
                                SemanticErrorFlag = 1;
                            }
                        }

                        AST_NODE *aux = son0;

                        son0->symbol->type = SYMBOL_FUNCTION;

                        if(son0->type == AST_INT_DEF || son0->type == AST_CHAR_DEF)
                        {
                            son0->symbol->datatype = DATATYPE_INT;
                        }
                        else
                        {
                            son0->symbol->datatype = DATATYPE_FLOAT;
                        }


                        if(son1)
                        {
                            for(son1; son1; son1 = son1->son[1])
                            {
                                son0 = son1->son[0];

                                if(son0)
                                {
                                    switch (son0->type)
                                    {
                                        case AST_INT_DEF: son0->symbol->datatype = DATATYPE_INT; break;
                                        case AST_FLOAT_DEF: son0->symbol->datatype = DATATYPE_FLOAT; break;
                                        case AST_CHAR_DEF: son0->symbol->datatype = DATATYPE_CHAR; break;
                                        case AST_VEC:
                                        {
                                            {
                                                switch(son0->son[0]->type)
                                                {
                                                    case AST_INT_DEF: son0->son[0]->symbol->datatype = DATATYPE_INT; son0->son[0]->symbol->type = SYMBOL_VECTOR; break;
                                                    case AST_FLOAT_DEF: son0->son[0]->symbol->datatype = DATATYPE_FLOAT; son0->son[0]->symbol->type = SYMBOL_VECTOR; break;
                                                    case AST_CHAR_DEF: son0->son[0]->symbol->datatype = DATATYPE_CHAR; son0->son[0]->symbol->type = SYMBOL_VECTOR; break;
                                                }

                                            }

                                            break;
                                        }
                                    }
                                }
                            }

                        }

                        son2 = node->son[2];

                        if(son2)
                        {
                            while(son2->type != AST_RETURN)
                            {
                                if(son2->son[1])
                                {
                                    son2 = son2->son[1];
                                }
                                else
                                {
                                    son2 = son2->son[0];
                                }

                                if(!son2) break;
                            }

                            if(!son2) break;

                            if(son2->type == AST_RETURN)
                            {

                                if(son2->son[0]->type == AST_SYMBOL)
                                {

                                    if(aux->symbol->datatype != son2->son[0]->symbol->datatype)
                                    {
                                        if(!(((aux->symbol->datatype == DATATYPE_CHAR || aux->symbol->datatype == DATATYPE_INT) && (son2->son[0]->symbol->datatype == DATATYPE_CHAR || son2->son[0]->symbol->datatype == DATATYPE_INT))
                                         || (aux->symbol->datatype == DATATYPE_FLOAT  &&  son2->son[0]->symbol->datatype == DATATYPE_FLOAT)))
                                        {
                                            fprintf(stderr, "Function %s with type = %d and return type = %d\n", aux->symbol->text, aux->symbol->type, son2->son[0]->symbol->datatype);
                                            SemanticErrorFlag = 1;
                                        }
                                    }

                                }
                                else if(son2->son[0]->type == AST_ADD || son2->type == AST_SUB || son2->type == AST_MUL || son2->type == AST_DIV)
                                {
                                    checkOperands(son2->son[0], aux->symbol->datatype);
                                }
                                else if(son2->son[0]->type == AST_LESS || son2->son[0]->type == AST_GREATER || son2->son[0]->type == AST_LEQUAL || son2->son[0]->type == AST_GEQUAL || son2->son[0]->type == AST_EQUAL || son2->son[0]->type == AST_OR || son2->son[0]->type == AST_AND || son2->son[0]->type == AST_NOT)
                                {
                                    fprintf(stderr, "Cannot attribute a bool to a variable.\n");
                                    SemanticErrorFlag = 1;
                                }
                                else if(son2->son[0]->type == AST_FUNC_CALL)
                                {
                                    if(son2->son[0]->son[0]->symbol->datatype != aux->symbol->datatype)
                                    {
                                        fprintf(stderr, "Function %s with datatype = %d and return datatype = %d\n", aux->symbol->text, aux->symbol->datatype, son2->son[0]->son[0]->symbol->datatype);
                                        fprintf(stderr, "Datatype dont match for attribution.\n");
                                        SemanticErrorFlag = 1;
                                    }
                                    checkFuncCall(son2->son[0]);
                                }
                            }
                        }

                        break;
                    }

                    case AST_VEC_DEC:
                    {
                        if(son0->symbol)
                        {
                            if(son0->symbol->type != TK_IDENTIFIER)
                            {
                                fprintf(stderr, "Symbol %s redeclared.\n", son0->symbol->text);
                                SemanticErrorFlag = 1;
                            }
                        }

                        son0->symbol->type = SYMBOL_VECTOR;

                        if(son0->type == AST_INT_DEF || son0->type == AST_CHAR_DEF)
                        {
                            son0->symbol->datatype = DATATYPE_INT;
                        }
                        else
                        {
                            son0->symbol->datatype = DATATYPE_FLOAT;
                        }

                        if(node->son[1])
                        {
                            if(node->son[1]->type == AST_SYMBOL)
                            {
                                if(node->son[1]->symbol->datatype != DATATYPE_CHAR && node->son[1]->symbol->datatype != DATATYPE_INT)
                                {
                                    fprintf(stderr, "Index must be INTEGER.\n");
                                    SemanticErrorFlag = 1;
                                }
                            }
                        }

                        if(node->son[2])
                        {
                            son2 = node->son[2];

                            while(son2->type == son2->son[0]->type)
                            {
                                if(son0->symbol->datatype == DATATYPE_INT && !(son2->son[1]->symbol->datatype == DATATYPE_CHAR || son2->son[1]->symbol->datatype == DATATYPE_INT))
                                {
                                    fprintf(stderr, "Argument %s invalid in vector %s declaration; expected a integer;\n", son2->son[1]->symbol->text, son0->symbol->text);
                                    SemanticErrorFlag = 1;
                                }

                                if(son0->symbol->datatype == DATATYPE_FLOAT && (son2->son[1]->symbol->datatype == DATATYPE_CHAR || son2->son[1]->symbol->datatype == DATATYPE_INT))
                                {
                                    fprintf(stderr, "Argument %s invalid in vector %s declaration; expected a float;\n", son2->son[1]->symbol->text, son0->symbol->text);
                                    SemanticErrorFlag = 1;
                                }

                                son2 = son2->son[0];
                            }

                            if(son0->symbol->datatype == DATATYPE_INT && !(son2->son[0]->symbol->datatype == DATATYPE_CHAR || son2->son[0]->symbol->datatype == DATATYPE_INT))
                            {
                                fprintf(stderr, "Argument %s invalid in vector %s declaration; expected a integer;\n", son2->son[0]->symbol->text, son0->symbol->text);
                                SemanticErrorFlag = 1;
                            }

                            if(son0->symbol->datatype == DATATYPE_FLOAT && (son2->son[0]->symbol->datatype == DATATYPE_CHAR || son2->son[0]->symbol->datatype == DATATYPE_INT))
                            {
                                fprintf(stderr, "Argument %s invalid in vector %s declaration; expected a float;\n", son2->son[0]->symbol->text, son0->symbol->text);
                                SemanticErrorFlag = 1;
                            }

                        }
                        break;
                    }

                    case AST_ATTRIB:
                    {
                        if(son0->symbol->type == SYMBOL_VECTOR)
                        {
                            fprintf(stderr, "Attributing scalar to a vector (%s) type;\n", son0->symbol->text);
                            SemanticErrorFlag = 1;
                        }
                        else if(son0->symbol->type == SYMBOL_FUNCTION)
                        {
                            fprintf(stderr, "Attributing scalar to a function (%s) type;\n", son0->symbol->text);
                            SemanticErrorFlag = 1;
                        }
                        else if(son1)
                        {
                            if(son1->type == AST_SYMBOL)
                            {
                                if(son1->symbol->type == SYMBOL_FUNCTION)
                                {
                                    fprintf(stderr, "Cannot use function without parameters (%s);\n", son1->symbol->text);
                                    SemanticErrorFlag = 1;
                                }
                                else if(son1->symbol->type == SYMBOL_SCALAR)
                                {
                                    if(son1->symbol->datatype != son0->symbol->datatype)
                                    {
                                        if(son0->symbol->datatype == 1)
                                            fprintf(stderr, "Expecting an integer datatype.\n");
                                        else if(son0->symbol->datatype == 3)
                                            fprintf(stderr, "Expecting an float datatype.\n");

                                        SemanticErrorFlag = 1;
                                    }
                                }
                            }
                            else if(son1->type == AST_ADD || son1->type == AST_SUB || son1->type == AST_MUL || son1->type == AST_DIV)
                            {
                                checkOperands(son1, son0->symbol->datatype);
                            }
                            else if(son1->type == AST_LESS || son1->type == AST_GREATER || son1->type == AST_LEQUAL || son1->type == AST_GEQUAL || son1->type == AST_EQUAL || son1->type == AST_OR || son1->type == AST_AND || son1->type == AST_NOT)
                            {
                                fprintf(stderr, "Cannot attribute a bool to a variable.\n");
                                SemanticErrorFlag = 1;
                            }
                            else if(son1->type == AST_FUNC_CALL)
                            {
                                if(son1->son[0]->symbol->datatype != son0->symbol->datatype)
                                {
                                    fprintf(stderr, "Datatype dont match for attribution.\n");
                                    SemanticErrorFlag = 1;
                                }
                                checkFuncCall(son1);
                            }

                        }
                        break;
                    }

                    case AST_VEC_ATTRIB:
                    {
                        if(son0->symbol->type == SYMBOL_SCALAR)
                        {
                            fprintf(stderr, "Cannot index a scalar variable (%s) ;\n", son0->symbol->text);
                            SemanticErrorFlag = 1;
                        }
                        if(son1)
                        {
                            if(son1->type == AST_SYMBOL)
                            {
                                if(son1->symbol->datatype != DATATYPE_CHAR && son1->symbol->datatype != DATATYPE_INT)
                                {
                                    fprintf(stderr, "Index must be integer.\n");
                                    SemanticErrorFlag = 1;
                                }
                            }
                        }

                        son2 = node->son[2];

                        if(son2)
                        {

                            if(son2->type == AST_SYMBOL)
                            {
                                if(son2->symbol->type == SYMBOL_FUNCTION)
                                {
                                    fprintf(stderr, "Cannot use function without parameters (%s);\n", son2->symbol->text);
                                    SemanticErrorFlag = 1;
                                }
                                else if(son2->symbol->type == SYMBOL_SCALAR)
                                {
                                    if(son2->symbol->datatype != son0->symbol->datatype)
                                    {
                                        if(son0->symbol->datatype == DATATYPE_INT)
                                            fprintf(stderr, "Expecting an integer datatype.\n");
                                        else if(son0->symbol->datatype == DATATYPE_FLOAT)
                                            fprintf(stderr, "Expecting an float datatype.\n");

                                        SemanticErrorFlag = 1;
                                    }
                                }
                            }
                            else if(son2->type == AST_ADD || son2->type == AST_SUB || son2->type == AST_MUL || son2->type == AST_DIV)
                                checkOperands(son2, son0->symbol->datatype);
                            else if(son2->type == AST_LESS || son2->type == AST_GREATER || son2->type == AST_LEQUAL || son2->type == AST_GEQUAL || son2->type == AST_EQUAL || son2->type == AST_OR || son2->type == AST_AND || son2->type == AST_NOT)
                            {
                                fprintf(stderr, "Cannot attribute bool to a variable.\n");
                                SemanticErrorFlag = 1;
                            }
                            else if(son2->type == AST_FUNC_CALL)
                            {
                                if(son2->son[0]->symbol->datatype != son0->symbol->datatype)
                                {
                                    fprintf(stderr, "Datatype dont match for attribution.\n");
                                    SemanticErrorFlag = 1;
                                }
                                checkFuncCall(son2->son[0]);
                            }
                        }

                        break;

                    }

                    case AST_FUNC_CALL:
                    {
                        int flagFound = 0;
                        int i = 0, j = 0;

                        AST_NODE *son2;
                        AST_NODE *son2Aux;
                        AST_NODE *son1Aux;

                        astNodeFinder(0, getAST(), &son2, son0->symbol->text, &flagFound);

                        int pCount = 0;
                        int argCount = 0;

                        if(son2) //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                        {
                            if(flagFound == 1)
                            {
                                if(son2->type == AST_FUNC_DEC)
                                {
                                    son2Aux = son2->son[1];

                                    for(son2 = son2Aux; son2; son2 = son2->son[1])
                                    {
                                        pCount++;
                                    }
                                }
                            }
                            else
                            {
                                fprintf(stderr, "Call invalid function '%s'", son0->symbol->text);
                                SemanticErrorFlag = 1;
                            }
                        }
                        if(son1)
                        {
                            son1Aux = son1;

                            for(son1; son1; son1 = son1->son[1])
                            {
                                argCount++;
                            }
                        }


                        if(argCount != pCount)
                        {
                            fprintf(stderr, "Number of arguments in function call '%s' invalid.\n", son0->symbol->text);
                            SemanticErrorFlag = 1;
                        }
                        else if(son2)
                        {
                            int i = 0, j = 0;
                            son1 = son1Aux;
                            son2 = son2Aux;

                            for(i = 0; i < argCount; son1 = son1->son[1], i++)
                            {
                                for(j = 0; j < argCount - i - 1; son2 = son2->son[1], j++);
                                if(son2->son[0]->type == AST_VEC)
                                {
                                    if(son2->son[0]->symbol->datatype != son1->son[0]->symbol->datatype)
                                    {
                                        if(!((son2->son[0]->symbol->datatype == DATATYPE_CHAR || son2->son[0]->symbol->datatype == DATATYPE_INT) && (son1->son[0]->symbol->datatype == DATATYPE_CHAR || son1->son[0]->symbol->datatype == DATATYPE_INT)))
                                        {
                                            fprintf(stderr, "Argument '%s' dont match with parameter '%s'.\n", son2->son[0]->symbol->text, son1->son[0]->symbol->text);
                                            SemanticErrorFlag = 1;
                                        }
                                    }
                                }
                                else
                                {
                                    if(son2->son[0]->symbol->datatype != son1->son[0]->symbol->datatype )
                                    {
                                        fprintf(stderr, "Argument '%s' dont match with parameter '%s'.\n", son2->son[0]->symbol->text, son1->son[0]->symbol->text);
                                        SemanticErrorFlag = 1;
                                    }
                                }
                                son2 = son2Aux;
                            }
                        }
                        break;
                    }
                    default: break;
                }
        }
    }
}

void checkUndeclared()
{
    hashCheckUndeclared();
}

void checkOperands(AST_NODE *node, int datatype)
{
    int i = 0;

    if(!node)
    {
        return;
    }

    for(i = 0; i < MAX_SONS; i++)
    {
        checkOperands(node->son[i], datatype);
    }

    switch(node->type)
    {
        case AST_SYMBOL:
        {
            if(node->symbol->type != SYMBOL_SCALAR && node->symbol->type != SYMBOL_FUNCTION)
            {
                fprintf(stderr, "Identifier %s must be scalar in arithmetic expression, %d\n", node->symbol->text, node->symbol->datatype);
                SemanticErrorFlag = 1;
            }

            if(node->symbol->datatype == DATATYPE_INT || node->symbol->datatype == DATATYPE_CHAR)
            {
                node->datatype = AST_DATATYPE_INT;
            }
            else
            {
                node->datatype = AST_DATATYPE_FLOAT;
            }
            break;
        }
        case AST_DB:
        {
            node->datatype = node->son[0]->datatype;

            if(node->son[0]->symbol)
            {
                if(node->son[0]->symbol->datatype == DATATYPE_INT || node->son[0]->symbol->datatype == DATATYPE_CHAR)
                    node->datatype = AST_DATATYPE_INT;
                else
                    node->datatype = AST_DATATYPE_FLOAT;
            }
            break;
        }
        case AST_FUNC_CALL:
        {
            node->datatype = node->son[0]->datatype;
            checkFuncCall(node);

            if(node->son[0]->symbol->datatype == DATATYPE_INT || node->son[0]->symbol->datatype == DATATYPE_CHAR)
                node->datatype = AST_DATATYPE_INT;
            else
                node->datatype = AST_DATATYPE_FLOAT;
            break;
        }
        case AST_ADD:
        case AST_MUL:
        case AST_DIV:
        case AST_SUB:
            if(node->son[0]->datatype == AST_DATATYPE_BOOL || node->son[1]->datatype == AST_DATATYPE_BOOL )
            {
                fprintf(stderr, "Cannot use boolean in arithmetic expression.\n");
                SemanticErrorFlag = 1;
            }
            else if(node->son[0]->datatype != node->son[1]->datatype)
            {
                fprintf(stderr, "Arithmetic operation cannot mix float and int.\n");
                SemanticErrorFlag = 1;
            }
            else
            {
                node->datatype = node->son[0]->datatype;
            }
            break;
        case AST_NOT:
        {
            if(node->son[0]->datatype != AST_DATATYPE_BOOL)
            {
                fprintf(stderr, "Only boolean allowed when using NOT.\n");
                SemanticErrorFlag = 1;
            }

            node->datatype = AST_DATATYPE_BOOL;
            break;
        }
        case AST_ATTRIB:
                break;
        case AST_LESS:
        case AST_GREATER:
        case AST_LEQUAL:
        case AST_GEQUAL:
        case AST_EQUAL:
        case AST_OR:
        case AST_AND:
        {
            if(node->son[0]->datatype != node->son[1]->datatype)
            {
                fprintf(stderr, "Cannot mix different datatypes in boolean expression.\n");
                SemanticErrorFlag = 1;
            }

            node->datatype = AST_DATATYPE_BOOL;

            break;
        }
    }
}

void checkDeclaration(AST_NODE *son0, AST_NODE *son1)
{
    if(son0->symbol->datatype == DATATYPE_INT && !(son1->symbol->datatype == DATATYPE_INT || son1->symbol->datatype == DATATYPE_CHAR))
    {
        fprintf(stderr, "Symbol %s must be integer.\n", son1->symbol->text);
        SemanticErrorFlag = 1;
    }
    else if(son0->symbol->datatype == DATATYPE_FLOAT && son1->symbol->datatype != DATATYPE_FLOAT)
    {
        fprintf(stderr, "Symbol %s must be float.\n", son1->symbol->text);
        SemanticErrorFlag = 1;
    }
}

void checkFuncCall(AST_NODE* node)
{

    int found = 0;

    AST_NODE *son2;
    AST_NODE *son2Aux;
    AST_NODE *son1Aux;
    AST_NODE *son0 = node->son[0];
    AST_NODE *son1 = node->son[1];

    astNodeFinder(0, getAST(),  &son2 , son0->symbol->text, &found);

    if(found == 1)
    {
        int paramCounter = 0;
        int argumentCounter = 0;

        son2Aux = son2->son[1];
        son1Aux = son1;

        for(son2 = son2->son[1]; son2; son2 = son2->son[1])
            paramCounter++;

        for(; son1; son1 = son1->son[1])
            argumentCounter++;

        if(argumentCounter != paramCounter)
        {
            fprintf(stderr, "Number of arguments in function call '%s' invalid.\n", son0->symbol->text);
            SemanticErrorFlag = 1;
        }
        else
        {
            int i = 0, k = 0;

            son2 = son2Aux;
            son1 = son1Aux;

            for(i = 0; i < argumentCounter; son1 = son1->son[1],son2 = son2->son[1], i++)
            {

                if(son2->son[0]->type == AST_VEC)
                {
                    if(son2->son[0]->son[0]->symbol->datatype != son1->son[0]->symbol->datatype )
                    {
                        if(!((son2->son[0]->son[0]->symbol->datatype  == DATATYPE_INT || son2->son[0]->son[0]->symbol->datatype  == DATATYPE_CHAR)  &&
                             (son1->son[0]->son[0]->symbol->datatype  == DATATYPE_INT || son1->son[0]->son[0]->symbol->datatype  == DATATYPE_CHAR)))
                             {
                                 fprintf(stderr, "Argument '%s'  dont match with parameter '%s'.\n", son2->son[0]->son[0]->symbol->text, son1->son[0]->symbol->text);
                                 SemanticErrorFlag = 1;
                             }

                    }
                }
                else
                {
                    if(son2->son[0]->symbol->datatype != son1->son[0]->symbol->datatype )
                    {
                        if(!((son2->son[0]->symbol->datatype  == DATATYPE_INT || son2->son[0]->symbol->datatype  == DATATYPE_CHAR)  &&
                             (son1->son[0]->symbol->datatype  == DATATYPE_INT || son1->son[0]->symbol->datatype  == DATATYPE_CHAR)))
                         {
                             fprintf(stderr, "Argument '%s' dont match with parameter '%s'.\n", son2->son[0]->symbol->text, son1->son[0]->symbol->text);
                             SemanticErrorFlag = 1;
                         }

                    }
                }
                son2 = son2Aux;
            }
        }

    }

}
