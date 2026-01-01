#include <stdlib.h>
#include "Token.c"
#define Nnum 0
#define Nstr 1
#define Nvar 2
#define Nind 3
#define Nunar 4
#define Nbin 5
#define Nroot 6
// extern typedef struct Data;


typedef struct ExpressionNode{
    unsigned int ID;
    void* Node;
} ExpressionNode;

typedef struct StatementsNode{
    unsigned int CountStrings;
    ExpressionNode* Strings;
} StatementsNode;

typedef struct UnarOperationNode{
    Token* Operation;
    ExpressionNode* Operand;
} UnarOperationNode;

typedef struct BinOperationNode{
    Token* Operation;
    ExpressionNode* LNode;
    ExpressionNode* RNode;
} BinOperationNode;

typedef struct Value{
    Data* Value;
} Value;
typedef struct Chars{
    Token* string;
}Chars;
typedef struct Variable{
    Token* Var;
} Variable;
typedef struct Indexator{
    ExpressionNode* Var;
    ExpressionNode* Index;
} Indexator;


void NInit(Value* node, Token* token){
    Data* val = malloc(sizeof(Data));
    TypeTo(val, token->Code);
    node->Value = val;
}
void CInit(Chars* node, Token* string){
    node->string = string;
}
void VInit(Variable* node, Token* var){
    node->Var = var;
}
void INDInit(Indexator* node, ExpressionNode* var, ExpressionNode* index){
    node->Var = var;
    node->Index = index;
}
void BONInit(
    BinOperationNode* bin,
    ExpressionNode* lNode, 
    Token* token, 
    ExpressionNode* rNode
    )
{
    bin->LNode = lNode;
    bin->Operation = token;
    bin->RNode = rNode;
}
void UONInit(UnarOperationNode* operation, Token* token, ExpressionNode* operand){
    operation->Operation = token;
    operation->Operand = operand;
}
void ENInit(ExpressionNode* expression, unsigned int id, void* node){
    expression->ID = id;
    expression->Node = node;
}
void RInit(StatementsNode* root, unsigned int len){
    root->Strings = calloc(len, sizeof(ExpressionNode));
    root->CountStrings = 0;
}

void AddString(StatementsNode* root, ExpressionNode* string){
    root->Strings[root->CountStrings].ID = string->ID;
    root->Strings[root->CountStrings++].Node = string->Node;
}
void DelRoot(StatementsNode* root){
    free(root);
    root = NULL;
}