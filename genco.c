#include "genco.h"
#include "tacs.h"
#include "ast.h"
#include "hash.h"
#include "y.tab.h"

#define MAX_VECTOR_PRINT_SIZE 100
#define MAX_VECTOR_PARAM_LIST 17

char funReg[6][6] = {"edi", "esi", "edx", "ecx", "r8d", "r9d"};

void printTacInfo(char *type, TAC *tac)
{
	fprintf(stderr, "%s; \tres: %s ; \top1: %s ; \top2: %s",
		type,
		tac->res ? tac->res->text : "---",
		tac->op1 ? tac->op1->text : "---",
		tac->op2 ? tac->op2->text : "---") ;
    fprintf(stderr, "\n");

	return;
}

AST_NODE* findDecNode(char *text, AST_NODE* root)
{
	AST_NODE* node = root;

	while(node->son[0])
	{
		if(node->son[0]->type == AST_VEC_DEC || node->son[0]->type == AST_VAR_DEC)
		{
			if(strcmp(node->son[0]->son[0]->symbol->text, text) == 0)
            {
				return node->son[0];
            }
        }

		if(node->son[1])
		{
			if(node->son[1]->type == AST_VEC_DEC || node->son[1]->type == AST_VAR_DEC)
			{
				if(strcmp(node->son[1]->son[0]->symbol->text, text) == 0)
                {
					return node->son[1];
                }
            }
		}
		node = node->son[0];
	}
	return NULL;
}

void generateVarDec(AST_NODE *node, FILE* fasm)
{
    fprintf(fasm, "\t.globl\t%s\n", node->son[0]->symbol->text);

    if(strcmp(node->son[1]->symbol->text, "0") == 0 || strcmp(node->son[1]->symbol->text, "0.0") == 0)
    {
		fprintf(fasm, "\t.bss\n");
    }
    else
    {
		fprintf(fasm, "\t.data\n");
    }

    if(node->son[0]->type == AST_CHAR_DEF)
    {
		fprintf(fasm, "\t.size\t%s, 1\n", node->son[0]->symbol->text);
    }
    else
    {
        fprintf(fasm, "\t.size\t%s, 4\n", node->son[0]->symbol->text);
    }
    fprintf(fasm, "%s:\n", node->son[0]->symbol->text);
}

void generateVecDec(AST_NODE *node, FILE* fasm)
{
    //VECTOR NOT INITIALIZED
    if(!node->son[2])
    {
        if(node->son[0]->type == AST_CHAR_DEF)
		{
			fprintf(fasm, "\t.comm\t%s, %s, %d\n",
			node->son[0]->symbol->text,
			node->son[1]->symbol->text,
			1);
		}
		else
		{
			int size = atoi(node->son[1]->symbol->text) * 4;
			fprintf(fasm, "\t.comm\t%s, %d, %d\n",
			node->son[0]->symbol->text,
			size,
			16);
		}
    }
    //VECTOR ALREADY INITIALIZED
    else
    {
        if(node->son[2])
        {
            int n = atoi(node->son[1]->symbol->text);
            fprintf(fasm, "\t.globl\t%s\n", node->son[0]->symbol->text);
            fprintf(fasm, "\t.data\n");
            if(node->son[0]->type != AST_CHAR_DEF)
            {
                fprintf(fasm, "\t.size\t%s, %d\n", node->son[0]->symbol->text, n * 4);
            }
            else
            {
                fprintf(fasm, "\t.size\t%s, %d\n", node->son[0]->symbol->text, n);
            }

            fprintf(fasm, "%s:\n", node->son[0]->symbol->text);

            int type = node->son[0]->type;
    		node = node->son[2];
    		while(node->son[0])
    		{
    			char auxChar;

    			if(type == AST_CHAR_DEF)
    			{
    				if(node->son[0]->type == AST_LIT_LIST)
    					auxChar =  node->son[1]->symbol->text[1];
    				else
    					auxChar =  node->son[0]->symbol->text[1];
    				fprintf(fasm, "\t.byte\t%d\n", auxChar);
    			}
    			else
    			{
    				if(node->son[0]->type == AST_LIT_LIST)
    				{
    					if(type == AST_INT_DEF)
    						fprintf(fasm, "\t.long\t%s\n", node->son[1]->symbol->text);
    					else
    						fprintf(fasm, "\t.long\t%s\n", "10348328");

    				}
    				else
    				{
    					if(type == AST_INT_DEF)
    						fprintf(fasm, "\t.long\t%s\n", node->son[0]->symbol->text);
    					else
    						fprintf(fasm, "\t.long\t%s\n", "10348328");

    				}
    			}
    			node = node->son[0];
    		}
        }
    }
}

