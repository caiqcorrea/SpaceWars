#include <stdio.h>
#include "Vetores.cxx"
#include "Auxiliar.cxx"
#define G 6.67e-11
#define RAIO_COLISAO 10.0 //Raio utilizado para checar colisao com a nave
#define SIZE_X 200		  //Tamanho da horizontal da tela
#define SIZE_Y 200		  //Tamanho da vertical da tela
#define MAX_PROJ 100
#define NUM_NAVES 2
#define NUM_PLANETAS 1

/* Convenções de nomenclatura:
		Array: Variáveis do tipo v[MAX]
		Vetor: Estrutura vet2D, representa um vetor em R²
*/

/* ESTRUTURAS E VARIÁVEIS GLOBAIS */

typedef struct
{
	double m; //massa
	vet2D p;  //posicao
	vet2D v;
} Objeto;

typedef struct
{
	Objeto o;
	string nome;
} Nave;

typedef struct
{
	Objeto o;
} Projetil;

typedef struct
{
	Objeto o;
	double raio;
} Planeta;

#define vel o.v
#define mass o.m
#define pos o.p

Nave naves[NUM_NAVES];
Planeta planetas[NUM_PLANETAS];
Projetil projs[MAX_PROJ];
int tot_projs;

#define TERRA planetas[0]

enum TipoObj
{
	NAVE,
	PROJETIL,
	PLANETA
};

double dt; //Intervalo de tempo da simulacao, lido no arquivo principal

/* PROTOTIPAGEM DAS FUNÇÕES */

vet2D Forca(Objeto o1, Objeto o2); //Dados dois objetos, retorna o forca gravitacional exercida entre o1 e o2. O vetor retornado deve ser aplicado em o1.

void IncVel(vet2D F, Objeto *o); //Dado um objeto e uma forca, incrementa a velocidade desse objeto

void IncPos(vet2D v, Objeto *o); //Dado um objeto e uma velocidade, incrementa a posicao desse objeto

vet2D CalculaForcaSobre(TipoObj tipo, int index); //Recebe um objeto e calcula a forca gerada sobre ele pelos outros objetos.
//Devemos referenciar o objeto passando seu tipo e qual seu indice dentro do seu respectivo vetor (nave ou projs) (se o objeto e do tipo planeta, entao o indice eh indiferente)

/* IMPLEMENTAÇÃO DAS FUNÇÕES */

vet2D Forca(Objeto o1, Objeto o2)
{
	vet2D F, aux;

	aux = sub(o2.p, o1.p);
	F = mult(-G * o1.m * o2.m / pow(2, norma(aux)), versor(aux));

	return F;
}

void IncVel(vet2D F, Objeto *o)
{
	o->v = soma(o->v, mult(o->m * dt, F));
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
		return NULL_VET;
	if (tipo == NAVE)
	{
		for (i = 0; i < NUM_NAVES; i++)
		{
			if (i == index)
				continue;
			F = soma(F, Forca(naves[index].o, naves[i].o));
		}
		for (i = 0; i < tot_projs; i++)
		{
			F = soma(F, Forca(naves[index].o, projs[i].o));
		}
		F = soma(F, Forca(naves[index].o, planetas[0].o));
	}
	else
	{
		for (i = 0; i < NUM_NAVES; i++)
		{
			F = soma(F, Forca(projs[index].o, naves[i].o));
		}
		for (i = 0; i < tot_projs; i++)
		{
			if (i == index)
				continue;
			F = soma(F, Forca(projs[index].o, projs[i].o));
		}
		F = soma(F, Forca(projs[index].o, planetas[0].o));
	}

	return F;
}

double distanciaEntre(Objeto o1, Objeto o2)
{
	return norma(sub(o1.p, o2.p));
}

Bool checaColisaoEntre(Objeto o1, Objeto o2)
{
	if (distanciaEntre(o1, o2) < RAIO_COLISAO)
		return TRUE;
	return FALSE;
}