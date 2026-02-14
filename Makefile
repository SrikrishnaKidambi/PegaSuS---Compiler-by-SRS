pegasus: y.tab.c lex.yy.c
	gcc lex.yy.c y.tab.c -o pegasus -ll

y.tab.c: parser.y
	yacc -d parser.y

lex.yy.c: lexer.l y.tab.h
	flex lexer.l

clean:
	rm -f pegasus y.tab.c y.tab.h lex.yy.c
