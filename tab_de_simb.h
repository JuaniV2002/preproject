// tab_de_simb.h
#ifndef TAB_DE_SIMB_H
#define TAB_DE_SIMB_H

#include "ast.h"

typedef enum {FUN, VAR, CONST} flagType;

typedef struct Symbol {
    flagType flag;
    char* name;
    infoType type;
    Value* value;
} Symbol;

typedef struct TSNode {
    Symbol* symbol;

    struct TSNode* next;
} TSNode;

TSNode* initializeTS();

Symbol* insertSymbol(TSNode* symbolTable, flagType flag, char* name, infoType type, Value* value);

Symbol* getSymbol(TSNode* symbolTable, char* name);

void printTS(TSNode* symbolTable);

#endif // TAB_DE_SIMB_H