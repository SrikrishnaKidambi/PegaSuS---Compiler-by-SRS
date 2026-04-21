pegasus-cc: y.tab.c lex.yy.c symtab.c optimizer.c asm_gen.c transpiler.c snapshot_runtime.c
	gcc -g lex.yy.c y.tab.c symtab.c optimizer.c asm_gen.c transpiler.c snapshot_runtime.c -o pegasus-cc -ll -lm

y.tab.c: parser.y
	yacc -d parser.y

lex.yy.c: lexer.l y.tab.h
	flex lexer.l

clean:
	rm -f pegasus-cc y.tab.c y.tab.h lex.yy.c output.s output.py snapshot_runtime.o

install: pegasus-cc
	@which riscv64-linux-gnu-gcc > /dev/null 2>&1 || \
		(echo "ERROR: riscv64-linux-gnu-gcc not found. Run: sudo apt install gcc-riscv64-linux-gnu" && exit 1)
	@which qemu-riscv64 > /dev/null 2>&1 || \
		(echo "ERROR: qemu-riscv64 not found. Run: sudo apt install qemu-user" && exit 1)
	sudo cp pegasus /usr/local/bin/pegasus
	sudo cp pegasus-cc /usr/local/bin/pegasus-cc
	sudo cp snapshot_runtime.c /usr/local/bin/
	sudo cp snapshot_runtime.h /usr/local/bin/ 2>/dev/null || true
	sudo chmod +x /usr/local/bin/pegasus
	sudo chmod +x /usr/local/bin/pegasus-cc
	@echo "pegasus installed successfully!"