#include <stdlib.h>
#include "Data.c"


int main(){
    Data* data = malloc(sizeof(Data));
    CharTo(data, '0');
    printf("V: %c, T: %d\n", ToChar(data), data->Type);
    Data* nd = TypeToInt(data);
    printf("V: %d, T: %d\n", ToInt(nd), nd->Type);
}