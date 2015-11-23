#ifndef _SYMBOL_H
#define _SYMBOL_H



enum{
    RESULT;
    ATTRIBUTE;
    UNIFORM;
    NONE;


} SYMBOL_ATTRIBUTE;

struct symbol{
    int type_code;
    char* id;
    symbol* prev;
    symbol_attribute;
} symbol;


int get_multiplicity(int type_code){
    return type_code & ;
}



#endif

