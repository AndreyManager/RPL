#include <stdlib.h>
#include "Token.c"
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
    Token* operation;
    ExpressionNode* operand;
} UnarOperationNode;

typedef struct BinOperationNode{
    Token* operation;
    ExpressionNode* LNode;
    ExpressionNode* RNode;
} BinOperationNode;

typedef struct Number{
    Data* number;
} Number;

typedef struct Variable{
    Token* Var;
    Token* Type;
} Variable;


void NInit(Number* node, Token* number){
    Data* num = malloc(sizeof(Data));
    TypeTo(num, number->Code);
    node->number = num;
}

void VInit(Variable* node, Token* var){
    node->Var = var;
}

void BONInit(
    BinOperationNode* bin,
    ExpressionNode* lNode, 
    Token* token, 
    ExpressionNode* rNode
    )
{
    bin->LNode = lNode;
    bin->operation = token;
    bin->RNode = rNode;
}

void UONInit(UnarOperationNode* operation, Token* token, ExpressionNode* operand){
    operation->operation = token;
    operation->operand = operand;
}

void ENInit(ExpressionNode* expression, unsigned int id, void* node){
    expression->ID = id;
    expression->Node = node;
}

void RInit(StatementsNode* root, unsigned int len){
    root->Strings = calloc(len, sizeof(ExpressionNode));
    root->CountStrings = 0;
}

void Add(StatementsNode* root, ExpressionNode* string){
    root->Strings[root->CountStrings].ID = string->ID;
    root->Strings[root->CountStrings++].Node = string->Node;
}
void DelRoot(StatementsNode* root){
    free(root);
    root = NULL;
}