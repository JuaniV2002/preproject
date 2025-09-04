#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "tab_de_simb.h"
#include "interprete.h"

int findValue(Node* root, TSNode* symbolTable) {
    if (!root) return 0;

    Node* current = root;

    switch (current->t_Node) {
        case PROG:
            return findValue(current->left, symbolTable);
            return findValue(current->right, symbolTable);
            break;
        case DECL:
            if (current->left->t_Node != TERM) {
                findValue(current->left, symbolTable);
            }

            if (current->right->t_Node != TERM) {
                findValue(current->right, symbolTable);
            }
            
            break;
        case TERM:
            if (current->t_Info == TYPE_INT) {
                return current->info->int_num;
            } else if (current->t_Info == TYPE_BOOL) {
                return current->info->boolean;
            } else {
                fprintf(stderr, "Error: Tipo de terminal no soportado en la interpretacion.\n");
                exit(EXIT_FAILURE);
            }
            break;
        case ASSIGN: {
            Symbol* sym = getSymbol(symbolTable, current->left->info->id);
            if (!sym) {
                fprintf(stderr, "Error: Variable '%s' no declarada.\n", current->left->info->id);
                exit(EXIT_FAILURE);
            }

            // Buscar el valor a asignar
            if (current->right) {
                if (sym->type == TYPE_INT) {
                    sym->value->int_num = findValue(current->right, symbolTable);
                    return sym->value->int_num;
                } else if (sym->type == TYPE_BOOL) {
                    sym->value->boolean = findValue(current->right, symbolTable);
                    return sym->value->boolean;
                } else {
                    fprintf(stderr, "Error: Tipo incompatible en la asignacion a '%s'.\n", current->left->info->id);
                    exit(EXIT_FAILURE);
                }
            } else {
                fprintf(stderr, "Error: Asignacion sin valor a '%s'.\n", current->left->info->id);
                exit(EXIT_FAILURE);
            }
            break;
        }
        case RET:
            if (current->left) {
                return findValue(current->left, symbolTable);
            } else {
                return 0; // void return
            }
            break;
        case EXP:
            if (current->left && current->right) {
                int leftVal = findValue(current->left, symbolTable);
                int rightVal = findValue(current->right, symbolTable);
                switch (current->info->bin_op) {
                    case '+': return leftVal + rightVal;
                    case '*': return leftVal * rightVal;
                    default:
                        fprintf(stderr, "Error: Operador binario no soportado '%c'.\n", current->info->bin_op);
                        exit(EXIT_FAILURE);
                }
            } else {
                fprintf(stderr, "Error: Expresion incompleta.\n");
                exit(EXIT_FAILURE);
            }
            break;
        case SENT:
            return findValue(current->left, symbolTable);
            return findValue(current->right, symbolTable);
            break;
        default:
            fprintf(stderr, "Error: Nodo no soportado en la interpretacion.\n");
            exit(EXIT_FAILURE);
    }
}

void interpretDecl(Node* root, TSNode* symbolTable) {
    if (!root) return;

    Node* current = root;

    switch (current->t_Node) {
        case DECL:
            // Si es un nodo con hijos terminales (contiene los datos de la declaracion)
            if (current->left && current->left->t_Node == TERM && current->right && current->right->t_Node == TERM) {
                infoType varType = current->left->t_Info;
                char* varName = current->right->info->id;

                // Verificar si la variable ya existe
                if (getSymbol(symbolTable, varName)) {
                    fprintf(stderr, "Error: Variable '%s' ya declarada.\n", varName);
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
            break;
        
        default:
            break;
    }

    if (current->left->t_Node == DECL) interpretDecl(current->left, symbolTable);
    if (current->right->t_Node == DECL) interpretDecl(current->right, symbolTable);
}

void executeInterpreter(Node* root, TSNode* symbolTable) {
    if (!root) return;

    interpretDecl(root, symbolTable);
    findValue(root, symbolTable);
}