void generateString(AST_NODE *node, FILE *fasm)
{
    HASH_NODE *hashNode;

	fprintf(fasm, "\t.section\t.rodata\n");

    int i = 0;
	int count = 0;

    for (i = 0; i < HASH_SIZE; ++i){
        for (hashNode = Table[i]; hashNode; hashNode = hashNode->next)
        {
			if(hashNode->type == LIT_STRING)
			{
				hashNode->assemblyLabel = count;
				fprintf(fasm, ".LC%d:\n", count);
				fprintf(fasm, "\t.string\t\"%s\"\n", hashNode->text);
				count++;
			}
        }
    }
	//fprintf(fasm, "\n");
}

void generateVarTemp(AST_NODE *node, FILE *fasm)
{
    HASH_NODE *hashNode;

    int i;

    for (i = 0; i < HASH_SIZE; ++i)
    {
        for (hashNode = Table[i]; hashNode; hashNode = hashNode->next)
        {
			if(hashNode->type == SYMBOL_TEMP)
			{
				fprintf(fasm, "%s:\n", hashNode->text);
				fprintf(fasm, "\t.long\t0");
				fprintf(fasm, "\n");
			}
        }
    }
	//fprintf(fasm, "\n");
}

void getDec(AST_NODE *node, FILE *fasm)
{
    if(!node)
    {
        return;
	}

	switch(node->type)
	{
		case AST_DEC:
			if(node->son[1])
            {
				getDec(node->son[0], fasm);
            }
            getDec(node->son[1], fasm);
			break;

		case AST_VAR_DEC:
			generateVarDec(node, fasm);
			break;

		case AST_VEC_DEC:
			generateVecDec(node, fasm);
			break;
	}
}

void generatePrint(AST_NODE *node, TAC *tac, FILE *fasm)
{
    //fprintf(stderr, "Entrou generatePrint\n");
    // if(tac->node->son[0]->type == AST_STRING)
	// {
    //     fprintf(stderr, "[generatePrint] if AST_STRING\n");
	// 	fprintf(fasm, "\tleaq\t.LC%d(%%rip), %%rdi\n"
    //                   "\tcall\tputs\n", tac->res->assemblyLabel);
	// }
	//else
	//{
        //fprintf(stderr, "[generatePrint] else\n");

		AST_NODE *dec = findDecNode(tac->res->text, node);

		if(dec->son[0]->type == AST_INT_DEF)
		{
            //fprintf(stderr, "[generatePrint] if AST_INT_DEF\n");
			fprintf(fasm, "\tmovl\t%s(%%rip), %%eax\n"
            "\tmovl\t%%eax, %%esi\n"
            "\tleaq\t.LC%d(%%rip), %%edi\n"
            "\tmovl\t$0, %%eax\n"
            "\tcall\tprintf", tac->res->text, tac->res->assemblyLabel);
		}
		else if(dec->son[0]->type == AST_CHAR_DEF)
		{
            //fprintf(stderr, "[generatePrint] else if AST_CHAR_DEF\n");
			fprintf(fasm, "\tmovq\t%s(%%rip), %%rax\n"
			"\tmovq\t%%rax, %%rsi\n"
			"\tmovl\t%%.LC%d, %%edi\n"
			"\tmovl\t$0, %%eax\n"
			"\tcall\tprintf", tac->res->text, tac->res->assemblyLabel);
		}
		else if(dec->son[0]->type == AST_FLOAT_DEF)
		{
            //fprintf(stderr, "[generatePrint] else if AST_FLOAT_DEF\n");
			fprintf(fasm, "\tmovss\t%s(%%rip), %%xmm0\n"
			"\tcvtss2sd\t%%xmm0, %%xmm0\n"
			"\tleaq\t.LC%d(%%rip), %%rdi\n"
			"\tmovl\t$1, %%eax\n"
			"\tcall\tprintf", tac->res->text, tac->res->assemblyLabel);
		}
	//}

    /* FLOAT => movss b(%rip), %xmm0
                cvtss2sd	%xmm0, %xmm0
                movl	$.LC0, %edi
                movl	$1, %eax
                call	printf

        INT =>  movl	b(%rip), %eax
                movl	%eax, %esi
                movl	$.LC0, %edi
                movl	$0, %eax
                call	printf

       CHAR =>  movq	b(%rip), %rax
                movq	%rax, %rsi
                movl	$.LC1, %edi
                movl	$0, %eax
                call	printf*/

    //fprintf(fasm, "\n");
}

