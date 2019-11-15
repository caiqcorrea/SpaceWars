/* Arquivo que implementa um leitor de arquivos (uma espécie de analisador léxico)
 *
 */
#include "leitor.h"

#include "../base/auxiliar.h"

#include <stdio.h>
#include <string.h>

static string leituraAtual = NULL;
static FILE *arq = NULL;

void initLeitor(string nome)
{
    leituraAtual = mallocSafe(sizeof *leituraAtual * 200);
    arq = fopen(nome, "r");

    if (arq == NULL)
        throwException("initLeitor()",
                       "Arquivo não encontrado",
                       file_not_found_exception);
}

void disposeLeitor()
{
    freeSafe(leituraAtual);
    if (arq == NULL)
        return;
    fclose(arq);
}

void imprimeAtual()
{
    if (getLeitura() == NULL)
        return;
    printf("''%s''", getLeitura());
}

string proxLeitura()
{
    int c;

    if (arq == NULL)
        return NULL;

    c = fscanf(arq, " %s", leituraAtual);
    if (c == -1)
        return NULL;

    return getLeitura();
}

string getLeitura()
{
    return leituraAtual;
}

Bool strigual(string s)
{
    if (getLeitura() == NULL)
        return FALSE;
    return !(strcmp(s, getLeitura()));
}
