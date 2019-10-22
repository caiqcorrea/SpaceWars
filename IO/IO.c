/* A biblioteca IO deve conter tudo relacionado a saída e entrada
 * de dados ao programa que não seja das entradas e saída padrão.
 * 
 * Utiliza-se esta biblioteca para leitura ou escrita de arquivos.
 * Esta biblioteca não é genérica. Suas funções são todas feitas
 * com objetivo de ser parte do jogo Space Wars.
 */

#include "IO.h"

#include "../fisica/fisica.h"
#include "../fisica/gerenciadorBooster.h"
#include "../base/vetores.h"
#include "../base/auxiliar.h"
#include "../base/leitor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* P R O T O T I P O   D E   F U N Ç Õ E S   L O C A I S */

static void verificaIgualApos(string nome);
static void leVerificaIgualAtribuiDouble(double *f, string nomeVar);
static void leVerificaIgualAtribuiInt(int *i, string nomeVar);
static void leVerificaIgualAtribuiVet2D(vet2D *v, string nomeVar);
static void leVerificaIgualAtribuiString(string s, string nomeVar);

/* I M P L E M E N T A Ç Ã O   D A S   F U N Ç Õ E S */

/* FUNÇÕES DE INICIALIZAÇÃO DA PARTE FÍSICA */
/*void lerInputs(FILE *arq)
{
	int i;
	double tempoDeVida;

	lerTerra(arq);			   //Lê a primeira linha e atribui valores ao planeta
	lerNave(arq, &(naves[0])); //Lê a segunda linha e atribui valores à primeira nave
	lerNave(arq, &(naves[1])); //Lê a segunda linha e atribui valores à segunda nave
	fscanf(arq, "%d %lf", &tot_obj[PROJETIL], &tempoDeVida);
	for (i = 0; i < tot_obj[PROJETIL]; i++)
		lerProjetil(arq, &(projs[i]), tempoDeVida);
	printf("O arquivo foi lido com sucesso.\n");
	fclose(arq);
}*/

void lerInputs(string nome)
{
	int indicePlaneta, indiceNave;
	indiceNave = indicePlaneta = 0;
	initLeitor(nome);

	//A ideia é que cada iteração do while leia uma linha inteira
	//Se isso não aconteceu é porque, provavelmente, algum erro aconteceu
	while (proxLeitura() != NULL)
	{
		if (strigual("dt"))
			leVerificaIgualAtribuiDouble(&dt, "dt");
		else if (strigual("planeta"))
		{
			verificaIgualApos("nave");
			lerPlaneta(indicePlaneta);
			indicePlaneta++;
		}
		else if (strigual("nave"))
		{
			verificaIgualApos("planeta");
			lerNave(indiceNave);
			indiceNave++;
		}
		else
		{
			char errorMsg[100];
			strcpy(errorMsg, "Houve uma leitura errada no arquivo de configurações!\n");
			strcat(errorMsg, "'");
			strcat(errorMsg, getLeitura());
			strcat(errorMsg, "' é a leitura.\n");
			throwException("lerInputs", errorMsg, file_format_exception);
		}
	}

	disposeLeitor();
}

void lerPlaneta(int index)
{
	proxLeitura(); //deve ser um [
	if (!strigual("["))
		throwException("lerPlaneta", "Deve haver um '[' após 'planeta ='", file_format_exception);

	Planeta novo;
	getPlanetaPadrao(&novo);

	while (strcmp(proxLeitura(), "]")) //Lê até achar um ]
	{
		if (strigual("pos"))
			leVerificaIgualAtribuiVet2D(&(novo.pos), "pos");
		else if (strigual("vel"))
			leVerificaIgualAtribuiVet2D(&(novo.vel), "vel");
		else if (strigual("raio"))
			leVerificaIgualAtribuiDouble(&(novo.radius), "raio");
		else if (strigual("mass"))
			leVerificaIgualAtribuiDouble(&(novo.mass), "mass");
		else if (strigual("spr"))
			leVerificaIgualAtribuiInt((int*) &(novo.spr.img), "spr");
		else
			throwException("lerPlaneta",
						   "Parece que há alguma variável entre [ e ] que não está definida.\n"
						   "Verifique se o arquivo de inputs está correto.",
						   file_format_exception);
	}

	planetas[index] = novo;
}