void generateExpression(TAC *tac, FILE *fasm)
{
    char operand[8];

	switch(tac->type)
	{
        case TAC_ADD:
            strcpy(operand, "addl");
            break;
        case TAC_SUB:
            strcpy(operand, "subl");
            break;
        case TAC_MUL:
            strcpy(operand, "imull");
            break;
		case TAC_DIV:
            strcpy(operand, "idivl");
            break;
        default:
            return;
            break;
    }

    if(!tac->op1->isLiteral)
    {
        fprintf(fasm, "\tmovl\t%s(%%rip), %%edx\n", tac->op1->text);
    }
    else
    {
        fprintf(fasm, "\tmovl\t$%s, %%edx\n", tac->op1->text);
    }

    if(!tac->op2->isLiteral)
    {
        fprintf(fasm, "\tmovl\t%s(%%rip), %%eax\n", tac->op2->text);
    }
    else
    {
        fprintf(fasm, "\tmovl\t$%s, %%eax\n", tac->op2->text);
    }

    fprintf(fasm, "\t%s\t%%edx, %%eax\n", operand);
    fprintf(fasm, "\tmovl\t%%eax, %s(%%rip)\n", tac->res->text);
}

void generateBool(TAC *tac, FILE *fasm)
{
    char operand[8];

	if(tac->op1->isLiteral)
    {
        fprintf(fasm, "\tmovl\t$%s, %%edx\n", tac->op1->text);
    }
    else
    {
        fprintf(fasm, "\tmovl\t%s(%%rip), %%edx\n", tac->op1->text);
    }

    if(tac->op2->isLiteral)
    {
        fprintf(fasm, "\tmovl\t$%s, %%eax\n", tac->op2->text);
    }
    else
    {
        fprintf(fasm, "\tmovl\t%s(%%rip), %%eax\n", tac->op2->text);
    }

	fprintf(fasm, "\tcmpl\t%%eax, %%edx\n");

	switch(tac->type)
	{
        case TAC_EQUAL:
            strcpy(operand, "jne");
            break;
        case TAC_GREATER:
            strcpy(operand, "jle");
            break;
        case TAC_LESS:
            strcpy(operand, "jge");
        case TAC_GEQUAL:
            strcpy(operand, "jl");
            break;
        case TAC_LEQUAL:
            strcpy(operand, "jg");
            break;
        default:
            return;
            break;
    }

	fprintf(fasm, "\t%s", operand);
}

