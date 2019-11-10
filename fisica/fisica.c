/* BIBLIOTECA FÍSICA PARA O JOGO SPACE WARS
 *
 * Esta biblioteca foi criada especificamente para o jogo Space Wars,
 * Sua base são as três structs: Nave, Projétil e Planeta, que devem
 * se encontrar instanciadas dentro dos arrays: naves, projs e planetas.
 * 
 * Decidimos implementar uma struct mais básica chamada de Objeto, que existirá
 * dentro de cada uma das structs mais importantes. Ela possuirá inicialmente
 * os atributos: massa, posição e velocidade, mas a ideia é que todos os
 * atributos compartilhados pelos três structs estejam dentro de Objeto (como,
 * por exemplo, o raio).
 * Dessa forma, podemos criar funções genéricas que recebem apenas Objetos ao
 * invés de três (ou mais) funções que fazem a mesma coisa, mas uma para cada
 * tipo de struct.
 * 
 * Todos os calculos feitos são baseados na Teoria da Gravitação Universal de
 * Isaac Newton.
 * https://en.wikipedia.org/wiki/Newton%27s_law_of_universal_gravitation
 * É sabido que determinar posições exatas de orbitas complexas (como as do jogo) 
 * é um problema em aberto na matemática e na física, então o professor nos
 * recomendou usar o método numérico de Runge-Kutta.
 * 
 * Convenções de nomenclatura:
 *		Array: Variáveis do tipo v[MAX]
 *		Vetor: Estrutura vet2D, representa um vetor em R²
 */

#include "fisica.h"

#include "../base/vetores.h"
#include "../base/auxiliar.h"
#include "../graficos/graficos.h"

#include <math.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

/*--------------- V A R I Á V E I S   G L O B A I S ---------------*/

int tot_obj[NUM_TIPO_OBJ] = MAX_OBJ; //O número de objetos de cada tipo

/* Armazenaremos todos os objetos na tela através de arrays globais,
 * um para cada tipo: Nave, Planeta e Projetil.
 * Os tamanhos desses arrays estão definidos no começo desse arquivo e podem ser
 * editados hard-coded (quem sabe futuramente em tempo de execução)
 */
Nave naves[MAX_NAVES];			//O array que contém as duas naves dos jogadores
Planeta planetas[MAX_PLANETAS]; //O array que contém o planeta central
Projetil projs[MAX_PROJ];		//O array que contém os projéteis que estão atualmente na tela
Booster boosters[MAX_BOOSTERS]; //O array que contém os boosters que estão atualmente na tela

//Intervalo de tempo da simulacao, lido no arquivo principal.
double dt;

/*--------------- I M P L E M E N T A Ç Ã O   D A S   F U N Ç Õ E S ---------------*/

void getObjetoPadrao(Objeto *o)
{
	o->p.x = 0;
	o->p.y = 0;
	o->v.x = 0;
	o->v.y = 0;
	o->m = 0;
	o->r = 1;
	o->s.img = -1;
	o->s.angle = 0;
}

void getPlanetaPadrao(Planeta *p)
{
	getObjetoPadrao(&(p->o));
}

void getProjetilPadrao(Projetil *p)
{
	p->dano = 0;
	p->tempoRestante = 0;
	getObjetoPadrao(&(p->o));
}

Objeto *GetObjeto(TipoObj tipo, int indice)
{
	if (!(0 <= indice && indice < tot_obj[tipo]))
		throwException("GetObjeto", "indexOutOfRangeException", index_out_of_range_exception);

	else
		switch (tipo)
		{
		case PLANETA:
			return &(planetas[indice].o);

		case NAVE:
			return &(naves[indice].o);

		case PROJETIL:
			return &(projs[indice].o);

		case BOOSTER:
			return &(boosters[indice].o);

		default:
			throwException("GetObjeto", "tipo nao identificado", var_type_undefined_exception);
		}

	return NULL; //Só para o compilador não reclamar
}
void SetObjeto(TipoObj tipo, int indice, Objeto o)
{
	if (!(0 <= indice && indice < tot_obj[tipo]))
		throwException("SetObjeto", "indexOutOfRangeException", index_out_of_range_exception);

	else
		switch (tipo)
		{
		case PLANETA:
			planetas[indice].o = o;
			break;

		case NAVE:
			naves[indice].o = o;
			break;

		case PROJETIL:
			projs[indice].o = o;
			break;

		case BOOSTER:
			boosters[indice].o = o;
			break;

		default:
			throwException("SetObjeto", "tipo nao identificado", var_type_undefined_exception);
		}
}

Bool ObjetoDuplicado(Objeto o1, Objeto o2)
{
	return &o1 == &o2; //Se seus endereços são o mesmo, são o mesmo objeto
}

