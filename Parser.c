#include <stdio.h>
#include "Dictionary.c"
#include "ExpressionNode.c"
extern TokenType tokenType[];


// String NameExpressions[] = {{"Number"},
//                             {"Variable"},
//                             {"UnarOperation"},
//                             {"BinOperation"},
//                             {"Root"}};


typedef struct{
    unsigned int IsCanGo;
    unsigned int NumberString;
    unsigned int Pos;
    unsigned int LenTokens;
    Token* Tokens;
    Dictionary* DictionaryVariable;
    Dictionary* ValueInt;
    Dictionary* ValueFloat;
    Dictionary* ValueBool;
    Dictionary* ValueChar;
}Parser;


void ThrowParser(
    Parser* parser,
    unsigned int condition,
    unsigned int pos,
    char* massage
    )
{
    if(condition == 1){
        printf("Parser; String: %d; Pos: %d; El: %s\n", parser->NumberString, pos, parser->Tokens[pos].Code);
        printf("%s\n", massage);
        getchar();
        exit(0);
    }
}
//unsigned int MaxValue(Parser* parser){}

void ParserInit(Parser* parser, unsigned int lenTokens, Token* tokens){

    parser->IsCanGo = 1;
    parser->NumberString = 0;
    parser->Pos = 0;
    parser->Tokens = tokens;
    parser->LenTokens = lenTokens;
    parser->DictionaryVariable = malloc(sizeof(Dictionary));
    parser->ValueInt = malloc(sizeof(Dictionary));
    parser->ValueFloat = malloc(sizeof(Dictionary));
    parser->ValueBool = malloc(sizeof(Dictionary));
    parser->ValueChar = malloc(sizeof(Dictionary));
    ThrowParser(parser, parser->DictionaryVariable == NULL, parser->Pos, "Dic malloc error");
    unsigned int maxNames = 0;
    for(unsigned int i = 1; i < lenTokens; i++){
        if(IsEqStr(tokens[i].Type.Name, tokenType[12].Name) == 1){
            maxNames++;
        }
    }
    String names[maxNames];
    unsigned int maxVariable = 0;
    unsigned int isInNames = 1;
    for(unsigned int i = 2; i < lenTokens; i++){
        if(IsEqStr(tokens[i].Type.Name, tokenType[12].Name) == 1){
            if(IsEqStr(tokens[i-1].Type.Name, tokenType[24].Name) == 1){
                for(unsigned int ind = 0; ind < maxVariable; ind++){
                    if(IsEqStr(names[ind].word, tokens[i-1].Code) == 1){
                        isInNames = 0;
                        break;
                    }
                }
                if (isInNames == 1){
                    names[maxVariable].word = tokens[i-1].Code;
                    maxVariable++;
                }
                isInNames = 1;
            }
        }
    }
    printf("Memory:\nCount Variables: %d\n\n", maxVariable);

    Data* fv = malloc(sizeof(Data));
    DataInit(fv, 1, 0);
    IntTo(fv, -1);

    DicInit(parser->DictionaryVariable, maxVariable, 0, "r", fv);
    DicInit(parser->ValueInt, maxVariable, 0, "r", fv);
    DicInit(parser->ValueFloat, maxVariable, 1, "r", fv);
    DicInit(parser->ValueBool, maxVariable, 2, "r", fv);
    DicInit(parser->ValueChar, maxVariable, 3, "r", fv);
}

Token* GetToken(Parser* parser, unsigned int len, TokenType* types){
    if(parser->Pos < parser->LenTokens){
        Token* currentToken = malloc(sizeof(Token));
        ThrowParser(parser, currentToken == NULL, parser->Pos, "Token malloc error");
        *currentToken = parser->Tokens[parser->Pos];
        unsigned int isInTokens = 0;
        for(unsigned int i = 0; i < len; i++){
            if(types[i].Name == currentToken->Type.Name){
                isInTokens = 1;
                break;
            }
        }
        if(isInTokens == 1){
            parser->Pos++;
            return currentToken;
        }
    }
    return NULL;
}

