#include <stdlib.h>


int IsIn(char* str, char el){
    for (unsigned int i = 0; str[i] != '\0'; i++){
        if (str[i] == el){
            return 1;
        }
    }
    return 0;
}
char* result;
char* Match(char* str, char* regis){
    if(regis[1] == '\''){
        if (str[0] == '\''){
            result = calloc(4, sizeof(char));
            result[0] = str[0];
            result[1] = str[1];
            result[2] = str[2];
            result[3] = '\0';
            return result;
        }
        return NULL;
    }
    else if (regis[0] != '/'){
        unsigned int ind = 0;
        while (IsIn(regis, str[ind]) == 1){ind++;}
        
        if(ind != 0){
            result = (char*)calloc(ind+1, sizeof(char));
            for(unsigned int i = 0; i < ind; i++){
                result[i] = str[i];
            }
            return result;
        }
        else{
            return NULL;
        }
    }
    else{
        unsigned int lenRegis = 0;
        while(regis[lenRegis] != '\0'){lenRegis++;}

        unsigned int ind = 0;
        while(str[ind] == regis[ind+1] && regis[ind+1] != '\0'){ind++;}

        if(ind == lenRegis-1){
            result = (char*)calloc(ind+1, sizeof(char));
            for(unsigned int i = 0; i < ind+1; i++){
                result[i] = regis[i+1];
            }
            return result;
        }
        else{
            return NULL;
        }
    }
}