typedef struct Str{
    char* word;
}String;

union ASCI{
    short num;
    char symbol;   
};


unsigned short IsEqStr(char* str1, char* str2){
    unsigned int len1 = 0; while(str1[len1] != '\0'){len1++;}
    unsigned int len2 = 0; while(str2[len2] != '\0'){len2++;}
    if(len1 != len2){return 0;}
    for(unsigned int i = 0; i < len1; i++){if(str1[i] != str2[i]){return 0;}}
    return 1;
}

union ASCI asci;