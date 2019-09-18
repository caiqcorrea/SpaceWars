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

#include <stdio.h>
#include "vetores.c"
#include "auxiliar.c"

#define G 6.67e-11		  //Constante gravitacional
#define RAIO_COLISAO 10.0 //Raio utilizado para checar colisao com a nave
#define SIZE_X 1e10		  //Tamanho da horizontal da tela
#define SIZE_Y 1e10		  //Tamanho da vertical da tela
#define MAX_PROJ 100	  //Número máximo de projéteis
#define NUM_NAVES 2		  //Número de naves
#define NUM_PLANETAS 1	//Número de planetas

/*--------------- E S T R U T U R A S   E   V A R I Á V E I S   G L O B A I S ---------------*/

/* Struct Objeto possui uma massa, posição e velocidade. 
 * Ele é um objeto genérico que deve estar dentro de outros structs importantes
 * do programa, como as naves, projetéis e planetas.
 */
typedef struct
{
	double m; //massa
	vet2D p;  //posição
	vet2D v;  //velocidade
} Objeto;

/* Struct Nave contém um objeto próprio e um nome que é o nome da nave.
 *
 */
typedef struct
{
	Objeto o;
	string nome;
} Nave;

/* Struct Projetil possui um objeto e um tempoRestante na tela.
 * A cada quadro, o tempo retante é decrementado de dt e ao chegar em zero,
 * devemos exclui-lo do array de projéteis.
 */
typedef struct
{
	Objeto o;
	double tempoRestante;
} Projetil;

/* Struct Planeta possui, além de um objeto, um raio que deve
 * ser usado para cálculos de gravidade e para checar colisões
 */
typedef struct
{
	Objeto o;
	double raio;
} Planeta;

/* Defines que facilitam na hora de acessar um campo dos objetos que estão dentro
 * das structs. Basta fazer naves[0].vel para obter a velocidade da nave 0.
 * Os defines funcionam também caso tenhamos um ponteiro para um struct.
 * Basta fazer ponteiro->vel
 */
#define vel o.v  // Macro para a velocidade de um objeto
#define mass o.m // Macro para a massa de um objeto
#define pos o.p  // Macro para a posição de um objeto

/* Armazenaremos todos os objetos na tela através de arrays globais,
 * um para cada tipo: Nave, Planeta e Projetil.
 * Os tamanhos desses arrays estão definidos no começo desse arquivo e podem ser
 * editados hard-coded (quem sabe futuramente em tempo de execução)
 */
Nave naves[NUM_NAVES];			//O array que contém as duas naves dos jogadores
Planeta planetas[NUM_PLANETAS]; //O array que contém o planeta central
Projetil projs[MAX_PROJ];		//O que array que contém os projéteis que estão atualmente na tela
int tot_projs;					//O número de elementos do array projs

#define TERRA planetas[0] //Como só há um planeta, vamos chamá-lo de TERRA

/* Um enum com os tipos de objetos possíveis.
 * Serve para fazermos referência a qual dos três arrays estamos falando.
 * Por exemplo, uma função x com parâmetros um Objeto e um TipoObj. Dessa forma, com uma só função,
 * nós podemos fazer o mesmo tipo de tarefa aplicada a qualquer um dos três tipos de objeto.
 */
typedef enum
{
	NAVE,
	PLANETA,
	PROJETIL
} TipoObj;

//Intervalo de tempo da simulacao, lido no arquivo principal
double dt;

/*--------------- P R O T O T I P A G E M   D A S   F U N Ç Õ E S ---------------*/

//Dados dois objetos, retorna a força gravitacional exercida entre o1 e o2.
//O vetor retornado deve ser aplicado em o1.
vet2D Forca(Objeto o1, Objeto o2);

//Dado um objeto e uma forca, incrementa a velocidade desse objeto
void IncVel(vet2D F, Objeto *o);

//Dado um objeto e uma velocidade, incrementa a posicao desse objeto
void IncPos(vet2D v, Objeto *o);

//Recebe um objeto e calcula a força gerada sobre ele pelos outros objetos.
//Devemos referenciar o objeto passando seu tipo e qual seu índice dentro do seu respectivo array (nave ou projs)
//(se o objeto é do tipo planeta, então o índice é indiferente)
vet2D CalculaForcaSobre(TipoObj tipo, int index);

//Dados um tipo de objeto e um índice do vetor onde esse objeto se encontra
//a função retorna este objeto.
//A função verifica se o índice está dentro dos limites
Objeto getObjeto(TipoObj tipo, int indice);

//Dados um tipo de objeto, um índice do vetor referente ao tipo desse objeto e o objeto em si,
//a função sobrescreve o array no índice i com o objeto o.
//A função verifica se o índice está dentro dos limites
void setObjeto(TipoObj tipo, int indice, Objeto o);

//Dados dois objetos, retorna a distância entre eles
double distanciaEntre(Objeto o1, Objeto o2);

//Dados dois objetos, retorna TRUE se eles colidiram e FALSE caso contrário.
Bool checaColisaoEntre(Objeto o1, Objeto o2);

/*--------------- I M P L E M E N T A Ç Ã O   D A S   F U N Ç Õ E S ---------------*/

