// ast.h
#ifndef AST_H
#define AST_H

typedef enum {PROG, DECL, SENT, EXP, ASSIGN, RET, TERM} nodeType;

typedef enum {NONE_INFO, TYPE_VOID, TYPE_ID, TYPE_INT, TYPE_BOOL, TYPE_BIN_OP} infoType;

typedef union {
    char* id;
    int int_num;
    int boolean;
    char bin_op;
} Value;

typedef struct Node {
    nodeType t_Node;
    infoType t_Info;

    Value* info;

    struct Node* left;
    struct Node* right;
} Node;

Node* newNode_Terminal(infoType type, Value value);

Node* newNode_NonTerminal(nodeType type, infoType infType, Value value, struct Node* left, struct Node* right);

void printAST(struct Node* root, int level);

void freeAST(struct Node* root);

#endif // AST_H