void Requier(Parser* parser, unsigned int len, TokenType* types){
    Token* token = GetToken(parser, len, types);
    if(token == NULL){
        ThrowParser(parser, 1, parser->Pos, "Requier error");
        return;
    }
    free(token);
}

int GetLabel(Parser* parser, unsigned int Pos ){
    unsigned int numberString = 1;
    for(unsigned int i = 0; i < Pos; i++){
        if(parser->Tokens[i].Type.Name == tokenType[18].Name){
            numberString++;
        }
    }
    return numberString;
}

ExpressionNode* ParsePerentheses(Parser* parser);

ExpressionNode* ParseFormula(Parser* parser){
    ExpressionNode* leftNode = ParsePerentheses(parser);
    TokenType types[] = {tokenType[4],
                         tokenType[5],
                         tokenType[6],
                         tokenType[7],
                         tokenType[8],
                         tokenType[9],
                         tokenType[10],
                         tokenType[11],};
    Token* operation = GetToken(parser, 8, &types);
    while(operation != NULL){
        ExpressionNode* rightNode = ParsePerentheses(parser);
        BinOperationNode* bin = malloc(sizeof(BinOperationNode));
        ThrowParser(parser, bin == NULL, parser->Pos, "Bin malloc error");
        bin->LNode = malloc(sizeof(ExpressionNode));
        ThrowParser(parser, bin->LNode == NULL, parser->Pos, "LNode malloc error");
        ENInit(bin->LNode, leftNode->ID, leftNode->Node);
        BONInit(bin, bin->LNode, operation, rightNode);
        ENInit(leftNode, 3, bin);
        operation = GetToken(parser, 8, &types);
    }
    return leftNode;
}

ExpressionNode* ParseVariableOrNumberOrOperation(Parser* parser){
    Token* token = GetToken(parser, 1, &tokenType[15]);
    if (token != NULL){
        Number* var = malloc(sizeof(Number));
        ThrowParser(parser, var == NULL, parser->Pos, "Number malloc error");
        NInit(var, token);
        ExpressionNode* node = malloc(sizeof(ExpressionNode));
        ThrowParser(parser, node == NULL, parser->Pos, "NumberNode malloc error");
        ENInit(node, 0, var);
        return node;
    }
    token = GetToken(parser, 1, &tokenType[16]);
    if (token != NULL){
        Number* var = malloc(sizeof(Number));
        ThrowParser(parser, var == NULL, parser->Pos, "Number malloc error");
        NInit(var, token);
        ExpressionNode* node = malloc(sizeof(ExpressionNode));
        ThrowParser(parser, node == NULL, parser->Pos, "NumberNode malloc error");
        ENInit(node, 0, var);
        return node;
    }
    token = GetToken(parser, 1, &tokenType[17]);
    if (token != NULL){
        Number* var = malloc(sizeof(Number));
        ThrowParser(parser, var == NULL, parser->Pos, "Number malloc error");
        NInit(var, token);
        ExpressionNode* node = malloc(sizeof(ExpressionNode));
        ThrowParser(parser, node == NULL, parser->Pos, "NumberNode malloc error");
        ENInit(node, 0, var);
        return node;
    }
    token = GetToken(parser, 1, &tokenType[19]);
    if (token != NULL){
        Number* var = malloc(sizeof(Number));
        ThrowParser(parser, var == NULL, parser->Pos, "Number malloc error");
        NInit(var, token);
        ExpressionNode* node = malloc(sizeof(ExpressionNode));
        ThrowParser(parser, node == NULL, parser->Pos, "NumberNode malloc error");
        ENInit(node, 0, var);
        return node;
    }
    token = GetToken(parser, 1, &tokenType[24]);
    if (token != NULL){
        Variable* var = malloc(sizeof(Variable));
        ThrowParser(parser, var == NULL, parser->Pos, "Variable malloc error");
        VInit(var, token);
        ExpressionNode* node = malloc(sizeof(ExpressionNode));
        ThrowParser(parser, node == NULL, parser->Pos, "VariableNode malloc error");
        ENInit(node, 1, var);
        return node;
    }
    token = GetToken(parser, 1, &tokenType[4]);
    if (token != NULL){
        UnarOperationNode* var = malloc(sizeof(UnarOperationNode));
        ThrowParser(parser, var == NULL, parser->Pos, "Unar malloc error");
        UONInit(var, token, ParseFormula(parser));
        ExpressionNode* node = malloc(sizeof(ExpressionNode));
        ThrowParser(parser, node == NULL, parser->Pos, "UnarNode malloc error");
        ENInit(node, 2, var);
        return node;
    }
    token = GetToken(parser, 1, &tokenType[0]);
    if (token != NULL){
        sprintf(token->Code, "%d", GetLabel(parser, parser->Pos));
        Number* var = malloc(sizeof(Number));
        ThrowParser(parser, var == NULL, parser->Pos, "Number malloc error");
        NInit(var, token);
        ExpressionNode* node = malloc(sizeof(ExpressionNode));
        ThrowParser(parser, node == NULL, parser->Pos, "NumberNode malloc error");
        ENInit(node, 0, var);
        return node;
    }
    ThrowParser(parser, 1, parser->Pos, "ErrorValue");
}

