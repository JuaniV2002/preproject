%{
#include <stdlib.h>
#include "parser.tab.h"
%}
%option noyywrap yylineno nounput noinput

ID   [A-Za-z][A-Za-z0-9]*
NRO  [0-9]+
WS   [ \t\r\n]+

%%
{WS}                      ;
"//".*                    ;
"/*"([^*]|\*+[^*/])*"\*/" ;

"int"                     { return KW_INT; }
"return"                  { return KW_RETURN; }

{NRO}                     { yylval.ival = atoi(yytext); return INT; }
{ID}                      { yylval.sval = strdup(yytext); return ID; }

"="                       { return '='; }
"+"                       { return '+'; }
"-"                       { return '-'; }
"*"                       { return '*'; }
"("                       { return '('; }
")"                       { return ')'; }
";"                       { return ';'; }

.                         { /* ignorar */ }
%%
