#include "fisica.c"
#include <stdlib.h>

/* PROTOTIPAGEM DAS FUNÇÕES */

//Função que lê todos os inputs de um arquivo informado pelo usuário e atribui valores às variáveis globais
void lerInputs();

//Função que lê o planeta e inicializa-o
void lerPlaneta(FILE *arq);

//Função que lê uma nave e inicializa-a
void lerNave(FILE *arq, Nave *n);

//Função que lê um projétil e inicializa-o
void lerProjetil(FILE *arq, Projetil *p, double tempoDeVida);

/* IMPLEMENTAÇÃO DAS FUNÇÕES */

void lerInputs()
{
	int i;
	double tempoDeVida;
	string nomeArq;
	FILE *arq;

	nomeArq = mallocSafe(sizeof(string) * 50);

	printf("Digite o nome do seu arquivo: ");
	scanf("%s", nomeArq);
	arq = fopen(nomeArq, "r");

	if (arq == NULL)
	{
		printf("Não foi possivel abrir o arquivo.\n");
		exit(EXIT_FAILURE); //Sai do programa e indica que deu erro.
							//NÃO SEI AINDA SE exit(EXIT_FAILURE) SAI IMEDIATAMENTE DO PROGRAMA
	}

	lerPlaneta(arq);		   //Lê a primeira linha e atribui valores ao planeta
	lerNave(arq, &(naves[0])); //Lê a segunda linha e atribui valores à primeira nave
	lerNave(arq, &(naves[1])); //Lê a segunda linha e atribui valores à segunda nave
	fscanf(arq, "%d %lf", &tot_projs, &tempoDeVida);
	for (i = 0; i < tot_projs; i++)
		lerProjetil(arq, &(projs[i]), tempoDeVida);
	printf("O arquivo foi lido com sucesso.\n");
	fclose(arq);
	free(nomeArq);
}

void lerPlaneta(FILE *arq)
{
	TERRA.pos = NULL_VET;
	fscanf(arq, "%lf %lf %lf", &(TERRA.raio), &(TERRA.mass), &dt);
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
