/* A biblioteca auxiliar contém uma série de utilidades que
 * podem ser utilizadas por qualquer parte do programa.
 */

#include "auxiliar.h"

#include <stdlib.h>
#include <stdio.h>

void *mallocSafe(int size)
{
	void *ponteiro;
	ponteiro = malloc(size);
	if (ponteiro == NULL)
		throwException("mallocSafe", "Socorro! Malloc devolveu NULL!", stack_overflow_exception);
	return ponteiro;
}

void freeSafe(void *pnt)
{
	free(pnt);
	pnt = NULL;
}

void throwException(string nomeFunc, string mensagem, errorCode e)
{
	fprintf(stderr, "ERRO NA FUNCAO %s()\n%s\nerror code = %d\n", nomeFunc, mensagem, e);
	exit(EXIT_FAILURE);
}

/* 
 * pause()
 *
 * Para a execucao do programa ate que um ENTER seja digitado.
 */
void pause()
{
	char ch;

	printf("Digite ENTER para continuar. ");
	do
	{
		scanf("%c", &ch);
	} while (ch != '\n'); //ENTER
}

long geraRandomicoEntre(long min, long max)
{
	long num;
	num = rand() % (max - min + 1); //0 <= num <= (max-min)
	num += min;						//min <= num <= max
	return num;
}