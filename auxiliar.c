/* A biblioteca auxiliar contém uma série de utilidades que
 * podem ser utilizadas por qualquer parte do programa.
 */


#include <stdlib.h>
#define string char*

typedef enum
{
	FALSE = 0,
	TRUE = 1
} Bool;

void *mallocSafe(int size);
void freeSafe(void *pnt);

void *mallocSafe(int size){
	void *ponteiro;
	ponteiro = malloc(size);
	if(ponteiro == NULL) {
		printf("Houve um erro na alocação de memória!");
		exit(EXIT_FAILURE);
	}
	return ponteiro;
}

void freeSafe(void *pnt){
	free(pnt);
	pnt = NULL;
}