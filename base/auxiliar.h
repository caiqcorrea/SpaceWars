/* A biblioteca auxiliar contém uma série de utilidades que
 * podem ser utilizadas por qualquer parte do programa.
 */

#ifndef _AUXILIAR_H
#define _AUXILIAR_H

#define SIZE_RATIO 9/16

typedef char* string;

/* E S T R U T U R A S */

typedef enum
{
	FALSE = 0,
	TRUE = 1
} Bool;

/* Esse enum serve para informar à função throwException um código de erro.
 * Dessa forma, o usuário que estiver rodando o programa pode ter mais informações
 * sobre possíveis erros que ele encontra e que já foram tratados pelos arquivos 
 * do projeto.
 */
typedef enum {
	/* Este erro é retornado quando se tenta acessar índices de um array ou matriz
	 * que não deveriam ser acessados pelo usuário.
	 */
	index_out_of_range_exception = -1,

	/* Este erro é retornado quando o usuário tenta acessar um ponteiro nulo ou 
	 * algum campo de um ponteiro nulo.
	 */
	null_pointer_exception = 0,

	/* Este erro é retornado quando uma função tenta alocar memória e o computador
	 * não possui memória disponível.
	 */
	stack_overflow_exception = 1,

	/* Este erro é retornado quando o usuário especifica um tipo de variável errado
	 * para uma função.
	 */ 
	var_type_undefined_exception = 2,

	/* Este erro é retornando quando se tenta abrir um arquivo e não consegue.
	 */
	file_not_find_exception = 3
	
} errorCode;

/* F U N Ç Õ E S */

void *mallocSafe(int size);
void freeSafe(void *pnt);

//Função que envia uma exception ao strerr com o nome da função que retornou erro e uma mensagem ao usuário
//Além disso, deve receber um código de erro
//Esta função dá para o programa e indica ao sistema que houve um erro na execução
void throwException(string nomeFunc, string mensagem, errorCode e);

//Função pause, copiada do EP1 de Estrutura de Dados e Algoritmos I, matéria ministrada
//pela professora Cris no segundo semestre de 2019
//ime.usp.br/~cris
//
//A função pausa o programa até o usuário dar clicar ENTER
void pause();

#endif