Bool ObjetoIgual(Objeto o1, Objeto o2)
{
	return (o1.m == o2.m &&
			o1.p.x == o2.p.x &&
			o1.p.y == o2.p.y &&
			o1.r == o2.r &&
			o1.v.x == o2.v.x &&
			o1.v.y == o2.v.y);
}

vet2D Forca(Objeto o1, Objeto o2)
{
	vet2D aux = sub(o1.p, o2.p);
	if (norma(aux) < 1000) /* Uma distância mínima */
		return NULL_VET;

	return mult(-G * o1.m * o2.m / pow(norma(aux), 2), versor(aux));
}

void IncVel(vet2D F, Objeto *o)
{
	o->v = soma(o->v, mult((dt / (o->m)), F));
	if (norma(o->v) > MAX_VEL)
	{
		normaliza(&(o->v));  //Guardamos sua direção
		o->v = mult(MAX_VEL, o->v); //Fazemos seu módulo ser a velocidade máxima
	}
}

void IncPos(Objeto *o)
{
	o->p = soma(o->p, mult(dt, o->v));

	//Casos em que o objeto sai da tela: trazemos ele de volta do lado oposto
	while (o->p.x > SIZE_X_FIS / 2)
		o->p.x -= SIZE_X_FIS;

	while (o->p.x < -SIZE_X_FIS / 2)
		o->p.x += SIZE_X_FIS;

	while (o->p.y > SIZE_Y_FIS / 2)
		o->p.y -= SIZE_Y_FIS;

	while (o->p.y < -SIZE_Y_FIS / 2)
		o->p.y += SIZE_Y_FIS;
}

vet2D CalculaForcaSobre(Objeto o)
{
	vet2D F = NULL_VET;
	int i;
	TipoObj tipo;

	for (tipo = 0; tipo < NUM_TIPO_OBJ; tipo++)
		for (i = 0; i < tot_obj[tipo]; i++)
			F = soma(F, Forca(o, *GetObjeto(tipo, i)));

	return F;
}

double DistanciaEntre(Objeto o1, Objeto o2)
{
	return norma(sub(o1.p, o2.p));
}

void AtualizaObjeto(Objeto *o)
{
	IncVel(CalculaForcaSobre(*o), o);
	IncPos(o);
}

void AtualizaObjetos()
{
	int i;
	TipoObj tipo;

	for (tipo = 0; tipo < NUM_TIPO_OBJ; tipo++)
		if (tipo != PLANETA) //Planetas não precisam ser atualizados (pelo menos na versão atual)
			for (i = 0; i < tot_obj[tipo]; i++)
				AtualizaObjeto(GetObjeto(tipo, i));
}

void giraObjetoVel(Objeto *o)
{
	if (norma(o->v) == 0)
		setSpriteAng(&(o->s), 0);
	else
		setSpriteAng(&(o->s), anguloX(o->v));
}

void giraObjetoVelTipo(TipoObj tipo)
{
	int i;
	if (tipo > NUM_TIPO_OBJ)
		throwException("giraObjetoVelTipo", "Esse tipo não existe", index_out_of_range_exception);
	for (i = 0; i < tot_obj[tipo]; i++)
		giraObjetoVel(GetObjeto(tipo, i));
}

void ReduzTempoProj(Projetil *p)
{
	p->tempoRestante -= dt;
}

/* Essa função foi criada para facilitar a edição de código caso queiramos, no futuro,
 * mudar a forma de reduzir o tempo dos projéteis (como reduzir apenas de alguns e de outros não)
 */
void ReduzTempoProjs()
{
	int i;
	for (i = 0; i < tot_obj[PROJETIL]; i++)
		ReduzTempoProj(&(projs[i]));
}

Bool VerificaSeProjMorreu(Projetil p)
{
	return p.tempoRestante <= 0;
}

void CriaProjetil(Nave n)
{

	projs[tot_obj[PROJETIL]].radius = n.projetil.radius;

	//Começa um pouco distante da nave
	projs[tot_obj[PROJETIL]].pos = soma(mult((n.radius + projs[tot_obj[PROJETIL]].radius + 100), rotaciona(I_VET, n.spr.angle)),
										n.pos);
	//O 100 é uma distância de segurança para não colidirem

	//Começa com velocidade vInicial e direção da velocidade da nave
	projs[tot_obj[PROJETIL]].vel = mult(n.projetil.vInicial, rotaciona(I_VET, n.spr.angle));

	projs[tot_obj[PROJETIL]].mass = n.projetil.mass;
	projs[tot_obj[PROJETIL]].spr.img = n.projetil.spr.img;
	projs[tot_obj[PROJETIL]].spr.angle = n.projetil.spr.angle;
	projs[tot_obj[PROJETIL]].dano = n.projetil.dano;
	projs[tot_obj[PROJETIL]].tempoRestante = n.projetil.tempoRestante;

	//Existem outras características de um projétil que não são necessárias
	// de se definir. (cadencia, vInicial)

	tot_obj[PROJETIL]++;
}

