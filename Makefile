# Compilador y flags
CC = gcc

# Archivos fuente
BISON_SRC = analiz_sintactico.y
FLEX_SRC  = analiz_lexico.l
MAIN_SRC  = main.c
AST_SRC   = ast.c
INTER_SRC = interprete.c
ASM_SRC   = gener_assembly.c
SYM_SRC   = tab_de_simb.c

# Archivos generados por bison y flex
BISON_C = analiz_sintactico.tab.c
BISON_H = analiz_sintactico.tab.h
FLEX_C  = lex.yy.c

# Ejecutable final
TARGET = parser

# Archivo de entrada por defecto
FILE ?= ejemplo.txt

# Regla por defecto
all: $(TARGET)

# Generar ejecutable
$(TARGET): $(MAIN_SRC) $(AST_SRC) $(INTER_SRC) $(ASM_SRC) $(BISON_C) $(FLEX_C) $(SYM_SRC)
	$(CC) -o $(TARGET) $(MAIN_SRC) $(AST_SRC) $(INTER_SRC) $(ASM_SRC) $(BISON_C) $(FLEX_C) $(SYM_SRC)

# Generar parser con Bison
$(BISON_C) $(BISON_H): $(BISON_SRC)
	bison -d -Wcounterexamples $(BISON_SRC)

# Generar scanner con Flex
$(FLEX_C): $(FLEX_SRC)
	flex $(FLEX_SRC)

# Ejecutar el programa con archivo de entrada
run: $(TARGET)
	./$(TARGET) $(FILE)

# Limpiar archivos generados
clean:
	rm -f $(TARGET) $(BISON_C) $(BISON_H) $(FLEX_C)
