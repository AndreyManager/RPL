#include <stdlib.h>
#include "Data.c"


int main(){
    Data* data = malloc(sizeof(Data));
    TypeTo(data, "12.3");
    int* f = data->Value;
    printf("%d: %d", f[0], f[1]);
}