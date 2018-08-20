
default:
	lex scanner.l
	gcc -o etapa1 main.c lex.yy.c hash.c

hash_test:
	lex scanner.l
	gcc -o hash_test main_hash_test.c lex.yy.c hash.c

clean:
	rm -rf lex.yy.c etapa1 hash_test
