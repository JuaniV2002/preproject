# Gramática
P -> TipoRetorno main () {DeclVars Sentencias Retorno}

P -> TipoRetorno main () {DeclVars Sentencias}

TipoRetorno -> int | bool | void

DeclVars  -> (Declaracion)*
Declaracion -> Tipo id ;

Tipo -> int | bool

Sentencias -> (Asignacion)*

Asignacion -> id = E ;

Retorno -> return E ;
    | ƛ

E -> E + T
    | T

T -> T * F
    | F

F -> (E)
    | nro
    | true
    | false
    | id

id -> letra (letra | digito)*
nro -> digito+
letra -> a | b | ... | z | A | B | ... | Z
digito -> 0 | 1 | ... | 9

# Expresiones regulares

## Palabras reservadas

- \bint\b
- \bbool\b
- \bvoid\b
- \bmain\b
- \breturn\b
- \btrue\b
- \bfalse\b

## Identificadores
- id = [A-Za-z][A-Za-z0-9]*
- nro = [0-9]+

## Operadores
- ASIG = =
- SUMA = \+
- MULT = \*

## Delimitadores
- LPAREN \(
- RPAREN \)
- LBRACE \{
- RBRACE \}
- PCOMA ;