void lerNave(int index)
{
	proxLeitura(); //deve ser um [
	if (!strigual("["))
		throwException("lerNave", "Deve haver um '[' após 'nave ='", file_format_exception);

	Nave nova;
	getNavePadrao(&nova); //Se qualquer valor não estiver definido no cfg,
	//então ficará com o valor padrão

	while (strcmp(proxLeitura(), "]")) //Lê até achar um ]
	{
		if (strigual("nome"))
			leVerificaIgualAtribuiString(nova.nome, "nome");
		else if (strigual("pos"))
			leVerificaIgualAtribuiVet2D(&(nova.pos), "pos");
		else if (strigual("vel"))
			leVerificaIgualAtribuiVet2D(&(nova.vel), "vel");
		else if (strigual("raio"))
			leVerificaIgualAtribuiDouble(&(nova.radius), "raio");
		else if (strigual("mass"))
			leVerificaIgualAtribuiDouble(&(nova.mass), "mass");
		else if (strigual("hp"))
			leVerificaIgualAtribuiInt(&(nova.HP), "hp");
		else if (strigual("spr"))
			leVerificaIgualAtribuiInt((int *) &(nova.spr.img), "spr");
		else
			throwException("lerPlaneta",
						   "Parece que há alguma variável entre [ e ] que não está definida.\n"
						   "Verifique se o arquivo de inputs está correto.",
						   file_format_exception);
	}

	naves[index] = nova;
}

/*void lerTerra(FILE *arq)
{
	TERRA.pos = NULL_VET;
	fscanf(arq, "%lf %lf %lf", &(TERRA.radius), &(TERRA.mass), &tRestante);
	TERRA.spr.angle = 0;
	TERRA.spr.img = IMG_PLANETA1;
}

void lerNave(FILE *arq, Nave *n)
{
	n->nome = mallocSafe(sizeof(string) * 50);
	fscanf(arq, "%s %lf %lf %lf %lf %lf", n->nome, &(n->mass), &(n->pos.x), &(n->pos.y), &(n->vel.x), &(n->vel.y));
	n->HP = MAX_HP;
	n->radius = RAIO_NAVES;
	defineBoosterComo(&(n->boosterAtual), *BoosterPadrao());
	n->spr.angle = 0;
	n->spr.img = IMG_NAVE1;
}

void lerProjetil(FILE *arq, Projetil *p, double tempoDeVida)
{
	fscanf(arq, "%lf %lf %lf %lf %lf", &(p->mass), &(p->pos.x), &(p->pos.y), &(p->vel.x), &(p->vel.y));
	p->tempoRestante = tempoDeVida;
	p->radius = RAIO_PROJS;
	p->dano = 1;
	p->spr.angle = 0;
	p->spr.img = IMG_PROJ_PADRAO;
}*/

/* FUNÇÕES PARA BOOSTERS */

