#include <stdio.h>
#define Tlabel 0
#define Tlog 1
#define Tif 2
#define Tgoto 3
#define Tnot 4
#define Tand 5
#define Tor 6
#define Tadd 7
#define Tsub 8
#define Teq 9
#define Tgt 10
#define Tlt 11
#define Tassign 12
#define Tlper 13
#define Trper 14
#define Tlqper 15
#define Trqper 16
#define Tnumber 17
#define Ttrue 18
#define Tfalse 19
#define Tsemicolon 20
#define Tstring 21
#define Tsymbol 22
#define Tinput 23
#define Tarr 24
#define Tappend 25
#define Tpop 26
#define Tlen 27
#define Ttype 28
#define Tint 29
#define Tfloat 30
#define Tbool 31
#define Tchar 32
#define Tvar 33
#define Tspace 34


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
    {.Name = "label", .Regex = "/label"},
    {.Name = "print", .Regex = "/log"},
    {.Name = "if", .Regex = "/if"},
    {.Name = "goto", .Regex = "/goto"},
    {.Name = "not", .Regex = "/not"},
    {.Name = "and", .Regex = "/and"},
    {.Name = "or", .Regex = "/or"},
    {.Name = "add", .Regex = "/+"},
    {.Name = "sub", .Regex = "/-"},
    {.Name = "eq", .Regex = "/=="},
    {.Name = "gt", .Regex = "/>"},
    {.Name = "lt", .Regex = "/<"},
    {.Name = "assign", .Regex = "/="},
    {.Name = "lper", .Regex = "/("},
    {.Name = "rper", .Regex = "/)"},
    {.Name = "lqper", .Regex = "/["},
    {.Name = "rqper", .Regex = "/]"},
    {.Name = "number", .Regex = "0123456789."},
    {.Name = "true", .Regex = "/true"},
    {.Name = "false", .Regex = "/false"},
    {.Name = "semicolon", .Regex = "/;"},
    {.Name = "string", .Regex = "/\""},
    {.Name = "symbol", .Regex = "/'"},
    {.Name = "input", .Regex = "/input"},
    {.Name = "arr", .Regex = "/list"},
    {.Name = "append", .Regex = "/add"},
    {.Name = "pop", .Regex = "/pop"},
    {.Name = "len", .Regex = "/len"},
    {.Name = "type", .Regex = "/type"},
    {.Name = "int", .Regex = "/int"},
    {.Name = "float", .Regex = "/float"},
    {.Name = "bool", .Regex = "/bool"},
    {.Name = "char", .Regex = "/char"},
    {.Name = "variable", .Regex = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890"},
    {.Name = "space", .Regex = " \n\t"}
};