##
# Makefile for the lexical and syntactical analyser
#
# @author Luís Augusto Weber Mercado [lawmercado@inf.ufrgs.br]
# @author Nicholas de Aquino Lau [nalau@inf.ufrgs.br]
#

default:
	lex scanner.l
	yacc -d parser.y
	gcc -o etapa2 main.c lex.yy.c y.tab.c hash.c

clean:
	rm -rf lex.yy.c y.tab.c y.tab.h etapa2
