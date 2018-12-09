/**
 * Hash table implementation
 *
 * @author Luís Augusto Weber Mercado [lawmercado@inf.ufrgs.br]
 * @author Nicholas de Aquino Lau [nalau@inf.ufrgs.br]
 */


#include "hash.h"
#include "ast.h"
#include "semantic.h"
#include "y.tab.h"

void astFind(int level, AST_NODE *node, char *text);
AST_NODE* getAST();
int tempFlag = 0;

void hashInit (void)
{
    int i;

    for(i = 0; i < HASH_SIZE; ++i)
    {
        Table[i] = 0;
    }
}

int hashAddress(char *text)
{
    int i;
    int address = 1;

    for(i = 0; i < strlen(text); ++i)
    {
        address = (address*text[i])%HASH_SIZE + 1;
    }

    return address - 1;
}

HASH_NODE* hashInsert(int type, char *text)
{
    HASH_NODE *newnode;
    int address;

    address = hashAddress(text);

    if(newnode = hashFind(text))
    {
        return newnode;
    }
    newnode = (HASH_NODE*) calloc(1, sizeof(HASH_NODE));
    newnode->type = type;
    newnode->isLiteral = 0;
    newnode->assemblyLabel = 0;

    if(type == 1)
    {
        newnode->datatype = DATATYPE_INT;
        newnode->isLiteral = 1;
        //fprintf(stderr, "Definindo datatype int = %d\n", newnode->datatype);
    } else if (type == 2)
    {
        newnode->datatype = DATATYPE_FLOAT;
        newnode->isLiteral = 1;
        //fprintf(stderr, "Definindo datatype float = %d\n", newnode->datatype);
    } else if (type == 3)
    {
        newnode->datatype = DATATYPE_CHAR;
        newnode->isLiteral = 1;
        //fprintf(stderr, "Definindo datatype char = %d\n", newnode->datatype);
    }

    if(tempFlag == 1)
    {
        newnode->datatype = DATATYPE_TEMP;
        tempFlag = 0;
    }

    newnode->text = calloc(strlen(text)+1, sizeof(char));
    strcpy(newnode->text, text);
    newnode->next = Table[address];
    Table[address] = newnode;

    return newnode;
}

HASH_NODE* hashFind(char *text)
{
    HASH_NODE *node;
    int address;

    address = hashAddress(text);

    for(node = Table[address]; node; node = node->next)
    {
        if(!strcmp(text, node->text))
        {
            return node;
        }
    }

    return 0;
}

void hashPrint(void)
{
    HASH_NODE *node;
    int i;

    for(i = 0; i < HASH_SIZE; ++i)
    {
        for(node = Table[i]; node; node = node->next)
        {
            fprintf(stderr, "Table[%d] has %s \n", i, node->text);
        }
    }
}

void hashSetType(char *text, int type)
{
    //fprintf(stderr, "[hashSetType] Entrou hashSetType.\n");

    HASH_NODE *newnode;

    newnode = hashFind(text);
    newnode->type = type;
    //fprintf(stderr, "[hashSetType] Passou hashSetType.\n");
}

void hashCheckUndeclared(void)
{
    HASH_NODE *node;
    int i = 0;
    //fprintf(stderr, "[checkUndeclared] Entrou.\n");
    for(i = 0; i < HASH_SIZE; i++)
    {
        //fprintf(stderr, "[checkUndeclared] 1o for.\n");
        for(node = Table[i]; node; node = node->next)
        {
            //fprintf(stderr, "[checkUndeclared] 2o for.\n");
            astFind(0, getAST(), node->text);
            //fprintf(stderr, "[checkUndeclared] Saiu astFind.\n");
            if(node->type == SYMBOL_TK_IDENTIFIER)
            {
                //fprintf(stderr, "[checkUndeclared] If => Undeclared variable.\n");
                fprintf(stderr, "Undeclared variable %s.\n", node->text);
                SemanticErrorFlag = 1;
                break;
                //exit(4);
            }
            else if(node->type == SYMBOL_LIT_CHAR || node->type == SYMBOL_LIT_FLOAT || node->type == SYMBOL_LIT_INTEGER)
            {
                //fprintf(stderr, "ENTREI ELSE hashCheckUndeclared\n");
                switch (node->type)
                {
                    case SYMBOL_LIT_CHAR: node->datatype = DATATYPE_CHAR; /*fprintf(stderr, "CASE SYMBOL_LIT_CHAR\n");*/ break;
                    case SYMBOL_LIT_INTEGER: node->datatype = DATATYPE_INT; /*fprintf(stderr, "CASE SYMBOL_LIT_INT\n");*/ break;
                    case SYMBOL_LIT_FLOAT: node->datatype = DATATYPE_FLOAT; /*fprintf(stderr, "CASE SYMBOL_LIT_FLOAT\n");*/ break;
                }
                node->type = SYMBOL_SCALAR;
                //fprintf(stderr, "SYMBOL SCALAR DECLARED PARA VARS = %d\n", node->type);
            }
        }
    }
}

HASH_NODE *makeTemp()
{
    tempFlag = 1;
    static int serial = 0;
    static char name[100];
    sprintf(name, "__TEmpP%d", serial++);
    return hashInsert(SYMBOL_TEMP, name);
}

HASH_NODE *makeLabel()
{
    static int serial = 0;
    static char name[100];
    sprintf(name, "Label%d", serial++);
    return hashInsert(SYMBOL_LABEL, name);
}
