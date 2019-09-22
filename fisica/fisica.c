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

#include "vetores.h"
#include "../auxiliar.h"

#include <math.h>
#include <stddef.h>

/*--------------- V A R I Á V E I S   G L O B A I S ---------------*/

int tot_obj[NUM_TIPO_OBJ] = MAX_OBJ; //O número de objetos de cada tipo

/* Armazenaremos todos os objetos na tela através de arrays globais,
 * um para cada tipo: Nave, Planeta e Projetil.
 * Os tamanhos desses arrays estão definidos no começo desse arquivo e podem ser
 * editados hard-coded (quem sabe futuramente em tempo de execução)
 */
Nave naves[MAX_NAVES];			//O array que contém as duas naves dos jogadores
Planeta planetas[MAX_PLANETAS]; //O array que contém o planeta central
Projetil projs[MAX_PROJ];		//O que array que contém os projéteis que estão atualmente na tela

#define TERRA planetas[0] //Como só há um planeta, vamos chamá-lo de TERRA

//Intervalo de tempo da simulacao, lido no arquivo principal. Por padrão, é 0.001
double dt;

//Tempo restante de simulação;
double tRestante;

/*--------------- I M P L E M E N T A Ç Ã O   D A S   F U N Ç Õ E S ---------------*/

vet2D Forca(Objeto o1, Objeto o2)
{
	vet2D aux = sub(o1.p, o2.p);
	if (norma(aux) == 0)
		return NULL_VET;

	return mult(-G * o1.m * o2.m / pow(norma(aux), 2), versor(aux));
}

void IncVel(vet2D F, Objeto *o)
{
	o->v = soma(o->v, mult((dt / (o->m)), F));
}

void IncPos(Objeto *o)
{
	o->p = soma(o->p, mult(dt, o->v));

	//Casos em que o objeto sai da tela: trazemos ele de volta do lado oposto
	while (o->p.x > SIZE_X / 2)
		o->p.x -= SIZE_X;

	while (o->p.x < -SIZE_X / 2)
		o->p.x += SIZE_X;

	while (o->p.y > SIZE_Y / 2)
		o->p.y -= SIZE_Y;

	while (o->p.y < -SIZE_Y / 2)
		o->p.y += SIZE_Y;
}

vet2D CalculaForcaSobre(Objeto o)
{
	vet2D F = NULL_VET;
	int i;
	TipoObj tipo;

	for (tipo = 0; tipo < NUM_TIPO_OBJ; tipo++)
		for (i = 0; i < tot_obj[tipo]; i++)
			F = soma(F, Forca(o, *getObjeto(tipo, i)));

	return F;
}

double distanciaEntre(Objeto o1, Objeto o2)
{
	return norma(sub(o1.p, o2.p));
}

Bool checaColisaoEntre(Objeto o1, Objeto o2)
{
	return (distanciaEntre(o1, o2) < RAIO_COLISAO);
}

Objeto *getObjeto(TipoObj tipo, int indice)
{
	if (!(0 <= indice && indice < tot_obj[tipo]))
		throwException("getObjeto", "indexOutOfRangeException", index_out_of_range_exception);

	else
		switch (tipo)
		{
		case PLANETA:
			return &(planetas[indice].o);

		case NAVE:
			return &(naves[indice].o);

		case PROJETIL:
			return &(projs[indice].o);

		default:
			throwException("getObjeto", "tipo nao identificado", var_type_undefined_exception);
		}

	return NULL; //Só para o compilador não reclamar
}
void setObjeto(TipoObj tipo, int indice, Objeto o)
{
	if (!(0 <= indice && indice < tot_obj[tipo]))
		throwException("getObjeto", "indexOutOfRangeException", index_out_of_range_exception);

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

		default:
			throwException("getObjeto", "tipo nao identificado", var_type_undefined_exception);
		}
}

void AtualizaObjeto(Objeto *o)
{
	IncPos(o);
	IncVel(CalculaForcaSobre(*o), o);
}

void AtualizaObjetos()
{
	int i;
	TipoObj tipo;
	//Planetas não precisam ser atualizados (pelo menos na versão atual)
<<<<<<< HEAD
	for (tipo = 0; tipo < NUM_TIPO_OBJ; tipo++)
		for (i = 0; i < tot_obj[tipo]; i++)
			if (tipo != PLANETA)
=======
	for(tipo = 0 ; tipo < NUM_TIPO_OBJ ; tipo++)
		if(tipo != PLANETA)
			for (i = 0; i < tot_obj[tipo]; i++)
>>>>>>> 363ae34805b1c0049c7507786eda9c2f10f47d8a
				AtualizaObjeto(getObjeto(tipo, i));
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

void RemoveProj(int index)
{
	int i;
	for(i = index; i < tot_obj[PROJETIL]; i++)
		projs[i] = projs[i+1];
	tot_obj[PROJETIL]--;
}

/* ESTA FUNÇÃO RECEBERÁ ATUALIZAÇÕES NO FUTURO:
 *		Checar colisão (EP2)
 */
Bool AtualizaJogo()
{
	int i;
	//Primeiro atualizamos a posição e velocidade de todos os objetos
	AtualizaObjetos();
	//Depois, devemos reduzir o tempo de todos os projéteis
	ReduzTempoProjs();
	//Verificamos se algum projétil sumiu e removemos-o se sim
	for (i = 0; i < tot_obj[PROJETIL]; i++)
		if (VerificaSeProjMorreu(projs[i])) //Se o projétil morreu
			RemoveProj(i); //Removemos o projétil de índice i

	//VERIFICAR COLISÃO (EP2)

	tRestante -= dt; //Decrementamos o tempo restante de simulação

	return (tRestante > 0); //E a simulação continua enquanto o tempo for positivo
}

void freeAll()
{
	int i;
	for (i = 0; i < tot_obj[NAVE]; i++)
		freeSafe(naves[i].nome);
}
