CC     := gcc
CFLAGS := -std=c11 -Wall -Wextra -D_POSIX_C_SOURCE=200809L -D_DEFAULT_SOURCE
FLEX   := flex
BISON  := bison -d

all: quadrasimples quadravm

quadrasimples: parser.tab.o lex.yy.o main.o codegen.o
	$(CC) $(CFLAGS) -o $@ parser.tab.o lex.yy.o main.o codegen.o -lfl

quadravm: quadravm.o
	$(CC) $(CFLAGS) -o $@ quadravm.o

quadravm.o: quadravm.c
	$(CC) $(CFLAGS) -c quadravm.c

codegen.o: codegen.c codegen.h
	$(CC) $(CFLAGS) -c codegen.c

parser.tab.c parser.tab.h: parser.y
	$(BISON) parser.y

lex.yy.c: lexer.l parser.tab.h
	$(FLEX) lexer.l

parser.tab.o: parser.tab.c
	$(CC) $(CFLAGS) -c parser.tab.c

lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c lex.yy.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f quadrasimples quadravm parser.tab.* lex.yy.c *.o *.output

.PHONY: all clean
