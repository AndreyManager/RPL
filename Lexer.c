#include <stdio.h>
#include <stdlib.h>
#include "Match.c"
//#include "Token.c"
//#include "String.c"
//extern TokenType tokenType[];


typedef struct{
    unsigned int Pos;
    unsigned int LenTokens;
    unsigned int CountStrings;
    char* Code;
    Token* Tokens;
} Lexer;


void LexerInit(Lexer* lexer, char* code){
    lexer->Pos = 0;
    lexer->CountStrings = 1;
    lexer->LenTokens = 0;
    lexer->Code = code;
    lexer->Tokens = NULL;
}

char* subCode;
char* SubStr(unsigned int pos, char* str){
    unsigned int len = 0;
    while(str[len] != '\0'){len++;}

    subCode = (char*)calloc(len-pos, sizeof(char));
    if (subCode == NULL){return NULL;}

    for(int i = 0; i < len-pos; i++){
        subCode[i] = str[pos+i];
    }
    return subCode;
}

short NextToken(Lexer* lexer){
    unsigned int lenCode = 0;
    while(lexer->Code[lenCode] != '\0'){lenCode++;}
    if (lexer->Pos >= lenCode){
        return 0;
    }
    unsigned int lenType = sizeof(tokenType)/sizeof(tokenType[0]); 
    for (int i = 0; i < lenType; i++){
        TokenType type = {tokenType[i].Name, tokenType[i].Regex};
        char* code = SubStr(lexer->Pos, lexer->Code);
        if(code == NULL){
            return -1;
        }
        char* result = Match(code, type.Regex);
        free(code);

        if (result != NULL){
            unsigned int lenResult = 0;
            while(result[lenResult] != '\0'){lenResult++;}
            //printf("%d _ %s\n", lenResult, result);
            if(lenResult == 0){continue;}
            lexer->Pos += lenResult;

            if (i != lenType-1){
                Token* token = malloc(sizeof(Token));
                if (token == NULL){return -1;}
                TokenInit(token, lexer->Pos, lexer->LenTokens, result, &type);
                lexer->Tokens = realloc(lexer->Tokens, sizeof(Token) * (lexer->LenTokens+1));
                if (lexer->Tokens != NULL){
                    if (IsEqStr(token->Type.Name, tokenType[18].Name)){lexer->CountStrings++;}
                    lexer->Tokens[lexer->LenTokens++] = *token;
                    return 1;
                }
                printf("Lexer; String: %d; Pos: %d; El: %c\n", lexer->CountStrings, lexer->Pos, lexer->Code[lexer->Pos]);
                printf("Error realloc\n");
                return -1;
            }
            return 1;
        }
    }
    printf("Lexer; String: %d; Pos: %d; El: %c\n", lexer->CountStrings, lexer->Pos, lexer->Code[lexer->Pos]);
    printf("Expression is not in list tokens\n");
    return -1;
}