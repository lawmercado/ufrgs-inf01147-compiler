
all:
	lex scanner.l
	gcc -o etapa1 main.c lex.yy.c hash.c

clean:
	rm -rf lex.yy.c etapa1
