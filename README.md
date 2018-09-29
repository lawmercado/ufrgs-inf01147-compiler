# ufrgs-inf01147-compiler
Compilers module final assignment. Currently in the third phase.

## Running
```
make
./etapa3 inputfile outputfile
```
Where 'inputfile' contains the text to be evaluated by the parser and to have its syntactical tree created.
In the 'outputfile' will be generated a similar source code of the 'inputfile', which is generated based on the created syntactical tree.

## Cleaning
```
make clean
```

## Status
According to the portuguese assignment definition.

- [x] Corrijir a especificação das suas expressões em relação à etapa anterior,
simplificando-as de forma que as produções utilizem a forma mais simples que é
ambígua, e retirando a ambiguidade pela declaração da precedência correta dos
operadores com as definições %left, %right ou %nonassoc;
- [x] Implementar a estrutura da árvore de sintaxe, com funções de criação de nodo,
impressão de um nodo e impressão da árvore;
- [x] Colocar ações sintáticas ao lado das regras de produção descritas no arquivo para
o yacc, as quais criam ou propagam os nodos da árvore, montando-a
recursivamente segundo a análise. A maior parte das ações serão chamadas à
rotina de criação de nodo;
- [x] Finalmente, você deve implemetar uma rotina que percorre a árvore e gera um
programa fonte bastate semelhante, funcionalmente equivalente ao programa
analisado;

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
