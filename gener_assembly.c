#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "gener_assembly.h"

void generate_decl(Node* decl) {
    switch (decl->t_Node) {
        case DECL:
            if (decl->left && decl->left->t_Node == TERM && decl->right && decl->right->t_Node == TERM) {
                infoType varType = decl->left->t_Info;
                char* varName = decl->right->info->id;

                if (varType == TYPE_INT) {
                    printf("DECL %s int\n", varName);
                } else if (varType == TYPE_BOOL) {
                    printf("DECL %s bool\n", varName);
                } else {
                    fprintf(stderr, "Error: Tipo no soportado para la variable '%s'.\n", varName);
                    exit(EXIT_FAILURE);
                }
            }
            generate_decl(decl->left);
            generate_decl(decl->right);
            break;
        default:
            // No es una declaracion, no hacer nada
            break;
    }
}

void generate_expr(Node* expr) {
    if (!expr) return;

    switch (expr->t_Node) {
        case EXP:
            if (expr->left) generate_expr(expr->left);
            if (expr->right) generate_expr(expr->right);

            if (expr->info->bin_op == '+') {

                printf("ADD\n");
            } else if (expr->info->bin_op == '*') {
                printf("MUL\n");
            } else {
                fprintf(stderr, "Error: Operador desconocido '%c'.\n", expr->info->bin_op);
                exit(EXIT_FAILURE);
            }
            break;
        case TERM:
            if (expr->t_Info == TYPE_INT) {
                printf("PUSHI %d\n", expr->info->int_num);
            } else if (expr->t_Info == TYPE_BOOL) {
                printf("PUSHB %d\n", expr->info->boolean);
            } else if (expr->t_Info == TYPE_ID) {
                printf("LOAD %s\n", expr->info->id);
            } else {
                fprintf(stderr, "Error: Tipo de termino desconocido.\n");
                exit(EXIT_FAILURE);
            }
            break;
        default:
            fprintf(stderr, "Error: Nodo no soportado en la generacion de expresion.\n");
            exit(EXIT_FAILURE);
    }
}

void generate_sent(Node* sent) {
    switch (sent->t_Node) {
        case SENT:
            generate_sent(sent->left);
            generate_sent(sent->right);
            break;
        case ASSIGN: {
            Node* idNode = sent->left;
            Node* exp = sent->right;
            generate_expr(exp);
            printf("STORE %s\n", idNode->info->id);
            break;
        }
        case RET:
            if (sent->left) {
                generate_expr(sent->left);
            } else {
                printf("PUSHI 0\n");
            }
            printf("RET\n");
            break;
        default:
            fprintf(stderr, "Error: Nodo no soportado en la generacion de assembly.\n");
            exit(EXIT_FAILURE);
    }
}

void generate(Node* root) {
    if (!root) {
        printf("AST vacio\n");
        return;
    }

    char* funName = "main"; // Nombre por defecto
    printf("FUNC %s\n", funName);

    generate_decl(root->left);
    generate_sent(root->right);

    printf("ENDFUNC\n");
}