//Função que lê o arquivo booster.cfg
void leituraBoosters()
{
	int indice = 1;

	initLeitor(BOOSTERCFG_PATH);

	//A ideia é que cada iteração do while leia uma linha inteira
	//Se isso não aconteceu é porque, provavelmente, algum erro aconteceu
	while (proxLeitura() != NULL)
	{
		if (strigual("maxVel"))
			leVerificaIgualAtribuiVet2D(&maxVel, "maxVel");
		else if (strigual("minVel"))
			leVerificaIgualAtribuiVet2D(&minVel, "minVel");
		else if (strigual("maxMass"))
			leVerificaIgualAtribuiDouble(&maxMass, "maxMass");
		else if (strigual("minMass"))
			leVerificaIgualAtribuiDouble(&minMass, "minMass");
		else if (strigual("maxTempoRestanteTela"))
			leVerificaIgualAtribuiDouble(&maxTempoRestanteTela, "maxTempoRestanteTela");
		else if (strigual("minTempoRestanteTela"))
			leVerificaIgualAtribuiDouble(&minTempoRestanteTela, "minTempoRestanteTela");
		else if (strigual("maxTempoRestanteNave"))
			leVerificaIgualAtribuiDouble(&maxTempoRestanteNave, "maxTempoRestanteNave");
		else if (strigual("minTempoRestanteNave"))
			leVerificaIgualAtribuiDouble(&minTempoRestanteNave, "minTempoRestanteNave");
		else if (strigual("propBooster"))
			leVerificaIgualAtribuiDouble(&propBooster, "propBooster");
		else if (strigual("totalBoosters"))
		{
			leVerificaIgualAtribuiInt(&totalBoostersPreCriados, "totalBoosters");
			boostersPreCriados = malloc(sizeof(Booster) * totalBoostersPreCriados);

			defineBoosterPadrao();
		}
		else if (strigual("["))
			lerBooster(indice++);
		else
		{
			char errorMsg[100];
			strcpy(errorMsg, "Houve uma leitura errada no arquivo booster.cfg!\n");
			strcat(errorMsg, "'");
			strcat(errorMsg, getLeitura());
			strcat(errorMsg, "' é a leitura.\n");
			throwException("leituraBoosters", errorMsg, file_format_exception);
		}
	}

	disposeLeitor();
}

void lerBooster(int index)
{
	Booster novo;
	getBoosterPadrao(&novo);

	//Deve ser chamada assim que um [ foi lido e termina sua operação ao ler um ]
	//strcmp é TRUE se as strings são diferentes (não sei porque, mas é assim)
	while (strcmp(proxLeitura(), "]"))
	{
		if (strigual("nome"))
			leVerificaIgualAtribuiString(novo.nome, "nome");
		else if (strigual("vidaAdicional"))
			leVerificaIgualAtribuiInt(&(novo.vidaAdicional), "vidaAdicional");
		else if (strigual("cadencia"))
			leVerificaIgualAtribuiInt(&(novo.cadencia), "cadencia");
		else if (strigual("dano"))
			leVerificaIgualAtribuiInt(&(novo.proj.dano), "dano");
		else if (strigual("tempoProj"))
			leVerificaIgualAtribuiDouble(&(novo.proj.tempoRestante), "tempoProj");
		else if (strigual("massProj"))
			leVerificaIgualAtribuiDouble(&(novo.proj.mass), "massProj");
		else if (strigual("spr"))
			leVerificaIgualAtribuiInt((int*) &(novo.spr.img), "spr");
		else
			throwException("leBooster",
						   "Parece que há alguma variável entre [ e ] que não está definida.\n"
						   "Verifique se o arquivo booster.cfg está correto.",
						   file_format_exception);
	}

	boostersPreCriados[index] = novo;
}

static void verificaIgualApos(string nome)
{
	char msgErro[100];
	strcpy(msgErro, "É preciso e um sinal de igual após a palavra ");
	strcat(msgErro, nome);
	strcat(msgErro, "!");

	if (!strigual("="))
		throwException("leituraBoosters", msgErro, file_format_exception);
}

static void leVerificaIgualAtribuiDouble(double *f, string nomeVar)
{
	proxLeitura();
	verificaIgualApos(nomeVar);
	*f = atof(proxLeitura());
}

static void leVerificaIgualAtribuiInt(int *i, string nomeVar)
{
	proxLeitura();
	verificaIgualApos(nomeVar);
	*i = atoi(proxLeitura());
}

static void leVerificaIgualAtribuiVet2D(vet2D *v, string nomeVar)
{
	proxLeitura(); //deve ser um '='
	verificaIgualApos(nomeVar);

	v->x = atof(proxLeitura());
	proxLeitura(); //Jogar a vírgula fora
	v->y = atof(proxLeitura());
}

static void leVerificaIgualAtribuiString(string s, string nomeVar)
{
	proxLeitura(); //deve ser um '='
	verificaIgualApos(nomeVar);
	strcpy(s, getLeitura());
}