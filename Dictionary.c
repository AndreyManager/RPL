#include <stdlib.h>
#include "Data.c" 


typedef struct Dic{
    unsigned int Len;
    unsigned int Type;
    Data* NumArr;
    String* StrArr;
} Dictionary;


void DicInit(Dictionary* dic, unsigned int max, unsigned int type, char* str, Data* data){
    dic->Len = 1;
    dic->Type = type;
    dic->NumArr = calloc(max+1, sizeof(Data));
    dic->StrArr = calloc(max+1, sizeof(String));
    dic->NumArr[0] = *data;
    dic->StrArr[0].word = str;
}

unsigned short TryGetValue(Dictionary* dic, char* key){
    for (unsigned int i = 0; i < dic->Len; i++){
        if(IsEqStr(dic->StrArr[i].word, key) == 1){
            return 1;
        }
    }
    return 0;
}

String* strT;
Data* intT;
void DictionaryAdd(Dictionary* dic, Data* value, char* key){
    if(TryGetValue(dic, key) == 1){
        for(int i = 0; i < dic->Len; i++){
            if(IsEqStr(dic->StrArr[i].word, key)){
                dic->NumArr[i] = *value;
                return;
            }
        }
    }
    // ERROR
    dic->NumArr[dic->Len] = *value;
    dic->StrArr[dic->Len].word = key;
    dic->Len++;
}

Data* GetValue(Dictionary* dic, char* key){
    for (int i = 0; i < dic->Len; i++){
        if(IsEqStr(dic->StrArr[i].word, key)){
            return &(dic->NumArr[i]);
        }
    }
    return 0;
}

void DelDic(Dictionary* dic){
    free(dic->NumArr);
    free(dic->StrArr);
    free(dic);
    dic = NULL;
}