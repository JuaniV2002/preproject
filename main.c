#include <stdio.h>
#include "ast.h"
#include "analiz_sintactico.tab.h"
#include "tab_de_simb.h"
#include "interprete.h"
#include "gener_assembly.h"

extern FILE *yyin;
extern Node* root;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "ERROR: Incluir el archivo a comprobar su sintaxis: %s <archivo.txt>\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("No se pudo abrir el archivo");
        return 1;
    }

    if (yyparse() == 0) {
        printf("Arbol de sintaxis abstracta (AST):\n");
        printAST(root, 0);

        TSNode* symbolTable = initializeTS();

        printf("El interprete se esta ejecutando\n\n");
        executeInterpreter(root, symbolTable);

        printTS(symbolTable);

        printf("\nEl programa es sintacticamente correcto.\n\n");

        printf("Codigo assembly generado:\n\n");
        generate(root);
    } else {
        printf("Se encontraron errores de sintaxis.\n");
    }

    fclose(yyin);
    return 0;
}