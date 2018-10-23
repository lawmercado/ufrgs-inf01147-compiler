#include "semantic.h"
#include "y.tab.h"
#include "hash.h"

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

            fprintf(stderr, "Node type: %d.\n", node->type);
        }

        son0 = node->son[0];
        son1 = node->son[1];

        if(son0)
        {
            fprintf(stderr, "Node type: %d, Dec type %d.\n", node->type, son0->type);

            switch(node->type)
            {
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

                    if(son1->type == AST_SYMBOL)
                    {
                        checkDeclaration(son0,son1);
                    }
                    else if(son1->type == AST_LESS || son1->type == AST_GREATER || son1->type == AST_LEQUAL || son1->type == AST_GEQUAL || son1->type == AST_EQUAL || son1->type == AST_OR || son1->type == AST_AND || son1->type == AST_NOT || son1->type == AST_ADD || son1->type == AST_SUB || son1->type == AST_MUL || son1->type == AST_DIV)
                    {
                        checkOperands(son1, son0->symbol->datatype);
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

                    son0->symbol->type = SYMBOL_FUNCTION;

                    if(son0->type == AST_INT_DEF || son0->type == AST_CHAR_DEF)
                    {
                        son0->symbol->datatype = DATATYPE_INT;
                    }
                    else
                    {
                        son0->symbol->datatype = DATATYPE_INT;
                    }
                    break;
                }

                case AST_VEC_DEC:
                {
                    fprintf(stderr, "TEM VETOR\n");
                    if(son0->symbol)
                    {
                        if(son0->symbol->type != TK_IDENTIFIER)
                        {
                            fprintf(stderr, "Symbol %s redeclared.\n", son0->symbol->text);
                            SemanticErrorFlag = 1;
                        }
                    }

                    son0->symbol->type = SYMBOL_FUNCTION;

                    if(son0->type == AST_INT_DEF || son0->type == AST_CHAR_DEF)
                    {
                        son0->symbol->datatype = DATATYPE_INT;
                    }
                    else
                    {
                        son0->symbol->datatype = DATATYPE_FLOAT;
                    }

                    if(node->son[2])
                    {
                        son2 = node->son[2];
                        fprintf(stderr, "ENTROU IF\n");

                        while(son2->type == son2->son[0]->type)
                        {
                            fprintf(stderr, "ENTROU WHILE\n");
                            if(son0->symbol->datatype == DATATYPE_INT && !(son2->son[1]->symbol->datatype == DATATYPE_CHAR || son2->son[1]->symbol->datatype == DATATYPE_INT))
                            {
                                fprintf(stderr, "[SEMANTIC] Argument %s invalid in vector %s declaration; expected a integer;\n", son2->son[1]->symbol->text, son0->symbol->text);
                            }

                                if(son0->symbol->datatype == DATATYPE_FLOAT && (son2->son[1]->symbol->datatype == DATATYPE_CHAR || son2->son[1]->symbol->datatype == DATATYPE_INT))
                                {
                                    fprintf(stderr, "[SEMANTIC] Argument %s invalid in vector %s declaration; expected a float;\n", son2->son[1]->symbol->text, son0->symbol->text);
                                }

                                son2 = son2->son[0];
                            }

                            if(son0->symbol->datatype == DATATYPE_INT &&
                            !(son2->son[0]->symbol->datatype == DATATYPE_CHAR || son2->son[0]->symbol->datatype == DATATYPE_INT))
                            {
                                fprintf(stderr, "[SEMANTIC] Argument %s invalid in vector %s declaration; expected a integer;\n", son2->son[0]->symbol->text, son0->symbol->text);
                            }

                            if(son0->symbol->datatype == DATATYPE_FLOAT &&
                            (son2->son[0]->symbol->datatype == DATATYPE_CHAR || son2->son[0]->symbol->datatype == DATATYPE_INT))
                            {
                                fprintf(stderr, "[SEMANTIC] Argument %s invalid in vector %s declaration; expected a float;\n", son2->son[0]->symbol->text, son0->symbol->text);
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

    if(node == 0)
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
            if(node->symbol->type != SYMBOL_SCALAR)
            {
                fprintf(stderr, "Identifier %s must be scalar in arithmetic expression.\n", node->symbol->text);
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
        case AST_ADD:
        case AST_MUL:
        case AST_DIV:
        case AST_SUB:
            if(node->son[0]->type == AST_LESS ||
                node->son[0]->type == AST_GREATER ||
                node->son[0]->type == AST_GEQUAL ||
                node->son[0]->type == AST_LEQUAL ||
                node->son[0]->type == AST_EQUAL ||
                node->son[0]->type == AST_OR ||
                node->son[0]->type == AST_AND ||
                node->son[1]->type == AST_LESS ||
                node->son[1]->type == AST_GREATER ||
                node->son[1]->type == AST_GEQUAL ||
                node->son[1]->type == AST_LEQUAL ||
                node->son[1]->type == AST_EQUAL ||
                node->son[1]->type == AST_OR ||
                node->son[1]->type == AST_AND )
                {
                    fprintf(stderr, "Invalid operand in arithmetic expression.\n");
                    SemanticErrorFlag = 1;
                }

                if(node->son[0]->datatype != node->son[1]->datatype)
                {
                    fprintf(stderr, "Arithmetic operation cannot mix float and int data.\n");
                    SemanticErrorFlag = 1;
                }
                else
                {
                    node->datatype = node->son[0]->datatype;
                }
            break;
        case AST_ATTRIB:
            break;
    }
}

void checkDeclaration(AST_NODE *son0, AST_NODE *son1)
{
    if(son0->symbol->datatype == DATATYPE_INT && !(son1->symbol->datatype == DATATYPE_INT || son1->symbol->datatype == DATATYPE_CHAR))
    {
        fprintf(stderr, "[SEMANTIC] Symbol %s must be integer.\n", son1->symbol->text);

    }
    else if(son0->symbol->datatype == DATATYPE_FLOAT && son1->symbol->datatype != DATATYPE_FLOAT)
    {
        fprintf(stderr, "[SEMANTIC] Symbol %s must be float.\n", son1->symbol->text);
    }
}
