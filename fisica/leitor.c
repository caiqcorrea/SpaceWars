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
        throwException("initLeitor()", "Arquivo não encontrado\n", file_not_found_exception);
}

void disposeLeitor()
{
    fclose(arq);
    free(leituraAtual);
}

void imprimeAtual()
{
    printf("''%s''", getLeitura());
}

string proxLeitura()
{
    fscanf(arq, "%s", leituraAtual);
    return getLeitura();
}

string getLeitura()
{
    return leituraAtual;
}

Bool strigual(string s1)
{
    //Este método é feito especialmente para desfazer essa coisa estranha da string.h
    return !(strcmp(s1, leituraAtual));
}