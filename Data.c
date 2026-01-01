#include <stdio.h>
#include "String.c"

#define Vint 0
#define Vfloat 1
#define Vbool 2
#define Vchar 3
#define Varr 4

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
    if (data->Len > 1){
        for(int i = 0; i < data->Value[1]; i++){
            numf /= 10;
        }
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
int ToArr(Data* data){
    return data->Value[0];

}


void IntTo(Data* data, int num){
    data->Value[0] = num;
    data->Len = 1;
    data->Type = Vint;
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
    data->Len = 2;
    data->Type = Vfloat;
}

void BoolTo(Data* data, int num){
    data->Value[0] = num != 0 ? 1 : 0;
    data->Len = 1;
    data->Type = Vbool;
}
void CharTo(Data* data, char symbol){
    asci.symbol = symbol;
    data->Value[0] = asci.num;
    data->Len = 1;
    data->Type = Vchar;
}
void ArrTo(Data* data, int len){
    data->Value[0] = len;
    data->Len = 1;
    data->Type = Varr;
}
void TypeTo(Data* data, char* code){
    if(code[0] == '\'' && code[2] == '\''){
        DataInit(data, 1, Vchar);
        CharTo(data, code[1]);
        return;
    }
    if(IsEqStr(code, "true") == 1){
        DataInit(data, 1, Vbool);
        BoolTo(data, 1);
        return;
    }
    if(IsEqStr(code, "false") == 1){
        DataInit(data, 1, Vbool);
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
        DataInit(data, 1, Vint);
        IntTo(data, atoi(code));
        return;
    }
    else{
        DataInit(data, 2, Vfloat);
        char num[len];
        for(unsigned int i = 0; i < len; i++){
            num[i] = code[i + (i >= ind ? 1 : 0)]; // Jump point
        }
        data->Value[0] = atoi(num);
        data->Value[1] = len-ind;
    }
}

Data* TypeToInt(Data* data){
    if (data->Type == Vint){
        Data* newData = malloc(sizeof(Data));
        DataInit(newData, 1, Vint);
        IntTo(newData, ToInt(data));
        return newData;
    }
    else if(data->Type == Vfloat){
        Data* newData = malloc(sizeof(Data));
        DataInit(newData, 1, Vint);
        IntTo(newData, (int)ToFloat(data));
        return newData;
    }
    else if(data->Type == Vbool){
        Data* newData = malloc(sizeof(Data));
        DataInit(newData, 1, Vint);
        IntTo(newData, ToBool(data));
        return newData;
    }
    else if(data->Type == Vchar){
        if (data->Value[0] > 47 && data->Value[0] < 58){
            Data* newData = malloc(sizeof(Data));
            DataInit(newData, 1, Vint);
            IntTo(data, data->Value[0] - 48);
            return data;
        }
        return NULL;
    }
    return NULL;
}
Data* TypeToFloat(Data* data){
    if (data->Type == Vint){
        Data* newData = malloc(sizeof(Data));
        DataInit(newData, 1, Vfloat);
        FloatTo(newData, (float)ToInt(data));
        return newData;
    }
    else if(data->Type == Vfloat){
        Data* newData = malloc(sizeof(Data));
        DataInit(newData, 1, Vfloat);
        FloatTo(newData, ToFloat(data));
        return newData;
    }
    else if(data->Type == Vbool){
        Data* newData = malloc(sizeof(Data));
        DataInit(newData, 1, Vfloat);
        FloatTo(newData, (float)ToBool(data));
        return newData;
    }
    return NULL;
}
Data* TypeToBool(Data* data){
    if (data->Type == Vint){
        Data* newData = malloc(sizeof(Data));
        DataInit(newData, 1, Vbool);
        BoolTo(newData, ToInt(data) != 0);
        return newData;
    }
    else if(data->Type == Vfloat){
        Data* newData = malloc(sizeof(Data));
        DataInit(newData, 1, Vbool);
        BoolTo(newData, (int)ToFloat(data) != 0);
        return newData;
    }
    else if(data->Type == Vbool){
        Data* newData = malloc(sizeof(Data));
        DataInit(newData, 1, Vbool);
        BoolTo(newData, ToBool(data));
        return newData;
    }
    return NULL;
}
Data* TypeToChar(Data* data){
    if (data->Type == Vint){
        Data* newData = malloc(sizeof(Data));
        DataInit(newData, 1, Vchar);
        asci.num = ToInt(data) + 48;
        if (asci.num > 57 || asci.num < 48){
            return NULL;
        }
        CharTo(data, asci.symbol);
        return data;
    }
    return NULL;
}

void DelData(Data* data){
    free(data->Value);
    free(data);
    data = NULL;
}