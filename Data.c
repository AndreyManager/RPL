#include "String.c"
#include <stdio.h>


typedef struct Data{
    int* Value;
    unsigned int Len;
    unsigned int Type;
}Data;


void DataInit(Data* data, unsigned int len, unsigned int type){
    data->Value = calloc(len, sizeof(int));
    data->Len = len;
    data->Type = type;
}


int ToInt(Data* data){
    return data->Value[0];
}

float ToFloat(Data* data){
    float numf = (float)(data->Value[0]);
    for(int i = 0; i < data->Value[1]; i++){
        numf /= 10;
    }
    return numf;
}

int ToBool(Data* data){
    return data->Value[0];
}
char ToChar(Data* data){
    asci.num = data->Value[0];
    return asci.symbol;
}

void IntTo(Data* data, int num){
    data->Value[0] = num;
}

void FloatTo(Data* data, float num){
    int need = (int)num;
    int ind = 0;
    while (num != need){
        num *= 10;
        need = (int)num;
        ind++;
    }
    data->Value[0] = need;
    data->Value[1] = ind;
}

void BoolTo(Data* data, int num){
    data->Value[0] = num != 0 ? 1 : 0;
}
void CharTo(Data* data, char symbol){
    asci.symbol = symbol;
    data->Value[0] = asci.num;
}

void TypeTo(Data* data, char* code){
    if(code[0] == '\'' && code[2] == '\''){
        DataInit(data, 1, 3);
        CharTo(data, code[1]);
        return;
    }
    if(IsEqStr(code, "true") == 1){
        DataInit(data, 1, 2);
        BoolTo(data, 1);
        return;
    }
    if(IsEqStr(code, "false") == 1){
        DataInit(data, 1, 2);
        BoolTo(data, 0);
        return;
    }
    unsigned int ind = 0;
    unsigned int len = 0;
    for(unsigned int i = 0; code[i] != '\0'; i++){
        if(code[i] == '.')
        {
            ind = i;
        }
        len = i;
    }
    if(ind == 0){
        DataInit(data, 1, 0);
        IntTo(data, atoi(code));
        return;
    }
    else{
        DataInit(data, 2, 1);
        char num[len];
        for(unsigned int i = 0; i < len; i++){
            num[i] = code[i + (i >= ind ? 1 : 0)]; // Jump point
        }
        data->Value[0] = atoi(num);
        data->Value[1] = len-ind;
    }
}
void DelData(Data* data){
    free(data->Value);
    free(data);
    data = NULL;
}