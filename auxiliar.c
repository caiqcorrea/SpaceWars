/* A biblioteca auxiliar contém uma série de utilidades que
 * podem ser utilizadas por qualquer parte do programa.
 */

#include <stdlib.h>
#define string char *

typedef enum
{
	FALSE = 0,
	TRUE = 1
} Bool;

/* Esse enum serve para informar à função retorneErro um código de erro.
 * Dessa forma, o usuário que estiver rodando o programa pode ter mais informações
 * sobre possíveis erros que ele encontra e que já foram tratados pelos arquivos 
 * do projeto.
 */
typedef enum {
	/* Este erro é retornado quando se tenta acessar índices de um array ou matriz
	 * que não deveriam ser acessados pelo usuário.
	 */
	index_out_of_range_exception,

	/* Este erro é retornado quando o usuário tenta acessar um ponteiro nulo ou 
	 * algum campo de um ponteiro nulo.
	 */
	null_pointer_exception,

	/* Este erro é retornado quando uma função tenta alocar memória e o computador
	 * não possui memória disponível.
	 */
	stack_overflow,

	/* Este erro é retornado quando o usuário especifica um tipo de variável errado
	 * para uma função.
	 */ 
	var_type_undefined
} errorCode;

void *mallocSafe(int size);
void freeSafe(void *pnt);
void retorneErro(string nomeFunc, string mensagem, errorCode e);

void *mallocSafe(int size)
{
	void *ponteiro;
	ponteiro = malloc(size);
	if (ponteiro == NULL)
		retorneErro("mallocSafe", "Socorro! Malloc devolveu NULL!", stack_overflow);
	return ponteiro;
}

void freeSafe(void *pnt)
{
	free(pnt);
	pnt = NULL;
}

void retorneErro(string nomeFunc, string mensagem, errorCode e)
{
	fprintf(stderr, "ERRO NA FUNCAO %s()\n%s\nerror code = %d\n", nomeFunc, mensagem, e);
	exit(EXIT_FAILURE);
}