void generateVecRead(TAC *tac, AST_NODE *root, FILE *fasm)
{
	AST_NODE *node = findDecNode(tac->node->son[0]->symbol->text, root);

	int byteSize = 0;

	if(node->son[0]->type == AST_CHAR_DEF)
    {
		byteSize = 1;
    }
    else
    {
		byteSize = 4;
    }
	fprintf(fasm, "\tmovl\t%d+%s(%%rip), %%eax\n"
	                "\tmovl\t%%eax\t, %s(%%rip)\n", byteSize * atoi(tac->op2->text), tac->op1->text, tac->res->text);
	//fprintf(fasm, "\n");
}

void generateVecWrite(TAC *tac, AST_NODE *root, FILE *fasm)
{
    int byteSize;
	int index = atoi(tac->op1->text);
	AST_NODE* node = findDecNode(tac->res->text, root);

	if(node->son[0]->type == AST_CHAR_DEF)
	{
		byteSize = 1;

		if(tac->op2->isLiteral)
		{
			fprintf(fasm, "\tmovb\t$%d, %d+%s(%%rip) \n", tac->op2->text[1],index*byteSize, tac->res->text);
		}
		else{
			fprintf(fasm, "\tmovzbl\t$%s(%%rip), %%eax \n", tac->op2->text);
			fprintf(fasm, "\tmovl\t%%eax, %d+%s(%%rip) \n", index*byteSize, tac->res->text);
		}

	}
	else
	{
		byteSize = 4;

		if(tac->op2->isLiteral)
		{
			fprintf(fasm, "\tmovl\t$%s, %d+%s(%%rip) \n", tac->op2->text,index*byteSize, tac->res->text);
		}
		else{
			fprintf(fasm, "\tmovl\t$%s(%%rip), %%eax \n", tac->op2->text);
			fprintf(fasm, "\tmovl\t%%eax, %d+%s(%%rip) \n", index*byteSize, tac->res->text);
		}
	}
	//fprintf(output, "\n");
}

void generateVarAttrib(TAC *tac, AST_NODE *root, FILE *fasm)
{

	AST_NODE *node = findDecNode(tac->res->text, root);

	if(node->son[0]->type == AST_CHAR_DEF)
	{
		if(tac->op2->isLiteral)
		{
			fprintf(fasm, "\tmovb\t$%d, %s(%%rip) \n", tac->op2->text[1], tac->res->text);
		}
		else
        {
			fprintf(fasm, "\tmovzbl\t$%s(%%rip), %%eax \n", tac->op2->text);
			fprintf(fasm, "\tmovl\t%%%s, %s(%%rip) \n", "al",  tac->res->text);
		}

	}
	else
	{
		if(tac->op2->isLiteral)
		{
			fprintf(fasm, "\tmovl\t$%s, %s(%%rip) \n", tac->op2->text, tac->res->text);
		}
		else
        {
			fprintf(fasm, "\tmovl\t%s(%%rip), %%eax \n", tac->op2->text);
			fprintf(fasm, "\tmovl\t%%eax, %s(%%rip) \n", tac->res->text);
		}
	}
	//fprintf(fasm, "\n");

}

void generateParams(AST_NODE *node, AST_NODE *root, FILE *fasm)
{
    int i = 0;

	if(node == 0)
    {
		return;
    }

	if(node->type == AST_FUNC_DEC)
    {
		AST_NODE *pnode = node->son[1];

		while(pnode)
		{
			fprintf(fasm, "%s:\n", pnode->son[0]->symbol->text);

			if(pnode->son[0]->type == AST_INT_DEF)
			{
				fprintf(fasm, "\t.long\t%d\n", 0);
			}

			pnode = pnode->son[1];
		}
	}

	for (i=0; i<MAX_SONS; i++)
    {
		generateParams(node->son[i], root, fasm);
	}
}

void getParams(AST_NODE *node, AST_NODE *root, FILE* fasm)
{
	int i = 0;

	if(node == 0)
    {
		return;
    }

	if(node->type == AST_FUNC_DEC)
    {
		AST_NODE *pnode = node->son[1];

		while(pnode)
		{
			fprintf(fasm, "%s:\n", pnode->son[0]->symbol->text);

			if(pnode->son[0]->type == AST_INT_DEF)
			{
				fprintf(fasm, "\t.long\t%d\n", 0);
			}
			pnode = pnode->son[1];
		}
	}

	for (i=0; i<MAX_SONS; i++)
    {
		getParams(node->son[i], root, fasm);
	}
}

