#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "tab_de_simb.h"
#include "interprete.h"

int findValue(Node* root, TSNode* symbolTable) {
    if (!root) return 0;

    if (root->t_Node == TERM && root->t_Info == TYPE_INT) {
        return root->info->int_num;
    }
    if (root->t_Node == TERM && root->t_Info == TYPE_BOOL) {
        return root->info->boolean;
    }
    if (root->t_Node == TERM && root->t_Info == TYPE_ID) {
        Symbol* sym = getSymbol(symbolTable, root->info->id);
        if (!sym) {
            fprintf(stderr, "Error: Variable '%s' no declarada.\n", root->info->id);
            exit(EXIT_FAILURE);
        }
        if (sym->type == TYPE_INT) {
            return sym->value->int_num;
        } else if (sym->type == TYPE_BOOL) {
            return sym->value->boolean;
        } else {
            fprintf(stderr, "Error: Tipo de variable '%s' no soportado.\n", root->info->id);
            exit(EXIT_FAILURE);
        }
    }
    

    switch (root->t_Node) {
        case PROG:
            return findValue(root->right, symbolTable);
        case DECL:
            break;
        case TERM:
            break;
        case ASSIGN: {
            Symbol* sym = getSymbol(symbolTable, root->left->info->id);
            if (!sym) {
                fprintf(stderr, "Error: Variable '%s' no declarada.\n", root->left->info->id);
                exit(EXIT_FAILURE);
            }

            // Buscar el valor a asignar
            if (root->right) {
                if (sym->type == TYPE_INT) {
                    sym->value->int_num = findValue(root->right, symbolTable);
                    return sym->value->int_num;
                } else if (sym->type == TYPE_BOOL) {
                    sym->value->boolean = findValue(root->right, symbolTable);
                    return sym->value->boolean;
                } else {
                    fprintf(stderr, "Error: Tipo incompatible en la asignacion a '%s'.\n", root->left->info->id);
                    exit(EXIT_FAILURE);
                }
            } else {
                fprintf(stderr, "Error: Asignacion sin valor a '%s'.\n", root->left->info->id);
                exit(EXIT_FAILURE);
            }
            break;
        }
        case RET:
            if (root->left) {
                return findValue(root->left, symbolTable);
            } else {
                return 0; // void return
            }
            break;
        case EXP:
            if (root->left && root->right) {
                int leftVal = findValue(root->left, symbolTable);
                int rightVal = findValue(root->right, symbolTable);
                switch (root->info->bin_op) {
                    case '+': return leftVal + rightVal;
                    case '*': return leftVal * rightVal;
                    default:
                        fprintf(stderr, "Error: Operador binario no soportado '%c'.\n", root->info->bin_op);
                        exit(EXIT_FAILURE);
                }
            } else {
                fprintf(stderr, "Error: Expresion incompleta.\n");
                exit(EXIT_FAILURE);
            }
            break;
        case SENT:
            findValue(root->left, symbolTable);
            findValue(root->right, symbolTable);
            break;
        default:
            fprintf(stderr, "Error: Nodo no soportado en la interpretacion.\n");
            exit(EXIT_FAILURE);
    }
}

void interpretDecl(Node* root, TSNode* symbolTable) {
    if (!root) return;

    if (root->t_Node == DECL) {
        // Si es un nodo con hijos terminales (contiene los datos de la declaracion)
        if (root->left && root->left->t_Node == TERM && root->right && root->right->t_Node == TERM) {
            infoType varType = root->left->t_Info;
            char* varName = root->right->info->id;

            // Verificar si la variable ya existe
            if (getSymbol(symbolTable, varName)) {
                printf("Error: Variable '%s' ya declarada.\n", varName);
                exit(EXIT_FAILURE);
            }

            // Crear un nuevo valor para la variable
            Value* val = (Value*) malloc(sizeof(Value));
            if (!val) {
                fprintf(stderr, "Error al asignar memoria para el valor de la variable '%s'.\n", varName);
                exit(EXIT_FAILURE);
            }
            if (varType == TYPE_INT) {
                val->int_num = 0; // Inicializar a 0
            } else if (varType == TYPE_BOOL) {
                val->boolean = 0; // Inicializar a false
            } else {
                fprintf(stderr, "Error: Tipo no soportado para la variable '%s'.\n", varName);
                free(val);
                exit(EXIT_FAILURE);
            }

            // Insertar el simbolo en la tabla
            insertSymbol(symbolTable, VAR, varName, varType, val);
        }
    }

    if (root->left->t_Node == DECL) interpretDecl(root->left, symbolTable);
    if (root->right->t_Node == DECL) interpretDecl(root->right, symbolTable);
}

void executeInterpreter(Node* root, TSNode* symbolTable) {
    if (!root) return;

    interpretDecl(root->left, symbolTable);
    findValue(root, symbolTable);
}