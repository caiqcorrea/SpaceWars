/* A biblioteca IO deve conter tudo relacionado a saída e entrada
 * de dados ao programa que não seja das entradas e saída padrão.
 * 
 * Utiliza-se esta biblioteca para leitura ou escrita de arquivos.
 * Esta biblioteca não é genérica. Suas funções são todas feitas
 * com objetivo de ser parte do jogo Space Wars.
 */

#include "IO.h"

#include "../fisica/fisica.h"
#include "../base/vetores.h"
#include "../base/auxiliar.h"
#include "../base/leitor.h"
#include "gerenciadorBooster.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* P R O T O T I P O   D E   F U N Ç Õ E S   L O C A I S */

static void verificaIgualApos(string nome);
static void leVerificaIgualAtribuiDouble(double *f, string nomeVar);
static void leVerificaIgualAtribuiInt(int *i, string nomeVar);
static void leVerificaIgualAtribuiVet2D(vet2D *v, string nomeVar);

/* I M P L E M E N T A Ç Ã O   D A S   F U N Ç Õ E S */

/* FUNÇÕES DE INICIALIZAÇÃO DA PARTE FÍSICA */
void lerInputs(FILE *arq)
{
	int i;
	double tempoDeVida;

	dt = 0.1;				   //Inicializamos o dt como 0.001 (este valor pode ser alterado)
	lerTerra(arq);			   //Lê a primeira linha e atribui valores ao planeta
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
	fscanf(arq, "%lf %lf %lf", &(TERRA.radius), &(TERRA.mass), &tRestante);
}

void lerNave(FILE *arq, Nave *n)
{
	n->nome = mallocSafe(sizeof(string) * 50);
	fscanf(arq, "%s %lf %lf %lf %lf %lf", n->nome, &(n->mass), &(n->pos.x), &(n->pos.y), &(n->vel.x), &(n->vel.y));
	n->HP = MAX_HP;
	n->radius = RAIO_NAVES;
	defineBoosterComo(&(n->boosterAtual), BoosterPadrao);
}

void lerProjetil(FILE *arq, Projetil *p, double tempoDeVida)
{
	fscanf(arq, "%lf %lf %lf %lf %lf", &(p->mass), &(p->pos.x), &(p->pos.y), &(p->vel.x), &(p->vel.y));
	p->tempoRestante = tempoDeVida;
	p->radius = RAIO_PROJS;
	p->dano = 1;
}

/* FUNÇÕES PARA BOOSTERS */

//Função que lê o arquivo booster.cfg
void leituraBoosters()
{
	totalBoostersPreCriados = 0;
	int indiceProx = 1;

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

			//Fazemos isto para indicar que ainda não há um booster padrão
			BoosterPadrao.nome = "NULL";
		}
		else if (strigual("["))
		{
			if (totalBoostersPreCriados == 0)
				throwException("leituraBoosters",
							   "O array de booster não foi alocado ainda\n"
							   "Há um erro de formatação no arquivo boosters.cfg\n"
							   "Deve haver um 'totalBoosters' antes do primeiro booster",
							   file_format_exception);

			//Lemos um booster e colocamos-a no índice 'indiceProx' do array de pré criados
			//Como a função devolve TRUE se o booster lido foi o padrão, então
			//devemos decrementar o valor de indiceProx, pois ele não deve ser incrementado
			//nesse caso (já que colocamos o BoosterPadrao em uma posição não sequêncial)
			if (leBooster(indiceProx++))
				indiceProx--;
		}
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