void getPrintParams(AST_NODE *node, AST_NODE *root, int *count , FILE *fasm)
{
    int i = 0;

    if(count != 0)
    {
        fprintf(fasm, "\t.section\t.rodata\n");
    }

	if(node == 0)
    {
		return;
    }

	if(node->type == AST_PRINT)
    {
		AST_NODE *pnode = node->son[0];

		while(pnode)
		{
			fprintf(fasm, ".LC%d:\n", *count);

			if(pnode->son[0]->type != AST_STRING)
			{
				AST_NODE *dec = findDecNode(pnode->son[0]->symbol->text, root);

				if(dec->son[0]->type == AST_INT_TYPE)
				{
					fprintf(fasm, "\t.string\t\"%%d\"\n");
                }
				else if(dec->son[0]->type == AST_FLOAT_TYPE)
				{
					fprintf(fasm, "\t.string\t\"%%f\"\n");
				}
				else if(dec->son[0]->type == AST_CHAR_TYPE)
				{
					fprintf(fasm, "\t.string\t\"%%c\"\n");
				}
			}
			else
			{
				fprintf(fasm, "\t.string\t%s\n", pnode->son[0]->symbol->text);
			}

			pnode->son[0]->symbol->assemblyLabel = *count;

			pnode = pnode->son[1];
			*count = *count + 1;;
			//fprintf(fasm, "\n");
		}
	}

	for (i=0; i<MAX_SONS; i++)
    {
		getPrintParams(fasm, node->son[i], root, count);
	}
}