vet2D Forca(Objeto o1, Objeto o2)
{
	vet2D F, aux;

	aux = sub(o1.p, o2.p);
	F = mult(-G * o1.m * o2.m / pow(2, norma(aux)), versor(aux));

	return F;
}

void IncVel(vet2D F, Objeto *o)
{
	o->v = soma(o->v, mult((dt / (o->m)), F));
}

void IncPos(vet2D v, Objeto *o)
{
	o->p = soma(o->p, mult(dt, v));

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

vet2D CalculaForcaSobre(TipoObj tipo, int index)
{
	vet2D F = NULL_VET;
	int i;

	if (tipo == PLANETA)
		return NULL_VET; //Não se aplica força alguma sobre o planeta
	if (tipo == NAVE)	//Se é uma nave
	{
		for (i = 0; i < NUM_NAVES; i++)
		{
			if (i == index)
				continue;
			F = soma(F, Forca(naves[index].o, naves[i].o));
		}
		for (i = 0; i < tot_projs; i++)
			F = soma(F, Forca(naves[index].o, projs[i].o));
		for (i = 0; i < NUM_PLANETAS; i++)
			F = soma(F, Forca(naves[index].o, planetas[i].o));
	}
	else //Se é um projétil
	{
		for (i = 0; i < NUM_NAVES; i++)
			F = soma(F, Forca(projs[index].o, naves[i].o));
		for (i = 0; i < tot_projs; i++)
		{
			if (i == index)
				continue;
			F = soma(F, Forca(projs[index].o, projs[i].o));
		}
		for (i = 0; i < NUM_PLANETAS; i++)
			F = soma(F, Forca(projs[index].o, planetas[i].o));
	}

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

Objeto getObjeto(TipoObj tipo, int indice)
{
	switch (tipo)
	{
	case PLANETA:
		if (0 <= indice && indice < NUM_PLANETAS)
			return planetas[indice].o;
		else
			retorneErro("getObjeto", "indexOutOfRangeException", index_out_of_range_exception);
	case NAVE:
		if (0 <= indice && indice < NUM_NAVES)
			return naves[indice].o;
		else
			retorneErro("getObjeto", "indexOutOfRangeException", index_out_of_range_exception);
	case PROJETIL:
		if (0 <= indice && indice < tot_projs)
			return projs[indice].o;
		else
			retorneErro("getObjeto", "indexOutOfRangeException", index_out_of_range_exception);
	default:
		retorneErro("getObjeto", "tipo nao identificado", var_type_undefined);
	}
}
void setObjeto(TipoObj tipo, int indice, Objeto o)
{
	switch (tipo)
	{
	case PLANETA:
		if (0 <= indice && indice < NUM_PLANETAS)
			planetas[indice].o = o;
		else
			retorneErro("getObjeto", "indexOutOfRangeException", index_out_of_range_exception);
	case NAVE:
		if (0 <= indice && indice < NUM_NAVES)
			naves[indice].o = o;
		else
			retorneErro("getObjeto", "indexOutOfRangeException", index_out_of_range_exception);
	case PROJETIL:
		if (0 <= indice && indice < tot_projs)
			projs[indice].o = o;
		else
			retorneErro("getObjeto", "indexOutOfRangeException", index_out_of_range_exception);
	default:
		retorneErro("getObjeto", "tipo nao identificado", var_type_undefined);
	}
}

/*Função recebe um tipo de objeto, um índice, e um intervalo padrão dt, 
e atualiza o estado de movimento do objeto indicado pelo índice.*/
void AtualizaObj(TipoObj tipo, int index)
{
	vet2D F;
	if (tipo == PLANETA)
		return; /*O planeta não se move.*/
	if (tipo == NAVE)
	{
		F = CalculaForcaSobre(NAVE, index);
		IncVel(F, &(naves[index].o));
		IncPos(F, &(naves[index].o));
	}
	else
	{ /* projetil */
		F = CalculaForcaSobre(PROJETIL, index);
		IncVel(F, &(projs[index].o));
		IncPos(F, &(projs[index].o));
	}
}
/*Recebe um indice e um intervalo dt, e calcula o novo tempo de vida do objeto. 
Se o tempo estiver esgotado, retorna 1. Se não, retorna 0.*/
Bool AtualizaTempo(int index)
{
	projs[index].tempoRestante -= dt;
	return (projs[index].tempoRestante < 0);
}

/*Atualiza o estado de movimento de todos os projéteis e depois de todas as naves, 
a cada intervalo de tempo dt. Realiza n atualizações.*/
void AtualizaPosicoes(double dt, int n, int pause)
{
	int loop_counter, i, j;

	for (loop_counter = 0; loop_counter < n; loop_counter++)
	{
		for (i = 0; i < tot_projs; i++) /*projeteis*/
		{
			if (AtualizaTempo(i) == TRUE) /* Tempo do projetil terminou */
			{
				if (i == tot_projs - 1)
					tot_projs--;
				else
				{
					for (j = i; j < tot_projs - 1; j++)
						projs[j] = projs[j + 1];
					tot_projs--;
				}
			}
		}
		for (i = 0; i < tot_projs; i++) /*Atualiza o estado de movimento dos projeteis remanescentes*/
			AtualizaObj(PROJETIL, i);

		for (int i = 0; i < NUM_NAVES; i++) /* naves */
			AtualizaObj(NAVE, i);
	}
}