void RemoveProj(int index)
{
	int i;
	for (i = index; i < tot_obj[PROJETIL] - 1; i++)
		projs[i] = projs[i + 1];
	tot_obj[PROJETIL]--;
}

Bool ChecaColisaoEntre(Objeto o1, Objeto o2)
{
	return ChecaColisaoComRaio(o1, o2, o1.r + o2.r);
}

Bool ChecaColisaoComRaio(Objeto o1, Objeto o2, double raio)
{
	if (ObjetoDuplicado(o1, o2))
		return FALSE;
	if (ObjetoIgual(o1, o2))
		return FALSE; //Isso pode gerar erros, mas a chance é dx
	return (DistanciaEntre(o1, o2) < raio);
}

Bool ChecaColisaoComTodos(Objeto o)
{
	int i;
	Bool colidiu = FALSE;

	for (i = 0; i < tot_obj[PROJETIL] && !colidiu; i++) //PROJÉTEIS
		colidiu = ChecaColisaoEntre(o, *GetObjeto(PROJETIL, i));

	for (i = 0; i < tot_obj[NAVE] && !colidiu; i++)
		colidiu = ChecaColisaoEntre(o, *GetObjeto(NAVE, i));

	for (i = 0; i < tot_obj[PLANETA] && !colidiu; i++)
		colidiu = ChecaColisaoEntre(o, *GetObjeto(PLANETA, i));

	for (i = 0; i < tot_obj[BOOSTER] && !colidiu; i++)
		colidiu = ChecaColisaoEntre(o, *GetObjeto(BOOSTER, i));

	return colidiu;
}

void ChecaTodasColisoes()
{
	int i, j;
	Bool colidiu;
	//PROJÉTEIS
	for (i = 0; i < tot_obj[PROJETIL]; i++)
	{
		colidiu = FALSE;

		//com PLANETAS
		for (j = 0; !colidiu && j < tot_obj[PLANETA]; j++)
			if (ChecaColisaoEntre(*GetObjeto(PROJETIL, i), *GetObjeto(PLANETA, j)))
			{
				RemoveProj(i);
				colidiu = TRUE;
			}

		//com outro PROJÉTIL
		for (j = i + 1; !colidiu && j < tot_obj[PROJETIL]; j++)
			//Note que começamos de i + 1 pois assumimos todos os i-1 projéteis anteriores não colidiram com ninguém
			//e i não colide consigo mesmo.
			//Conclusão: se i colide com um j, então j > i
			if (ChecaColisaoEntre(*GetObjeto(PROJETIL, i), *GetObjeto(PROJETIL, j)))
			{
				RemoveProj(j); //Como j > i, removemos ele primeiro. Assim o i-ésimo projétil do array continua sendo aquele
				RemoveProj(i); //que colidiu. Se fosse o contrário (remover i primeiro) seria o (j-1)-ésimo quem colidiu com o i-ésimo
				colidiu = TRUE;
			}

		//com NAVES
		for (j = 0; !colidiu && j < tot_obj[NAVE]; j++)
			if (ChecaColisaoEntre(*GetObjeto(PROJETIL, i), *GetObjeto(NAVE, j)))
			{
				RemoveProj(i);
				DecrementaVida(&(naves[j]), projs[i].dano);
				colidiu = TRUE;
			}

		if (colidiu)
			i--; //Removemos o i-ésimo, então o próximo a ser visto é quem virou o i-ésimo
				//Note que se o projétil colide com um planeta (a primeira verificação) a função não entra nos outros for's
	}
	//NAVES
	for (i = 0; i < tot_obj[NAVE]; i++)
	{
		colidiu = FALSE;
		//com PLANETAS
		for (j = 0; !colidiu && j < tot_obj[PLANETA]; j++)
			if (ChecaColisaoEntre(*GetObjeto(NAVE, i), *GetObjeto(PLANETA, j)))
			{
				Destroi(&(naves[i]));
				colidiu = TRUE;
			}

		//com outras NAVES
		for (j = i + 1; !colidiu && j < tot_obj[NAVE]; j++)
			//Começamos de i+1 pois as i-1 primeiras naves não colidiram com outras naves e naves[i] não colide com si própria
			if (ChecaColisaoEntre(*GetObjeto(NAVE, i), *GetObjeto(NAVE, j)))
			{
				Destroi(&(naves[j]));
				Destroi(&(naves[i]));
				colidiu = TRUE;
			}

		if (colidiu)
			break; //Se colidiu, paramos por aqui. O jogo terminou.
	}
}

void freeFisica()
{
	int i;
	for (i = 0; i < tot_obj[NAVE]; i++)
		freeSafe(naves[i].nome);
	freeSafe(boostersPreCriados);
}