ExpressionNode* ParsePerentheses(Parser* parser){
    if(GetToken(parser, 1, &tokenType[13]) != NULL){
        ExpressionNode* node = ParseFormula(parser);
        Requier(parser, 1, &tokenType[14]);
        return node;
    }
    else{
        return ParseVariableOrNumberOrOperation(parser);
    }
}

ExpressionNode* ParseUnarOperation(Parser* parser, TokenType* type){
    Token* operation = GetToken(parser, 1, type);
    if (operation != NULL){
        UnarOperationNode* operationNode = malloc(sizeof(UnarOperationNode));
        ThrowParser(parser, operation == NULL, parser->Pos, "Unar malloc error");
        UONInit(operationNode, operation, ParseFormula(parser));
        ExpressionNode* node = malloc(sizeof(ExpressionNode));
        ThrowParser(parser, node == NULL, parser->Pos, "UnarNode malloc error");
        ENInit(node, 2, operationNode);
        return node;
    }
    printf("error Unar\n");
}

ExpressionNode* ParseVariable(Parser* parser){
    ExpressionNode* var = ParseVariableOrNumberOrOperation(parser);
    Token* assignOperation = GetToken(parser, 1, &tokenType[12]);

    if (assignOperation != NULL){
        ExpressionNode* value = ParseFormula(parser);

        BinOperationNode* operation = malloc(sizeof(BinOperationNode));
        ThrowParser(parser, operation == NULL, parser->Pos, "Bin malloc error");
        operation->LNode = malloc(sizeof(ExpressionNode));
        ThrowParser(parser, operation->LNode == NULL, parser->Pos, "BinNode malloc error");
        ENInit(operation->LNode, var->ID, var->Node);
        BONInit(operation, operation->LNode, assignOperation, value);

        ExpressionNode* node = malloc(sizeof(ExpressionNode));
        ThrowParser(parser, node == NULL, parser->Pos, "BinNode malloc error");
        ENInit(node, 3, operation);
        return node;
    }
    printf("errorVar\n");
}
ExpressionNode* ParseCreateVariable(Parser* parser){
    TokenType types[] = {tokenType[20], tokenType[21], tokenType[22], tokenType[23]};
    Token* typeVar = GetToken(parser, sizeof(types)/sizeof(TokenType), &types);
    ExpressionNode* var = ParseVariableOrNumberOrOperation(parser);
    Token* assignOperation = GetToken(parser, 1, &tokenType[12]);

    if (assignOperation != NULL){
        ExpressionNode* value = ParseFormula(parser);
        ((Variable*)(var->Node))->Type = typeVar;
        BinOperationNode* operation = malloc(sizeof(BinOperationNode));
        ThrowParser(parser, operation == NULL, parser->Pos, "Bin malloc error");
        operation->LNode = malloc(sizeof(ExpressionNode));
        ThrowParser(parser, operation->LNode == NULL, parser->Pos, "BinNode malloc error");
        ENInit(operation->LNode, var->ID, var->Node);
        BONInit(operation, operation->LNode, assignOperation, value);
        
        ExpressionNode* node = malloc(sizeof(ExpressionNode));
        ThrowParser(parser, node == NULL, parser->Pos, "BinNode malloc error");
        ENInit(node, 3, operation);
        return node;
    }
    ThrowParser(parser, 1, parser->Pos, "ErrorVar");
}

