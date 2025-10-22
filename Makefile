CC     := gcc
CFLAGS := -std=c11 -Wall -Wextra -D_POSIX_C_SOURCE=200809L -D_DEFAULT_SOURCE
FLEX   := flex
BISON  := bison -d -Wall -Wcounterexamples -v

all: quadrasimples

quadrasimples: parser.tab.o lex.yy.o main.o
	$(CC) $(CFLAGS) -o $@ parser.tab.o lex.yy.o main.o -lfl

parser.tab.c parser.tab.h: parser.y
	$(BISON) parser.y

lex.yy.c: lexer.l parser.tab.h
	$(FLEX) lexer.l

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f quadrasimples parser.tab.* lex.yy.c *.o *.output

.PHONY: all clean
