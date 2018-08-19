lex scanner.l
gcc -o main lex.yy.c main.c
./main input.txt output.txt
