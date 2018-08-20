# ufrgs-inf01147-tp
Compilers module final assignment. Currently in the first phase.

## Running
```
make
./etapa1 input.txt output.txt
```
Where 'input.txt' contains the text to be evaluated and 'output.txt' contains the expected output.

## Cleaning
```
make clean
```

## Status
According to the portuguese assignment definition.

### Phase 1
- [x] reconhecer as expressões regulares que descrevem cada tipo de lexema;
- [x] classificar os lexemas reconhecidos em tokens retornando as constantes definidas no
arquivo tokens.h fornecido ou codigos ascii para caracteres simples;
- [x] incluir os identificadores e os literais (inteiros, caracteres e strings) em uma tabela de
símbolos implementada com estrutura hash;
- [x] controlar o número de linha do arquivo fonte, e fornecer uma função declarada como
int getLineNumber(void) a ser usada nos testes e pela futura análise sintática;
- [x] ignorar comentários no formato C99 de única linha e múltiplas linhas;
- [x] informar erro léxico ao encontrar caracteres inválidos na entrada, retornando o token
de erro;
- [x] definir e atualizar uma variável global e uma função int isRunning(void), que
mantém e retorna valor true (diferente de 0) durante a análise e muda para false (igual
a 0) ao encontrar a marca de fim de arquivo;
