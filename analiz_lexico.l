%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "analiz_sintactico.tab.h"
%}

%option noyywrap
%option yylineno
/* Macros para definir tokens */
DIGIT      [0-9]
LETTER   [a-zA-Z]

/* Reglas de la gramatica */
%%
[ \t\r\n]+  { /* ignorar espacios en blanco */ }
{DIGIT}+   { yylval.int_num = atoi(yytext); return INT_NUM; }

"return"    { return T_RET; }
"int"       { return T_INT; }
"bool"      { return T_BOOL; }
"void"      { return T_VOID; }
"main"      { return T_MAIN; }
"true"      { yylval.boolean = 1; return T_TRUE; }
"false"     { yylval.boolean = 0; return T_FALSE; }

[_]*{LETTER}+({LETTER}|{DIGIT})*    { yylval.str = strdup(yytext); return T_ID; }

"+"     { return '+'; }
"*"     { return '*'; }

"="     { return '='; }

"("     { return '('; }
")"     { return ')'; }
"{"     { return '{'; }
"}"     { return '}'; }

";"     { return ';'; }
%%