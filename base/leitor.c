/* Arquivo que implementa um leitor de arquivos (uma espécie de analisador léxico)
 *
 */
#include "leitor.h"
#include "../base/auxiliar.h"

#include <stdio.h>
#include <string.h>

static string leituraAtual;
static FILE *arq;

void initLeitor(string nome)
{
    leituraAtual = mallocSafe(sizeof *leituraAtual * 200);
    arq = fopen(nome, "r");

    if (arq == NULL)
        throwException("initLeitor()", "Arquivo não encontrado", file_not_found_exception);
}

void disposeLeitor()
{
    fclose(arq);
    freeSafe(leituraAtual);
}

void imprimeAtual()
{
    printf("''%s''", getLeitura());
}

string proxLeitura()
{
    int c;
    c = fscanf(arq, " %s", leituraAtual);
    if (c == -1)
        return NULL;

    return getLeitura();
}

string getLeitura()
{
    return leituraAtual;
}

Bool strigual(string s1)
{
    return !(strcmp(s1, leituraAtual));
}