void generateASM(TAC *tac, char *filename)
{
    FILE *fasm;
    TAC *initTac = tac; //Salvar tac inicial.
    AST_NODE *getAST(void);

    AST_NODE *root = getAST();

    int vectorIndex = 0;
    char *vectorPrint[MAX_VECTOR_PRINT_SIZE];

    if(!(fasm = fopen(filename, "w")))
    {
        fprintf(stderr, "Cannot open outputfile %s\n", filename);
        exit(5);
    }

//DECLARATIONS
//VAR AND VEC DECS
    getDec(root, fasm);
//VAR TEMPS
    generateVarTemp(root, fasm);
//GETTING PARAMS
    getParams(root, root, fasm);
    getPrintParams(root, root, &count, fasm);
/////////////////

//PRINT PARAMS
    // while(tac=tac->next)
    // {
	// 	switch(tac->type)
    //     {
	// 		case TAC_PRINT :
    //         {
    //         //Criando .LCx antes do codigo, necessario para prints
    //             if(rodataFlag != 1)
    //             {
    //                 fprintf(fasm, "\t.section\t.rodata\n");
    //                 rodataFlag = 1;
    //             }
    //
	// 			if((tac->res) && (tac->res->text[0] == '"'))
    //             {
    //                 if(tac->res->text)
    //                 {
    // 					fprintf(fasm,".LC%d:\n"
    //                     "\t.string %s\n", vectorIndex, tac->res->text);
    // 					vectorPrint[vectorIndex] = tac->res->text;
    // 					vectorIndex++;
    //                 }
	// 			}
    //         } break;
    //         default : break;
    //     }
    // }

//INSIDE
    while(initTac = initTac->next)
    {
        if(initTac->type == TAC_SYMBOL)
        {
            continue;
        }

        switch (initTac->type)
        {
            case TAC_BEGIN_FUNCTION :
            {
                printTacInfo("TAC_BEGIN_FUNCTION", initTac);
                if(initTac->res)
                {
                    fprintf(fasm, "\t.globl	%s\n"
                    "%s:\n"
                        "\t.cfi_startproc\n"
                        "\tpushq	%%rbp\n"
                        "\tmovq	%%rsp, %%rbp", initTac->res ? initTac->res->text : 0, initTac->res ? initTac->res->text : 0);
                }
            } break;

            case TAC_END_FUNCTION :
            {
                printTacInfo("TAC_END_FUNCTION", initTac);
                fprintf(fasm, "\tpopq	%%rbp\n"
                "\tret\n"
                "\t.cfi_endproc\n");
            } break;

            case TAC_PRINT : printTacInfo("TAC_PRINT", initTac); generatePrint(root, initTac, fasm); break;
            case TAC_ADD : printTacInfo("TAC_ADD", initTac); generateExpression(initTac, fasm); break;
            case TAC_SUB : printTacInfo("TAC_SUB", initTac); generateExpression(initTac, fasm); break;
            case TAC_MUL : printTacInfo("TAC_MUL", initTac); generateExpression(initTac, fasm); break;
            case TAC_DIV : printTacInfo("TAC_DIV", initTac); generateExpression(initTac, fasm); break;
            case TAC_ATTRIB : printTacInfo("TAC_ATTRIB", initTac); generateVarAttrib(initTac, root, fasm); break;
            case TAC_LESS : printTacInfo("TAC_LESS", initTac); generateBool(initTac, fasm); break;
            case TAC_GREATER : printTacInfo("TAC_GREATER", initTac); generateBool(initTac, fasm); break;
            case TAC_GEQUAL : printTacInfo("TAC_GEQUAL", initTac); generateBool(initTac, fasm); break;
            case TAC_LEQUAL : printTacInfo("TAC_LEQUAL", initTac); generateBool(initTac, fasm); break;
            case TAC_EQUAL : printTacInfo("TAC_EQUAL", initTac); generateBool(initTac, fasm); break;
            case TAC_VEC_WRITE : printTacInfo("TAC_VEC_WRITE", initTac); generateVecWrite(initTac, root, fasm); break;
            case TAC_VEC_READ : printTacInfo("TAC_VEC_READ", initTac); generateVecRead(initTac, root, fasm); break;
            case TAC_RETURN:
            {
                printTacInfo("TAC_RETURN", initTac);
                if(initTac->op1->isLiteral)
                {
                    fprintf(fasm, "\tmovl\t$%s, %%eax\n", initTac->op1->text);
                }
                else
                {
                    fprintf(fasm, "\tmovl\t%s(%%rip), %%eax \n", initTac->op1->text);
                }
                fprintf(fasm, "\n");
            } break;

            case TAC_IFZ: printTacInfo("TAC_IFZ", initTac);fprintf(fasm, "\t.%s\n", initTac->res->text); break;

            case TAC_JUMP: printTacInfo("TAC_JUMP", initTac);fprintf(fasm, "\tjmp\t.%s\n", initTac->res->text); break;

            case TAC_LABEL: printTacInfo("TAC_LABEL", initTac);fprintf(fasm, ".%s:\n", initTac->res->text); break;

            case TAC_FUNC_CALL:
            {
                printTacInfo("TAC_FUNC_CALL", initTac);
                fprintf(fasm, "\tcall\t%s\n"
                              "\tmovl\t%%eax, %s(%%rip)\n", initTac->op1->text, initTac->res->text);
            } break;

            case TAC_ARG_LIST:
            {
                printTacInfo("TAC_ARG_LIST", initTac);
                if(initTac->res->isLiteral)
            	{
            		fprintf(fasm, "\tmovl\t$%s, %%%s\n", initTac->res->text, funReg[atoi(initTac->op2->text)-1]);
            	}
            	else
            	{
            		fprintf(fasm, "\tmovl\t%s(%%rip), %%%s\n", initTac->res->text, funReg[atoi(initTac->op2->text)-1]);
            	}
            } break;

            case TAC_PARAM_LIST: printTacInfo("TAC_PARAM_LIST", initTac);fprintf(fasm, "\tmovl\t%%%s, %%%s(%%rip)\n", funReg[atoi(initTac->op2->text)-1], initTac->res->text); break;

        }
    }
}
