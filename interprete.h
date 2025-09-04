#ifndef INTERPRETE_H
#define INTERPRETE_H

#include "ast.h"
#include "tab_de_simb.h"

void executeInterpreter(Node* root, TSNode* symbolTable);

#endif