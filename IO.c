/* A biblioteca IO deve conter tudo relacionado a saída e entrada
 * de dados ao programa que não seja das entradas e saída padrão.
 * 
 * Utiliza-se esta biblioteca para leitura ou escrita de arquivos.
 * Esta biblioteca não é genérica. Suas funções são todas feitas
 * com objetivo de ser parte do jogo Space Wars.
 */

 #include "IO.h"

#include "fisica/fisica.h"
#include "fisica/vetores.h"
#include "auxiliar.h"

#include<stdio.h>
 
/* I M P L E M E N T A Ç Ã O   D A S   F U N Ç Õ E S */

void lerInputs(FILE *arq)
{
	int i;
	double tempoDeVida;

	lerTerra(arq);		   //Lê a primeira linha e atribui valores ao planeta
	lerNave(arq, &(naves[0])); //Lê a segunda linha e atribui valores à primeira nave
	lerNave(arq, &(naves[1])); //Lê a segunda linha e atribui valores à segunda nave
	fscanf(arq, "%d %lf", &tot_obj[PROJETIL], &tempoDeVida);
	for (i = 0; i < tot_obj[PROJETIL]; i++)
		lerProjetil(arq, &(projs[i]), tempoDeVida);
	printf("O arquivo foi lido com sucesso.\n");
	fclose(arq);
}

void lerTerra(FILE *arq)
{
	TERRA.pos = NULL_VET;
	fscanf(arq, "%lf %lf %lf", &(TERRA.raio), &(TERRA.mass), &tRestante);
	dt = 0.01;
}

void lerNave(FILE *arq, Nave *n)
{
	n->nome = mallocSafe(sizeof(string) * 50);
	fscanf(arq, "%s %lf %lf %lf %lf %lf", n->nome, &(n->mass), &(n->vel.x), &(n->vel.y), &(n->pos.x), &(n->pos.y));
}

void lerProjetil(FILE *arq, Projetil *p, double tempoDeVida)
{
	fscanf(arq, "%lf %lf %lf %lf %lf", &(p->mass), &(p->vel.x), &(p->vel.y), &(p->pos.x), &(p->pos.y));
	p->tempoRestante = tempoDeVida;
}
