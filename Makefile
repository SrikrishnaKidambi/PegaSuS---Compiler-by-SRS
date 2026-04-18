pegasus-cc: y.tab.c lex.yy.c symtab.c optimizer.c asm_gen.c transpiler.c
	gcc -g lex.yy.c y.tab.c symtab.c optimizer.c asm_gen.c transpiler.c -o pegasus-cc -ll -lm

y.tab.c: parser.y
	yacc -d parser.y

lex.yy.c: lexer.l y.tab.h
	flex lexer.l

clean:
	rm -f pegasus-cc y.tab.c y.tab.h lex.yy.c output.s output.py
