#include <stdio.h>


typedef struct TokenType{
    char* Name;
    char* Regex;
} TokenType;

typedef struct Token{
    unsigned int LexerPos;
    unsigned int ParserPos;
    char* Code;
    TokenType Type;
}Token;



void TokenInit(
    Token* token,
    unsigned int lexerPos,
    unsigned int parserPos,
    char* code,
    TokenType* tokenType
    )
{
    token->LexerPos = lexerPos;
    token->ParserPos = parserPos;
    token->Code = code;
    token->Type = *tokenType;
}

void DelToken(Token* token){
    printf("f\n");
    free(token->Code); token->Code = NULL;
    free(token); token = NULL;   
}


TokenType tokenType[] = {
    {.Name = "label", .Regex = "/label"},                         // 0
    {.Name = "print", .Regex = "/log"},                           // 1
    {.Name = "if", .Regex = "/if"},                               // 2
    {.Name = "goto", .Regex = "/goto"},                           // 3
    {.Name = "not", .Regex = "/not"},                             // 4
    {.Name = "and", .Regex = "/and"},                             // 5
    {.Name = "or", .Regex = "/or"},                               // 6
    {.Name = "add", .Regex = "/+"},                               // 7
    {.Name = "sub", .Regex = "/-"},                               // 8
    {.Name = "eq", .Regex = "/=="},                               // 9
    {.Name = "gt", .Regex = "/>"},                                // 10
    {.Name = "lt", .Regex = "/<"},                                // 11
    {.Name = "assign", .Regex = "/="},                            // 12
    {.Name = "lper", .Regex = "/("},                              // 13
    {.Name = "rper", .Regex = "/)"},                              // 14
    {.Name = "number", .Regex = "0123456789."},                   // 15
    {.Name = "true", .Regex = "/true"},                           // 16
    {.Name = "false", .Regex = "/false"},                         // 17
    {.Name = "semicolon", .Regex = "/;"},                         // 18
    {.Name = "symbol", .Regex = "/'"},                            // 19
    {.Name = "variable", .Regex = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890"},  // 20
    {.Name = "space", .Regex = " \n\t"}                           // 21
};