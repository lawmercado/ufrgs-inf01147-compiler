# ufrgs-inf01147-tp
Compilers module final assignment. Currently in the second phase.

## Running
```
make
./etapa2 inputfile
```
Where 'inputfile' contains the text to be evaluated by the parser.

## Cleaning
```
make clean
```

## Status
According to the portuguese assignment definition.

### Phase 2
- [x] o programa principal deve chamar uma única vez a rotina yyparse para reconhecer
programas que fazem parte da linguagem, e se concluída com sucesso, a análise deve
retornar o valor 0 com exit(0);
- [x] imprimir uma mensagem de erro sintático para os programas não reconhecidos,
informando a linha onde o erro ocorreu, e retornar o valor 3 como código genérico de
erro sintático, chamando exit(3);
- [x] os nodos armazenados na tabela hash devem distinguir entre os tipos de símbolos
armazenados, e o nodo deve ser associado ao token retornado através da atribuição
para yylval.symbol;

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
