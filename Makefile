pegasus: y.tab.c lex.yy.c symtab.c optimizer.c asm_gen.c
	gcc -g lex.yy.c y.tab.c symtab.c optimizer.c asm_gen.c -o pegasus -ll -lm

y.tab.c: parser.y
	yacc -d parser.y

lex.yy.c: lexer.l y.tab.h
	flex lexer.l

clean:
	rm -f pegasus y.tab.c y.tab.h lex.yy.c