ExpressionNode* ParseExpression(Parser* parser){
    TokenType not[] = {tokenType[24], tokenType[3], tokenType[2], tokenType[1]};
    if(GetToken(parser, 4, &not) == NULL){return ParseCreateVariable(parser);}
    parser->Pos--;

    if(GetToken(parser, 3, &not) == NULL){return ParseUnarOperation(parser, &tokenType[1]);}
    parser->Pos--;

    if(GetToken(parser, 2, &not) == NULL){return ParseUnarOperation(parser, &tokenType[2]);}
    parser->Pos--;

    if(GetToken(parser, 1, &not) == NULL){return ParseUnarOperation(parser, &tokenType[3]);}
    parser->Pos--;

    return ParseVariable(parser);
}

ExpressionNode* ParseCode(Parser* parser){
    StatementsNode* root = malloc(sizeof(StatementsNode));
    RInit(root, GetLabel(parser, parser->LenTokens-1));
    while(parser->Pos < parser->LenTokens){
        ExpressionNode* stringNode = ParseExpression(parser);
        Requier(parser, 1, &tokenType[18]);
        Add(root, stringNode);
    }
    ExpressionNode* node = malloc(sizeof(ExpressionNode));
    ThrowParser(parser, node == NULL, parser->Pos, "RootNode malloc error");
    ENInit(node, 4, root);
    return node;
}

