#include <stdio.h>
#include <stdlib.h>
#include "interprete.h"

infoType retType;

infoType findType(Node* root, TSNode* symbolTable) {
    if (!root) return NONE_INFO;

    if (root->t_Node == TERM && root->t_Info == TYPE_INT) {
        return TYPE_INT;
    }
    if (root->t_Node == TERM && root->t_Info == TYPE_BOOL) {
        return TYPE_BOOL;
    }
    if (root->t_Node == TERM && root->t_Info == TYPE_ID) {
        Symbol* sym = getSymbol(symbolTable, root->info->id);
        if (!sym) {
            fprintf(stderr, "Error de retorno: Variable '%s' no declarada.\n", root->info->id);
            exit(EXIT_FAILURE);
        }
        return sym->type;
    }

    switch (root->t_Node) {
        case EXP:
            if (root->left && root->right) {
                infoType leftType = findType(root->left, symbolTable);
                infoType rightType = findType(root->right, symbolTable);
                if (leftType != rightType) {
                    fprintf(stderr, "Error de retorno: Tipos incompatibles en la expresion.\n");
                    exit(EXIT_FAILURE);
                }
                return leftType;
            } else {
                fprintf(stderr, "Error de retorno: Expresion incompleta.\n");
                exit(EXIT_FAILURE);
            }
            break;
        default:
            fprintf(stderr, "Error de retorno: Nodo no soportado en la interpretacion.\n");
            exit(EXIT_FAILURE);
    }
}

int checkReturnExistence(Node* root) {
    if (!root) return 0;

    switch (root->t_Node) {
        case PROG:
            if (root->right) checkReturnExistence(root->right);
            break;
        case RET:
            // Se encontro un return
            return 1;
        default:
            if (root->left) checkReturnExistence(root->left);
            if (root->right) checkReturnExistence(root->right);
            break;
    }
}

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
            retType = root->t_Info;
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
                if (retType == TYPE_VOID) {
                    fprintf(stderr, "Error: La funcion es de tipo void y no debe retornar un valor.\n");
                    exit(EXIT_FAILURE);
                }

                int retVal = findValue(root->left, symbolTable);

                if (retType != findType(root->left, symbolTable)) {
                    fprintf(stderr, "Error: Tipo de retorno no coincide con el tipo de la funcion.\n");
                    exit(EXIT_FAILURE);
                }
                return retVal;
            } else {
                if (retType != TYPE_VOID) {
                    fprintf(stderr, "Error: La funcion debe retornar un valor.\n");
                    exit(EXIT_FAILURE);
                }
                
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
            // Verificar dos nodos mas adelante si hay una sentencia despues de un return
            if (root->left && root->left->left && (root->left->left->t_Node == RET || root->left->right->t_Node == RET) && root->right) {
                fprintf(stderr, "Error: Sentencia despues de return no permitida.\n");
                exit(EXIT_FAILURE);
            }

            // Verificar un nodo mas adelante si hay una sentencia despues de un return
            if (root->left->left && root->left->left->t_Node == RET && root->right) {
                fprintf(stderr, "Error: Sentencia despues de return no permitida.\n");
                exit(EXIT_FAILURE);
            }

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
    int val = findValue(root, symbolTable);

    if (!checkReturnExistence(root)) {
        if (retType != TYPE_VOID) {
            fprintf(stderr, "Error: La funcion debe retornar un valor.\n");
            exit(EXIT_FAILURE);
        }
    }

    if (retType == TYPE_VOID) {
        printf("Valor de retorno de la funcion: void\n\n");
    } else if (retType == TYPE_INT) {
        printf("Valor de retorno de la funcion: %d\n\n", val);
    } else if (retType == TYPE_BOOL) {
        printf("Valor de retorno de la funcion: %s\n\n", val ? "true" : "false");
    }
}