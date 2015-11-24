#ifndef _SYMBOL_H
#define _SYMBOL_H



typedef enum{
    RESULT,
    ATTRIBUTE,
    UNIFORM,
    NONE,


} symbol_attribute;

struct symbol{
    int type_code;
    int mult;
    int scope;
    char* id;
    symbol* prev;
    int attr;
};




#endif