Bool leBooster(int index)
{
	char nome[50];
	int i;
	int dano, cadencia, vidaAdicional;
	double tempoProj, massProj;
	Bool ehPadrao = FALSE;
	dano = cadencia = vidaAdicional =
		tempoProj = massProj = -1; //Inidica que os campos não foram definidos

	//Deve ser chamada assim que um [ foi lido e termina sua operação ao ler um ]
	//strcmp é TRUE se as strings são diferentes (não sei porque, mas é assim)
	while (strcmp(proxLeitura(), "]"))
	{
		if (strigual("nome"))
		{
			proxLeitura();
			verificaIgualApos("nome");
			proxLeitura();
			ehPadrao = strigual("PADRAO");
			strcpy(nome, getLeitura());
		}
		else if (strigual("vidaAdicional"))
		{
			proxLeitura();
			//Se ao invés de igual é - e o booster padrão já foi setado
			if (strigual("-") && strcmp("NULL", BoosterPadrao.nome) != 0)
				vidaAdicional = BoosterPadrao.vidaAdicional;
			else
			{
				verificaIgualApos("vidaAdicional");
				vidaAdicional = atoi(proxLeitura());
			}
		}
		else if (strigual("cadencia"))
		{
			proxLeitura();
			//Se ao invés de igual é - e o booster padrão já foi setado
			if (strigual("-") && strcmp("NULL", BoosterPadrao.nome) != 0)
				cadencia = BoosterPadrao.cadencia;
			else
			{
				verificaIgualApos("cadencia");
				cadencia = atoi(proxLeitura());
			}
		}
		else if (strigual("dano"))
		{
			proxLeitura();
			//Se ao invés de igual é - e o booster padrão já foi setado
			if (strigual("-") && strcmp("NULL", BoosterPadrao.nome) != 0)
				dano = BoosterPadrao.proj.dano;
			else
			{
				verificaIgualApos("dano");
				dano = atoi(proxLeitura());
			}
		}
		else if (strigual("tempoProj"))
		{
			proxLeitura();
			//Se ao invés de igual é - e o booster padrão já foi setado
			if (strigual("-") && strcmp("NULL", BoosterPadrao.nome) != 0)
				tempoProj = BoosterPadrao.proj.tempoRestante;
			else
			{
				verificaIgualApos("tempoProj");
				tempoProj = atof(proxLeitura());
			}
		}
		else if (strigual("massProj"))
		{
			proxLeitura();
			//Se ao invés de igual é - e o booster padrão já foi setado
			if (strigual("-") && strcmp("NULL", BoosterPadrao.nome) != 0)
				massProj = BoosterPadrao.proj.mass;
			else
			{
				verificaIgualApos("massProj");
				massProj = atof(proxLeitura());
			}
		}
		else
			throwException("leBooster",
						   "Parece que há alguma variável entre [ e ] que não está definida.\n"
						   "Verifique se o arquivo booster.cfg está correto.",
						   file_format_exception);
	}

	//Após ler, vamos instanciar o booster e colocá-lo em seu lugar do array

	//Primeiro verificamos se todos os campos estão definidos
	if (dano == -1 || cadencia == -1 || vidaAdicional == -1 ||
		tempoProj == -1 || massProj == -1 || strcmp(nome, "") == 0)
	{
		throwException("leBooster",
					   "Há algum booster em booster.cfg que não teve todos seus campos definidos!",
					   file_format_exception);
	}

	Booster novo;
	novo.nome = mallocSafe(sizeof(char) * strlen(nome));
	for (i = 0; (novo.nome[i] = nome[i]) != '\0'; i++)
		; //Garante que o '\0' estará no nome sem ter que fazer um caso separado
	novo.cadencia = cadencia;
	novo.vidaAdicional = vidaAdicional;
	novo.proj.tempoRestante = tempoProj;
	novo.proj.dano = dano;
	novo.proj.pos = novo.proj.vel = NULL_VET;
	novo.proj.mass = massProj;
	novo.proj.radius = RAIO_PROJS;
	novo.mass = geraRandomicoEntre(minMass, maxMass);
	novo.radius = RAIO_BOOSTER;
	do
	{
		novo.pos.x = geraRandomicoEntre(-SIZE_X_FIS, SIZE_X_FIS);
		novo.pos.y = geraRandomicoEntre(-SIZE_Y_FIS, SIZE_Y_FIS);
	} while (ChecaColisaoComTodos(novo.o));
	novo.vel.x = geraRandomicoEntre(minVel.x, maxVel.x);
	novo.vel.y = geraRandomicoEntre(minVel.y, minVel.y);
	novo.tempoRestanteNave = geraRandomicoEntre(minTempoRestanteNave, maxTempoRestanteNave);
	novo.tempoRestanteTela = geraRandomicoEntre(minTempoRestanteTela, maxTempoRestanteTela);

	if (ehPadrao)
		defineBoosterComo(&(BoosterPadrao), novo);
	else
		defineBoosterComo(&(boostersPreCriados[index]), novo);

	return ehPadrao;
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
	*i = atof(proxLeitura());
}

static void leVerificaIgualAtribuiVet2D(vet2D *v, string nomeVar)
{
	proxLeitura(); //deve ser um '='
	verificaIgualApos(nomeVar);

	v->x = atof(proxLeitura());
	proxLeitura(); //Jogar a vírgula fora
	v->y = atof(proxLeitura());
}
