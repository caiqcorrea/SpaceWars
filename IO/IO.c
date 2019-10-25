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
			proxLeitura();
			verificaIgualApos("planeta");
			lerPlaneta(indicePlaneta);
			indicePlaneta++;
		}
		else if (strigual("nave"))
		{
			proxLeitura();
			verificaIgualApos("nave");
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
			leVerificaIgualAtribuiInt((int *)&(novo.spr.img), "spr");
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
			leVerificaIgualAtribuiInt((int *)&(nova.spr.img), "spr");
		else
			throwException("lerNave",
						   "Parece que há alguma variável entre [ e ] que não está definida.\n"
						   "Verifique se o arquivo de inputs está correto.",
						   file_format_exception);
	}

	naves[index] = nova;
}

/* FUNÇÕES PARA BOOSTERS */

//Função que lê o arquivo booster.cfg
void lerBoosters()
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
			totalBoostersPreCriados++; //Um a mais para o padrão (que vai na posição 0)
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
	//strcmp é TRUE se as strings são diferentes
	while (strcmp(proxLeitura(), "]"))
	{
		if (strigual("nome"))
		{
			leVerificaIgualAtribuiString(novo.nome, "nome");
			printf("Nome do booster lido\n");
			printf("novo.nome = '%s'\n", novo.nome);
		}
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
		else if (strigual("raio"))
			leVerificaIgualAtribuiDouble(&(novo.radius), "raio");
		else if (strigual("spr"))
			leVerificaIgualAtribuiInt((int *)&(novo.spr.img), "spr");
		else
			throwException("leBooster",
						   "Parece que há alguma variável entre [ e ] que não está definida.\n"
						   "Verifique se o arquivo booster.cfg está correto.",
						   file_format_exception);
	}

	imprimeBoosterPreCriados();
	boostersPreCriados[index] = novo;
}

static void verificaIgualApos(string nome)
{
	char msgErro[100];
	strcpy(msgErro, "É preciso e um sinal de igual após a palavra ");
	strcat(msgErro, nome);
	strcat(msgErro, "!");

	if (!strigual("="))
		throwException("verificaIgualApos", msgErro, file_format_exception);
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

	//Assume que s é uma string já mallocada!
	strcpy(s, proxLeitura());
}