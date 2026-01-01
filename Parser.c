#include <stdio.h>
#include "Dictionary.c"
#include "ExpressionNode.c"
#include "List.c"
//#include "String.c"
extern TokenType tokenType[];


typedef struct{
    int IsCanGo;
    int NumberString;
    int Pos;
    int LenTokens;
    int LenLists;
    Token* Tokens;
    Dictionary* DictionaryVariable;
    List* Lists;
}Parser;


void ThrowParser(Parser* parser, int condition, Token* token, char* massage)
{
    if(condition == 1){
        if (token != NULL){
            printf("Parser; String: %d; Pos: %d; El: %s\n", 
                    parser->NumberString, token->ParserPos + 1, token->Code);
        }
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
    parser->LenLists = 0;
    parser->DictionaryVariable = malloc(sizeof(Dictionary));
    ThrowParser(parser, parser->DictionaryVariable == NULL, NULL, "Dic malloc error");
    int maxNames = 0;
    for(unsigned int i = 1; i < lenTokens; i++){
        if(IsEqStr(tokens[i].Type.Name, tokenType[12].Name) == 1){
            maxNames++;
        }
    }
    String names[maxNames];
    int maxVariable = 0;
    int maxList = 0;
    int isInNames = 1;
    for(unsigned int i = 2; i < lenTokens; i++){
        if(IsEqStr(tokens[i].Type.Name, tokenType[Tassign].Name) == 1){
            if(IsEqStr(tokens[i-1].Type.Name, tokenType[Tvar].Name) == 1){
                for(int ind = 0; ind < maxVariable; ind++){
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
        else if(IsEqStr(tokens[i].Type.Name, tokenType[Tarr].Name) == 1){
            maxList++;
        }
    }
    printf("Memory:\nCount Variables: %d\nCount List: %d\n\n", maxVariable, maxList);

    Data* fv = malloc(sizeof(Data));
    DataInit(fv, 1, 0);
    IntTo(fv, -1);
    DicInit(parser->DictionaryVariable, maxVariable, 0, "r", fv);
    parser->Lists = calloc(maxList, sizeof(List)); 
}

Token* GetCurrentTokenIfIn(Parser* parser, unsigned int len, TokenType* types){
    if(parser->Pos < parser->LenTokens){
        Token* currentToken = malloc(sizeof(Token));
        ThrowParser(parser, currentToken == NULL, &parser->Tokens[parser->Pos], "Token malloc error");
        *currentToken = parser->Tokens[parser->Pos];
        unsigned int isInTokens = 0;
        for(unsigned int i = 0; i < len; i++){
            if(IsEqStr(types[i].Name, currentToken->Type.Name)){
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
    Token* token = GetCurrentTokenIfIn(parser, len, types);
    if(token == NULL){
        printf("F2\n");
        ThrowParser(parser, 1, &parser->Tokens[parser->Pos - 1], "Requier error");
        return;
    }
    free(token);
}

int GetLabel(Parser* parser, unsigned int Pos ){
    unsigned int numberString = 1;
    for(unsigned int i = 0; i < Pos; i++){
        if(parser->Tokens[i].Type.Name == tokenType[Tsemicolon].Name){
            numberString++;
        }
    }
    return numberString;
}

ExpressionNode* ParseParentheses(Parser* parser);

ExpressionNode* ParseFormula(Parser* parser){
    ExpressionNode* leftNode = ParseParentheses(parser);
    TokenType types[] = {tokenType[Tnot],
                         tokenType[Tand],
                         tokenType[Tor],
                         tokenType[Tadd],
                         tokenType[Tsub],
                         tokenType[Teq],
                         tokenType[Tgt],
                         tokenType[Tlt]};
    Token* operation = GetCurrentTokenIfIn(parser, 8, &types);
    while(operation != NULL){
        ExpressionNode* rightNode = ParseParentheses(parser);
        BinOperationNode* bin = malloc(sizeof(BinOperationNode));
        ThrowParser(parser, bin == NULL, &parser->Tokens[parser->Pos], "Bin malloc error");
        bin->LNode = malloc(sizeof(ExpressionNode));
        ThrowParser(parser, bin->LNode == NULL, &parser->Tokens[parser->Pos], "LNode malloc error");
        ENInit(bin->LNode, leftNode->ID, leftNode->Node);
        BONInit(bin, bin->LNode, operation, rightNode);
        ENInit(leftNode, Nbin, bin);
        operation = GetCurrentTokenIfIn(parser, 8, &types);
    }
    return leftNode;
}
ExpressionNode* GetNodeUnaGerOperation(Parser* parser, Token* operation){
    UnarOperationNode* operationNode = malloc(sizeof(UnarOperationNode));
    ThrowParser(parser, operation == NULL, &parser->Tokens[parser->Pos], "Unar malloc error");
    UONInit(operationNode, operation, ParseFormula(parser));
    ExpressionNode* node = malloc(sizeof(ExpressionNode));
    ThrowParser(parser, node == NULL, &parser->Tokens[parser->Pos], "UnarNode malloc error");
    ENInit(node, Nunar, operationNode);
    return node;
}

ExpressionNode* ParseVariableOrNumberOrOperation(Parser* parser){
    Token* token = GetCurrentTokenIfIn(parser, 1, &tokenType[Tnumber]);
    if (token != NULL){
        Value* var = malloc(sizeof(Value));
        ThrowParser(parser, var == NULL, &parser->Tokens[parser->Pos], "Value malloc error");
        NInit(var, token);
        ExpressionNode* node = malloc(sizeof(ExpressionNode));
        ThrowParser(parser, node == NULL, &parser->Tokens[parser->Pos], "ValueNode malloc error");
        ENInit(node, Nnum, var);
        return node;
    }
    token = GetCurrentTokenIfIn(parser, 1, &tokenType[Ttrue]);
    if (token != NULL){
        Value* var = malloc(sizeof(Value));
        ThrowParser(parser, var == NULL, &parser->Tokens[parser->Pos], "Value malloc error");
        NInit(var, token);
        ExpressionNode* node = malloc(sizeof(ExpressionNode));
        ThrowParser(parser, node == NULL, &parser->Tokens[parser->Pos], "ValueNode malloc error");
        ENInit(node, Nnum, var);
        return node;
    }
    token = GetCurrentTokenIfIn(parser, 1, &tokenType[Tfalse]);
    if (token != NULL){
        Value* var = malloc(sizeof(Value));
        ThrowParser(parser, var == NULL, &parser->Tokens[parser->Pos], "Value malloc error");
        NInit(var, token);
        ExpressionNode* node = malloc(sizeof(ExpressionNode));
        ThrowParser(parser, node == NULL, &parser->Tokens[parser->Pos], "ValueNode malloc error");
        ENInit(node, Nnum, var);
        return node;
    }
    token = GetCurrentTokenIfIn(parser, 1, &tokenType[Tsymbol]);
    if (token != NULL){
        Value* var = malloc(sizeof(Value));
        int len = 0;
        while(token->Code[len] != '\0') {len++;}
        ThrowParser(parser, len != 4 && len != 3, &parser->Tokens[parser->Pos], "Char value error");
        if (len == 4){
            ThrowParser(parser, token->Code[2] != 'n', &parser->Tokens[parser->Pos], "Char value error");
            token->Code = "\'\n\'";
        }
        ThrowParser(parser, var == NULL, &parser->Tokens[parser->Pos], "Number malloc error");
        NInit(var, token);
        ExpressionNode* node = malloc(sizeof(ExpressionNode));
        ThrowParser(parser, node == NULL, &parser->Tokens[parser->Pos], "NumberNode malloc error");
        ENInit(node, Nnum, var);
        return node;
    }
    token = GetCurrentTokenIfIn(parser, 1, &tokenType[Tstring]);
    if (token != NULL){
        int len = 0;
        while(token->Code[len] != '\0') {len++;}
        len -= 1;
        char* str = calloc(len, sizeof(char));
        for (int i = 0; i < len - 1; i++){
            str[i] = token->Code[i + 1];
        }
        str[len - 1] = '\0';
        token->Code = str;
        Chars* var = malloc(sizeof(Chars));
        ThrowParser(parser, var == NULL, &parser->Tokens[parser->Pos], "String malloc error");
        CInit(var, token);
        ExpressionNode* node = malloc(sizeof(ExpressionNode));
        ThrowParser(parser, node == NULL, &parser->Tokens[parser->Pos], "StringNode malloc error");
        ENInit(node, Nstr, var);
        return node;
    }
    token = GetCurrentTokenIfIn(parser, 1, &tokenType[Tinput]);
    if (token != NULL){
        Chars* var = malloc(sizeof(Chars));
        ThrowParser(parser, var == NULL, &parser->Tokens[parser->Pos], "String malloc error");
        CInit(var, token);
        ExpressionNode* node = malloc(sizeof(ExpressionNode));
        ThrowParser(parser, node == NULL, &parser->Tokens[parser->Pos], "StringNode malloc error");
        ENInit(node, Nstr, var);
        return node;
    }
    token = GetCurrentTokenIfIn(parser, 1, &tokenType[Tvar]);
    if (token != NULL){
        Variable* var = malloc(sizeof(Variable));
        ThrowParser(parser, var == NULL, &parser->Tokens[parser->Pos], "Variable malloc error");
        VInit(var, token);
        ExpressionNode* node = malloc(sizeof(ExpressionNode));
        Token* qper = GetCurrentTokenIfIn(parser, 1, &tokenType[Tlqper]);
        if (qper != NULL){
            ExpressionNode* ind = ParseFormula(parser);
            Indexator* indexator = malloc(sizeof(Indexator));
            ThrowParser(parser, indexator == NULL, &parser->Tokens[parser->Pos], "ind malloc error");
            ExpressionNode* nodeV = malloc(sizeof(ExpressionNode));
            ThrowParser(parser, nodeV == NULL, &parser->Tokens[parser->Pos], "VariableNode malloc error");
            ENInit(nodeV, Nvar, var);
            INDInit(indexator, nodeV, ind);
            
            Requier(parser, 1, &tokenType[Trqper]);
            ThrowParser(parser, node == NULL, &parser->Tokens[parser->Pos], "IndexatorNode malloc error");
            ENInit(node, Nind, indexator);
        }
        else{
            ThrowParser(parser, node == NULL, &parser->Tokens[parser->Pos], "VariableNode malloc error");
            ENInit(node, Nvar, var);
        }
        return node;
    }
    token = GetCurrentTokenIfIn(parser, 1, &tokenType[Tlabel]);
    if (token != NULL){
        sprintf(token->Code, "%d", GetLabel(parser, parser->Pos));
        Value* var = malloc(sizeof(Value));
        ThrowParser(parser, var == NULL, &parser->Tokens[parser->Pos], "Number malloc error");
        NInit(var, token);
        ExpressionNode* node = malloc(sizeof(ExpressionNode));
        ThrowParser(parser, node == NULL, &parser->Tokens[parser->Pos], "NumberNode malloc error");
        ENInit(node, Nnum, var);
        return node;
    }
    int types[] = {Tnot, Tarr, Tlen, Ttype, Tint, Tfloat, Tbool, Tchar};
    for (int i = 0; i < sizeof(types) / sizeof(int); i++){
        token = GetCurrentTokenIfIn(parser, 1, &tokenType[types[i]]);
        if (token != NULL){
            return GetNodeUnaGerOperation(parser, token);
        }
    }
    ThrowParser(parser, 1, &parser->Tokens[parser->Pos], "Error Value");
}

ExpressionNode* ParseParentheses(Parser* parser){
    if(GetCurrentTokenIfIn(parser, 1, &tokenType[Tlper]) != NULL){
        ExpressionNode* node = ParseFormula(parser);
        Requier(parser, 1, &tokenType[Trper]);
        return node;
    }
    else{
        return ParseVariableOrNumberOrOperation(parser);
    }
}

ExpressionNode* ParseUnarOperation(Parser* parser, TokenType* type){
    Token* operation = GetCurrentTokenIfIn(parser, 1, type);
    if (operation != NULL){
        return GetNodeUnaGerOperation(parser, operation);
    }
    ThrowParser(parser, 1, &parser->Tokens[parser->Pos], "Error Unar");
}

ExpressionNode* ParseVariable(Parser* parser){
    ExpressionNode* var = ParseVariableOrNumberOrOperation(parser);
    Token* assignOperation = GetCurrentTokenIfIn(parser, 1, &tokenType[Tassign]);
    if (assignOperation != NULL){
        ExpressionNode* value = ParseFormula(parser);

        BinOperationNode* operation = malloc(sizeof(BinOperationNode));
        ThrowParser(parser, operation == NULL, &parser->Tokens[parser->Pos], "Bin malloc error");
        operation->LNode = malloc(sizeof(ExpressionNode));
        ThrowParser(parser, operation->LNode == NULL, &parser->Tokens[parser->Pos], "BinNode malloc error");
        ENInit(operation->LNode, var->ID, var->Node);
        BONInit(operation, operation->LNode, assignOperation, value);

        ExpressionNode* node = malloc(sizeof(ExpressionNode));
        ThrowParser(parser, node == NULL, &parser->Tokens[parser->Pos], "BinNode malloc error");
        ENInit(node, Nbin, operation);
        return node;
    }
    ThrowParser(parser, 1, &parser->Tokens[parser->Pos], "ErrorVar");
}

ExpressionNode* ParseCurrentString(Parser* parser){
    TokenType not[] = {tokenType[Tvar], 
                       tokenType[Tgoto], 
                       tokenType[Tif], 
                       tokenType[Tlog], 
                       tokenType[Tappend], 
                       tokenType[Tpop]};
    for (int i = 5; i > 0; i--){
        if(GetCurrentTokenIfIn(parser, i, &not) == NULL){return ParseUnarOperation(parser, &not[i]);}
        parser->Pos--;
    }

    return ParseVariable(parser);
}

ExpressionNode* ParseCode(Parser* parser){
    StatementsNode* root = malloc(sizeof(StatementsNode));
    RInit(root, GetLabel(parser, parser->LenTokens-1));
    while(parser->Pos < parser->LenTokens){
        ExpressionNode* stringNode = ParseCurrentString(parser);
        Requier(parser, 1, &tokenType[Tsemicolon]);
        AddString(root, stringNode);
    }
    ExpressionNode* node = malloc(sizeof(ExpressionNode));
    ThrowParser(parser, node == NULL, &parser->Tokens[parser->Pos], "RootNode malloc error");
    ENInit(node, Nroot, root);
    return node;
}
void LogData(Parser* parser, Data* value){
    if (value == NULL){
        printf("null\n");
        return;
    }
    if (value->Type == Vint){printf("%d", ToInt(value));}
    else if (value->Type == Vfloat){printf("%f", ToFloat(value));}
    else if (value->Type == Vbool){printf("%s", ToBool(value) == 1 ? "true" : "false");}
    else if (value->Type == Vchar){printf("%c", ToChar(value));}
    else if (value->Type == Varr){
        List list = parser->Lists[ToArr(value)];
        for (int i = 0; i < list.Len; i++){
            LogData(parser, &list.Values[i]);
        }
    }
    else{
        printf("null\n");
    }
}

Data* Run(Parser* parser, ExpressionNode* node){
    if(node->ID == Nnum){
        Data* num = ((Value*)(node->Node))->Value;
        return num;
    }
    if (node->ID == Nstr){
        Chars* chars = (Chars*)(node->Node);
        if (chars->string->Type.Name == tokenType[Tinput].Name){

            chars->string->Code = GetString();
        }
        char* str = chars->string->Code;
        int len = 0;
        while(str[len] != '\0'){len++;}
        List list;
        ListInit(&list, len);
        parser->Lists[parser->LenLists] = list; // ERROR;

        Data* result = malloc(sizeof(Data));
        DataInit(result, 1, Varr);
        ArrTo(result, parser->LenLists++);
        for (int i = 0; i < len; i++){
            Data* symbol = malloc(sizeof(Data));
            DataInit(symbol, 1, Vchar);
            CharTo(symbol, str[i]);
            list.Values[i] = *symbol;
        }
        return result;
    }
    if(node->ID == Nvar){
        Variable var = *(Variable*)(node->Node);
        if (TryGetValue(parser->DictionaryVariable, var.Var->Code) == 1){
            return GetValue(parser->DictionaryVariable, var.Var->Code);
        }
        char m[24 + sizeof(var.Var->Code)];
        sprintf(m, "Variable %s is not created", var.Var->Code);
        ThrowParser(parser, var.Var->Code != NULL, var.Var->ParserPos, m);
        return NULL;
    }
    if(node->ID == Nind){
        Indexator indexator = *(Indexator*)(node->Node);
        Variable var = *(Variable*)(indexator.Var->Node);
        if (TryGetValue(parser->DictionaryVariable, var.Var->Code) == 1){
            Data* indList = GetValue(parser->DictionaryVariable, var.Var->Code);
            Data* ind = Run(parser, indexator.Index);
            
            if (indList->Type != Varr){
                char m[20 + sizeof(var.Var->Code)];
                sprintf(m, "List %s is not created", var.Var->Code);
                ThrowParser(parser, var.Var->Code != NULL, var.Var, m);
                return NULL;
            }
            if (parser->Lists[ToArr(indList)].Len > ToInt(ind) && ToInt(ind) > -1){
                return &(parser->Lists[ToArr(indList)].Values[ToInt(ind)]);
            }
            ThrowParser(parser, var.Var->Code != NULL, var.Var, "Index beyond the boundaries of array");
        }
        char m[20 + sizeof(var.Var->Code)];
        sprintf(m, "List %s is not created", var.Var->Code);
        ThrowParser(parser, var.Var->Code != NULL, var.Var, m);
        return NULL;
    }
    if(node->ID == Nunar){
        UnarOperationNode unar = *(UnarOperationNode*)(node->Node);
        if(IsEqStr(unar.Operation->Type.Name, tokenType[Tlog].Name)){
            Data* value = Run(parser, unar.Operand);
            LogData(parser, value);
            return NULL;
        }
        if(unar.Operation->Type.Name == tokenType[Tgoto].Name){
            if(parser->IsCanGo == 1){
                Data* Operand = Run(parser, unar.Operand);
                if (Operand->Type == Vfloat){
                    parser->NumberString = (int)ToFloat(Operand) - 1;
                }
                else{
                    parser->NumberString = ToInt(Operand) - 1;
                }
            }
            parser->IsCanGo = 1;
            return NULL;
        }
        if(unar.Operation->Type.Name == tokenType[Tif].Name){
            Data* Operand = Run(parser, unar.Operand);
            if (Operand->Type == Vfloat){
                parser->IsCanGo = (ToFloat(Operand) != 0) ? 1 : 0;
            }
            else{
                parser->IsCanGo = (ToInt(Operand) != 0) ? 1 : 0;
            }
            return NULL;
        }
        if(unar.Operation->Type.Name == tokenType[Tnot].Name){
            Data* operand = Run(parser, unar.Operand);
            Data* result = malloc(sizeof(Data));
            DataInit(result, 1, Vbool);
            BoolTo(result, (ToInt(operand) != 0) ? 0 : 1);
            return result;
        }
        if(unar.Operation->Type.Name == tokenType[Tarr].Name){
            Data* operand = Run(parser, unar.Operand);
            List list;
            ListInit(&list, ToInt(operand));
            parser->Lists[parser->LenLists] = list;  // ERROR;
            Data* result = malloc(sizeof(Data));
            DataInit(result, 1, Varr);
            ArrTo(result, parser->LenLists++);
            return result;
        }
        if (unar.Operation->Type.Name == tokenType[Tlen].Name){
            Data* operand = Run(parser, unar.Operand);
            ThrowParser(parser, operand->Type != Varr, unar.Operation, "ErrorLen");
            Data* result = malloc(sizeof(Data));
            DataInit(result, 1, Vint);
            IntTo(result, parser->Lists[operand->Value[0]].Len);
            return result;
        }
        if (unar.Operation->Type.Name == tokenType[Ttype].Name){
            Data* operand = Run(parser, unar.Operand);
            Data* result = malloc(sizeof(Data));
            DataInit(result, 1, Vint);
            IntTo(result, operand->Type);
            return result;
        }
        if (unar.Operation->Type.Name == tokenType[Tint].Name){
            Data* operand = Run(parser, unar.Operand);
            Data* result = TypeToInt(operand);
            ThrowParser(parser, result == NULL, unar.Operation, "Error int");
            return result;
        }
        if (unar.Operation->Type.Name == tokenType[Tfloat].Name){
            Data* operand = Run(parser, unar.Operand);
            Data* result = TypeToFloat(operand);
            ThrowParser(parser, result == NULL, unar.Operation, "Error float");
            return result;
        }
        if (unar.Operation->Type.Name == tokenType[Tbool].Name){
            Data* operand = Run(parser, unar.Operand);
            Data* result = TypeToBool(operand);
            ThrowParser(parser, result == NULL, unar.Operation, "Error bool");
            return result;
        }
        if (unar.Operation->Type.Name == tokenType[Tchar].Name){
            Data* operand = Run(parser, unar.Operand);
            Data* result = TypeToChar(operand);
            ThrowParser(parser, result == NULL, unar.Operation, "Error char");
            return result;
        }
        if (unar.Operation->Type.Name == tokenType[Tappend].Name){
            Data* operand = Run(parser, unar.Operand);
            ThrowParser(parser, operand->Type != Varr, unar.Operation, "ErrorAppend");
            Add(&parser->Lists[ToArr(operand)]);
            return NULL;
        }
        if (unar.Operation->Type.Name == tokenType[Tpop].Name){
            Data* operand = Run(parser, unar.Operand);
            ThrowParser(parser, operand->Type != Varr, unar.Operation, "ErrorPop");
            Pop(&parser->Lists[ToArr(operand)]);
            return NULL;
        }
    }
    if(node->ID == Nbin){
        BinOperationNode* bin = (BinOperationNode*)(node->Node);
        if(IsEqStr(bin->Operation->Type.Name, tokenType[Tadd].Name)){
            Data* LValue = Run(parser, bin->LNode);
            Data* RValue = Run(parser, bin->RNode);
            Data* result = malloc(sizeof(Data));
            if (LValue->Type == Vfloat || RValue->Type == Vfloat){
                DataInit(result, 2, Vfloat);
                FloatTo(result, ToFloat(LValue) + ToFloat(RValue));
            }
            else{
                DataInit(result, 1, Vint);
                IntTo(result, ToInt(LValue) + ToInt(RValue));
            }
            return result;
        }
        if(bin->Operation->Type.Name == tokenType[Tsub].Name){
            Data* LValue = Run(parser, bin->LNode);
            Data* RValue = Run(parser, bin->RNode);
            Data* result = malloc(sizeof(Data));
            if (LValue->Type == Vfloat || RValue->Type == Vfloat){
                DataInit(result, 2, Vfloat);
                FloatTo(result, ToFloat(LValue) - ToFloat(RValue));
            }
            else{
                DataInit(result, 1, Vint);
                IntTo(result, ToInt(LValue) - ToInt(RValue));
            }
            return result;
        }
        if(bin->Operation->Type.Name == tokenType[Tand].Name){
            Data* LValue = Run(parser, bin->LNode);
            Data* RValue = Run(parser, bin->RNode);
            Data* result = malloc(sizeof(Data));
            DataInit(result, 1, Vbool);
            if (LValue->Type == Vfloat || RValue->Type == Vfloat){
                BoolTo(result, (ToFloat(LValue) != 0) && (ToFloat(RValue) != 0) ? 1 : 0);
            }
            else{
                BoolTo(result, (ToInt(LValue) != 0) && (ToInt(RValue) != 0) ? 1 : 0);
            }
            return result;
        }
        if(bin->Operation->Type.Name == tokenType[Tor].Name){
            Data* LValue = Run(parser, bin->LNode);
            Data* RValue = Run(parser, bin->RNode);
            Data* result = malloc(sizeof(Data));
            DataInit(result, 1, Vbool);
            if (LValue->Type == Vfloat || RValue->Type == Vfloat){
                BoolTo(result, (ToFloat(LValue) != 0) || (ToFloat(RValue) != 0) ? 1 : 0);
            }
            else{
                BoolTo(result, (ToInt(LValue) != 0 ) || (ToInt(RValue) != 0) ? 1 : 0);
            }
            return result;
        }
        if(bin->Operation->Type.Name == tokenType[Teq].Name){
            Data* LValue = Run(parser, bin->LNode);
            Data* RValue = Run(parser, bin->RNode);
            Data* result = malloc(sizeof(Data));
            DataInit(result, 1, Vbool);
            if (LValue->Type == Vfloat || RValue->Type == Vfloat){
                BoolTo(result, (ToFloat(LValue) == ToFloat(RValue)) ? 1 : 0);
            }
            else{
                BoolTo(result, (ToInt(LValue) == ToInt(RValue)) ? 1 : 0);
            }
            return result;
        }
        if(bin->Operation->Type.Name == tokenType[Tgt].Name){
            Data* LValue = Run(parser, bin->LNode);
            Data* RValue = Run(parser, bin->RNode);
            Data* result = malloc(sizeof(Data));
            DataInit(result, 1, Vbool);
            if (LValue->Type == Vfloat || RValue->Type == Vfloat){
                BoolTo(result, (ToFloat(LValue) > ToFloat(RValue)) ? 1 : 0);
            }
            else{
                BoolTo(result, (ToInt(LValue) > ToInt(RValue)) ? 1 : 0);
            }
            return result;
        }
        if(bin->Operation->Type.Name == tokenType[Tlt].Name){
            Data* LValue = Run(parser, bin->LNode);
            Data* RValue = Run(parser, bin->RNode);
            Data* result = malloc(sizeof(Data));
            DataInit(result, 1, Vbool);
            if (LValue->Type == Vfloat || RValue->Type < Vfloat){
                BoolTo(result, (ToFloat(LValue) < ToFloat(RValue)) ? 1 : 0);
            }
            else{
                BoolTo(result, (ToInt(LValue) < ToInt(RValue)) ? 1 : 0);
            }
            return result;
        }
        if(bin->Operation->Type.Name == tokenType[Tassign].Name){
            if (bin->LNode->ID == Nvar){
                Variable* var = (Variable*)(bin->LNode->Node);
                char* name = var->Var->Code;
                Data* result = Run(parser, bin->RNode);
                DictionaryAdd(parser->DictionaryVariable, result, name);
                return result;
            }
            else{
                Indexator* indexator = (Indexator*)(bin->LNode->Node);
                Variable* var = (Variable*)(indexator->Var->Node);
                if (TryGetValue(parser->DictionaryVariable, var->Var->Code) == 1){
                    Data* indList = GetValue(parser->DictionaryVariable, var->Var->Code);
                    Data* ind = Run(parser, indexator->Index);

                    if (indList->Type != Varr){
                        char m[20 + sizeof(var->Var->Code)];
                        sprintf(m, "List %s is not created", var->Var->Code);
                        ThrowParser(parser, var->Var->Code != NULL, var->Var, m);
                        return NULL;
                    }
                    if (parser->Lists[ToArr(indList)].Len > ToInt(ind) && ToInt(ind) > -1){
                        Data* result = Run(parser, bin->RNode);
                        parser->Lists[ToArr(indList)].Values[ToInt(ind)] = *result;
                        return result;
                    }
                    ThrowParser(parser, var->Var->Code != NULL, var->Var, "Index beyond the boundaries of array");
                }
                char m[20 + sizeof(var->Var->Code)];
                sprintf(m, "List %s is not created", var->Var->Code);
                ThrowParser(parser, var->Var->Code != NULL, var->Var, m);
            }
        }
    }
    if (node->ID == Nroot){
        StatementsNode code = *(StatementsNode*)(node->Node);
        while(parser->NumberString < code.CountStrings){
            Run(parser, &code.Strings[parser->NumberString]);
            parser->NumberString++;
        }
        return NULL;
    }
    return NULL;
}

void DelParser(Parser* parser){
    for (unsigned int i = 0; i < parser->LenTokens; i++){
        free(parser->Tokens[i].Code);
        parser->Tokens[i].Code = NULL;
    }
    free(parser->Tokens);
    parser->Tokens = NULL;
    free(parser);
    parser = NULL;
}