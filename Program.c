#include <stdio.h>
#include <stdlib.h>
#include "Parser.c"
#include "Lexer.c"
//extern TokenType tokenType[];


int main(){
    char* myCode;
    FILE* file = fopen("code.txt", "r");
    int len = 0;
    if(file != NULL){
        fseek(file, 0, SEEK_END);
        len = ftell(file);
        fseek(file, 0, SEEK_SET);
        myCode = malloc(len+1);
        if(myCode != NULL){
            fread(myCode, 1, len, file);
            myCode[len] = '\0';
            len++;
        }
        else{
            printf("Not Code\n");
            return 1;
        }
        fclose(file);
    }
    else{
        printf("Not File\n");
        return 1;
    }
    for(int i = len-2; myCode[i] != ';'; i--){
        myCode[i] = ' ';
    }
    //printf("%s\n", myCode);

    Lexer lexer;
    LexerInit(&lexer, myCode);
    int nt = 1;
    while(nt == 1){
        nt = NextToken(&lexer);
        if (nt == 0){break;}
        if (nt == -1){return 1;}
    }
    
    printf("%d\n", lexer.LenTokens);
    for(int i = 0; i < lexer.LenTokens; i++){
        printf("%s\n", lexer.Tokens[i].Type.Name);
    }
    printf("\n\n");

    Parser* parser = malloc(sizeof(Parser));
    if(parser == NULL){printf("Parser malloc error\n");return 1;}
    ParserInit(parser, lexer.LenTokens, lexer.Tokens);
    ExpressionNode* rootNode = ParseCode(parser);
    Run(parser, rootNode);

    DelRoot((StatementsNode*)(rootNode->Node));
    DelDic(parser->DictionaryVariable);
    DelParser(parser);
    getchar();
    return 0;
}