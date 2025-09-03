CC = gcc
CFLAGS = -Wall -g

YACC_FILE = parser.y
LEX_FILE = lexer_token.l
TARGET = codegen

YACC_OUT = parser.tab.c parser.tab.h
LEX_OUT = lex.yy.c
OBJECTS = parser.tab.o lex.yy.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

parser.tab.o: parser.tab.c parser.tab.h
	$(CC) $(CFLAGS) -c $<

lex.yy.o: lex.yy.c parser.tab.h
	$(CC) $(CFLAGS) -c $<

parser.tab.c parser.tab.h: $(YACC_FILE)
	bison -d $<

lex.yy.c: $(LEX_FILE) parser.tab.h
	flex -o lex.yy.c $<

test: $(TARGET)
	@cat test.txt | ./$(TARGET)

clean:
	rm -f $(YACC_OUT) $(LEX_OUT) $(OBJECTS) $(TARGET)

.PHONY: all test clean
