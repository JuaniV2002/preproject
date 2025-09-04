#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "tab_de_simb.h"

TSNode* initializeTS() {
    TSNode* tableNode = (TSNode*) malloc(sizeof(TSNode));
    if (!tableNode) {
        fprintf(stderr, "Error al asignar memoria para la tabla de simbolos.\n");
        exit(EXIT_FAILURE);
    }
    tableNode->symbol = NULL;
    tableNode->next = NULL;
    return tableNode;
}

Symbol* insertSymbol(TSNode* symbolTable, flagType flag, char* name, infoType type, Value* value) {
    if (getSymbol(symbolTable, name)) {
        fprintf(stderr, "Error: El simbolo '%s' ya existe en la tabla de simbolos.\n", name);
        return NULL;
    }

    Symbol* newSymbol = (Symbol*) malloc(sizeof(Symbol));
    if (!newSymbol) {
        fprintf(stderr, "Error al asignar memoria para un nuevo simbolo.\n");
        exit(EXIT_FAILURE);
    }
    newSymbol->flag = flag;
    newSymbol->name = strdup(name);
    newSymbol->type = type;
    newSymbol->value = value;

    if (symbolTable->symbol == NULL) {
        symbolTable->symbol = newSymbol;
        return newSymbol;
    }
    

    TSNode* newTSNode = (TSNode*) malloc(sizeof(TSNode));
    if (!newTSNode) {
        fprintf(stderr, "Error al asignar memoria para una nueva entrada en la tabla de simbolos.\n");
        free(newSymbol->name);
        free(newSymbol);
        exit(EXIT_FAILURE);
    }
    newTSNode->symbol = newSymbol;

    TSNode* current = symbolTable;
    while (current->next) {
        current = current->next;
    }

    newTSNode->next = current->next;
    current->next = newTSNode;

    return newSymbol;
}

Symbol* getSymbol(TSNode* symbolTable, char* name) {
    TSNode* current = symbolTable;

    while (current) {
        if (current->symbol && strcmp(current->symbol->name, name) == 0) {
            return current->symbol;
        }
        current = current->next;
    }
    return NULL; // No encontrado
}

void printTS(TSNode* symbolTable) {
    TSNode* current = symbolTable;
    printf("Tabla de Simbolos:\n");
    printf("------------------\n");
    while (current) {
        if (current->symbol) {
            printf("Flag: %s\n", current->symbol->flag == FUN ? "FUN" : "VAR");
            printf("Nombre: %s, Tipo: ", current->symbol->name);
            switch (current->symbol->type) {
                case TYPE_VOID: printf("void\n\n"); break;
                case TYPE_INT: printf("int\n\n"); break;
                case TYPE_BOOL: printf("bool\n\n"); break;
                default: printf("none\n\n"); break;
            }
        }
        current = current->next;
    }
    printf("------------------\n");
}