Data* Run(Parser* parser, ExpressionNode* node){
    if(node->ID == 0){
        Data* num = ((Number*)(node->Node))->number;
        return num;
    }
    if(node->ID == 1){
        Variable var = *(Variable*)(node->Node);
        if (TryGetValue(parser->DictionaryVariable, var.Var->Code) == 1){
            unsigned int types = ToInt(GetValue(parser->DictionaryVariable, var.Var->Code));
            if (types == 0){
                if(TryGetValue(parser->ValueInt, var.Var->Code) == 1){
                    return GetValue(parser->ValueInt, var.Var->Code);
                }
            }
            else if (types == 1){
                if(TryGetValue(parser->ValueFloat, var.Var->Code) == 1){
                    return GetValue(parser->ValueFloat, var.Var->Code);
                }
            }
            else if (types == 2){
                if(TryGetValue(parser->ValueBool, var.Var->Code) == 1){
                    return GetValue(parser->ValueBool, var.Var->Code);
                }
            }
            else if (types == 3){
                if(TryGetValue(parser->ValueChar, var.Var->Code) == 1){
                    return GetValue(parser->ValueChar, var.Var->Code);
                }
            }
        }
        char m[24 + sizeof(var.Var->Code)];
        sprintf(m, "Variable %s is not created", var.Var->Code);
        ThrowParser(parser, var.Var->Code != NULL, var.Var->ParserPos, m);
        return NULL;
    }
    if(node->ID == 2){
        UnarOperationNode unar = *(UnarOperationNode*)(node->Node);
        if(unar.operation->Type.Name == tokenType[1].Name){
            Data* value = Run(parser, unar.operand);
            if (value->Type == 0){printf("%d\n", ToInt(value));}
            else if (value->Type == 1){printf("%f", ToFloat(value));}
            else if (value->Type == 2){printf("%s", ToBool(value) == 1 ? "true" : "false");}
            else if (value->Type == 3){printf("%c", ToChar(value));}
            return NULL;
        }
        if(unar.operation->Type.Name == tokenType[3].Name){
            if(parser->IsCanGo == 1){
                Data* Operand = Run(parser, unar.operand);
                if (Operand->Type == 1){
                    parser->NumberString = (int)ToFloat(Operand) - 1;
                }
                else{
                    parser->NumberString = ToInt(Operand) - 1;
                }
            }
            parser->IsCanGo = 1;
            return NULL;
        }
        if(unar.operation->Type.Name == tokenType[2].Name){
            Data* Operand = Run(parser, unar.operand);
            if (Operand->Type == 1){
                parser->IsCanGo = (ToFloat(Operand) != 0) ? 1 : 0;
            }
            else{
                parser->IsCanGo = (ToInt(Operand) != 0) ? 1 : 0;
            }
            return NULL;
        }
        if(unar.operation->Type.Name == tokenType[4].Name){
            Data* Operand = Run(parser, unar.operand);
            Data* result = malloc(sizeof(Data));
            if (Operand->Type == 1){
                DataInit(result, 2, 1);
                FloatTo(result, (ToFloat(Operand) != 0) ? 0 : 1);
            }
            else{
                DataInit(result, 1, 0);
                IntTo(result, (ToInt(Operand) != 0) ? 0 : 1);
            }
            return result;
        }
    }
    if(node->ID == 3){
        BinOperationNode* bin = (BinOperationNode*)(node->Node);
        if(IsEqStr(bin->operation->Type.Name, tokenType[7].Name)){
            Data* LValue = Run(parser, bin->LNode);
            Data* RValue = Run(parser, bin->RNode);
            Data* result = malloc(sizeof(Data));
            if (LValue->Type == 1 || RValue->Type == 1){
                DataInit(result, 2, 1);
                FloatTo(result, ToFloat(LValue) + ToFloat(RValue));
            }
            else{
                DataInit(result, 1, 0);
                IntTo(result, ToInt(LValue) + ToInt(RValue));
            }
            return result;
        }
        if(bin->operation->Type.Name == tokenType[8].Name){
            Data* LValue = Run(parser, bin->LNode);
            Data* RValue = Run(parser, bin->RNode);
            Data* result = malloc(sizeof(Data));
            if (LValue->Type == 1 || RValue->Type == 1){
                DataInit(result, 2, 1);
                FloatTo(result, ToFloat(LValue) - ToFloat(RValue));
            }
            else{
                DataInit(result, 1, 0);
                IntTo(result, ToInt(LValue) - ToInt(RValue));
            }
            return result;
        }
        if(bin->operation->Type.Name == tokenType[5].Name){
            Data* LValue = Run(parser, bin->LNode);
            Data* RValue = Run(parser, bin->RNode);
            Data* result = malloc(sizeof(Data));
            if (LValue->Type == 1 || RValue->Type == 1){
                DataInit(result, 2, 1);
                FloatTo(result, (ToFloat(LValue) != 0) && (ToFloat(RValue) != 0) ? 1 : 0);
            }
            else{
                DataInit(result, 1, 0);
                IntTo(result, (ToInt(LValue) != 0) && (ToInt(RValue) != 0) ? 1 : 0);
            }
            return result;
        }
        if(bin->operation->Type.Name == tokenType[6].Name){
            Data* LValue = Run(parser, bin->LNode);
            Data* RValue = Run(parser, bin->RNode);
            Data* result = malloc(sizeof(Data));
            if (LValue->Type == 1 || RValue->Type == 1){
                DataInit(result, 2, 1);
                FloatTo(result, (ToFloat(LValue) != 0) || (ToFloat(RValue) != 0) ? 1 : 0);
            }
            else{
                DataInit(result, 1, 0);
                IntTo(result, (ToInt(LValue) !=0 ) || (ToInt(RValue) != 0) ? 1 : 0);
            }
            return result;
        }
        if(bin->operation->Type.Name == tokenType[9].Name){
            Data* LValue = Run(parser, bin->LNode);
            Data* RValue = Run(parser, bin->RNode);
            Data* result = malloc(sizeof(Data));
            if (LValue->Type == 1 || RValue->Type == 1){
                DataInit(result, 2, 1);
                FloatTo(result, (ToFloat(LValue) == ToFloat(RValue)) ? 1 : 0);
            }
            else{
                DataInit(result, 1, 0);
                IntTo(result, (ToInt(LValue) == ToInt(RValue)) ? 1 : 0);
            }
            return result;
        }
        if(bin->operation->Type.Name == tokenType[10].Name){
            Data* LValue = Run(parser, bin->LNode);
            Data* RValue = Run(parser, bin->RNode);
            Data* result = malloc(sizeof(Data));
            if (LValue->Type == 1 || RValue->Type == 1){
                DataInit(result, 2, 1);
                FloatTo(result, (ToFloat(LValue) > ToFloat(RValue)) ? 1 : 0);
            }
            else{
                DataInit(result, 1, 0);
                IntTo(result, (ToInt(LValue) > ToInt(RValue)) ? 1 : 0);
            }
            return result;
        }
        if(bin->operation->Type.Name == tokenType[11].Name){
            Data* LValue = Run(parser, bin->LNode);
            Data* RValue = Run(parser, bin->RNode);
            Data* result = malloc(sizeof(Data));
            if (LValue->Type == 1 || RValue->Type < 1){
                DataInit(result, 2, 1);
                FloatTo(result, (ToFloat(LValue) < ToFloat(RValue)) ? 1 : 0);
            }
            else{
                DataInit(result, 1, 0);
                IntTo(result, (ToInt(LValue) < ToInt(RValue)) ? 1 : 0);
            }
            return result;
        }
        if(bin->operation->Type.Name == tokenType[12].Name){
            Variable* var = (Variable*)(bin->LNode->Node);
            char* name = var->Var->Code;
            Data* result = Run(parser, bin->RNode);
            unsigned int types = 0;
            if (var->Type != NULL){
                if (IsEqStr(var->Type->Code, "float") == 1){types = 1;}
                else if(IsEqStr(var->Type->Code, "bool") == 1){types = 2;}
                else if(IsEqStr(var->Type->Code, "char") == 1){types = 3;}
                Data* type = malloc(sizeof(Data));
                DataInit(type, 1, 0);
                IntTo(type, types);
                DictionaryAdd(parser->DictionaryVariable, type, name);
            }else{
                if(TryGetValue(parser->DictionaryVariable, name)){
                    types = ToInt(GetValue(parser->DictionaryVariable, name));
                }
            }
            if(types == 0){
                DictionaryAdd(parser->ValueInt, result, name);
            }
            else if (types == 1){
                DictionaryAdd(parser->ValueFloat, result, name);
            }
            else if (types == 2){
                DictionaryAdd(parser->ValueBool, result, name);
            }
            else if (types == 3){
                DictionaryAdd(parser->ValueChar, result, name);
            }
            return result;
        }
    }
    if (node->ID == 4){
        StatementsNode code = *(StatementsNode*)(node->Node);
        while(parser->NumberString < code.CountStrings){
            Run(parser, &code.Strings[parser->NumberString]);
            parser->NumberString++;
        }
        return NULL;
    }
}

void DelParser(Parser* parser){
    for (unsigned int i = 0; i < parser->LenTokens; i++){
        free(parser->Tokens[i].Code); parser->Tokens[i].Code = NULL;
    }
    free(parser->Tokens); parser->Tokens = NULL;
    free(parser); parser = NULL;
}