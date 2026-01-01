#include <stdlib.h>

typedef struct List{
    int Len;
    Data* Values;
} List;


void ListInit(List* list, int len){
    list->Len = len;
    list->Values = calloc(len, sizeof(Data));
}
void Add(List* list){
    list = realloc(list, ++list->Len);
}
void Pop(List* list){
    list = realloc(list, --list->Len);
}