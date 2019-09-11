#include <math.h>

/* ESTRUTURAS E VARIÁVEIS GLOBAIS */

/* Defines para referenciar às funções que retornam vetores prontos
 * Quando se quer criar um vetor nulo ou um dos vetores canônicos, pode-se chamar pelo nome deles
 */
#define NULL_VET null_vet()
#define I_VET i_vet()
#define J_VET j_vet()

typedef struct
{
	double x;
	double y;
} vet2D; //Vetor em 2D, representado em duas coordenadas cartesianas

/* PROTOTIPOS DAS FUNÇÕES */

double norma(vet2D v); //Norma do vetor em 2D

vet2D mult(double a, vet2D v); //Multiplicação por escalar, retorna a*v

vet2D divide(double a, vet2D v); // Divisão por escalar, retorna v/a

vet2D versor(vet2D v); //Transforma v no seu respectivo versor

vet2D soma(vet2D v, vet2D w); //Soma de vetores, retorna v+w

vet2D sub(vet2D v, vet2D w); //Subtração de vetores, retorna v-w

double escalar(vet2D v, vet2D w); // Produto escalar entre vetores, retorna <v,w>

double vetorial(vet2D v, vet2D w); // Retorna a norma do produto vetorial entre os vetores v e w

void normaliza(vet2D *v); //Metodo que normaliza o vetor v

vet2D null_vet(); //Cria e retorna um vetor nulo

vet2D i_vet(); //Cria e retorna um vetor cartesiano (1,0)

vet2D j_vet(); //Cria e retorna um vetor cartesiano (0,1)

/* IMPLEMENTAÇÃO DAS FUNCÕES */

double norma(vet2D v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

vet2D mult(double a, vet2D v)
{
	v.x *= a;
	v.y *= a;
	return v;
}

vet2D divide(double a, vet2D v)
{
	v.x /= a;
	v.y /= a;
	return v;
}

vet2D versor(vet2D v)
{
	return divide(norma(v), v);
}

vet2D soma(vet2D v, vet2D w)
{
	v.x += w.x;
	v.y += w.y;
	return v;
}

vet2D sub(vet2D v, vet2D w)
{
	v.x -= w.x;
	v.y -= w.y;
	return v;
}

double escalar(vet2D v, vet2D w)
{
	return v.x * w.x + v.y * w.y;
}

double vetorial(vet2D v, vet2D w)
{
	return v.x * w.y - v.y * w.x;
}

void normaliza(vet2D *v)
{
	*v = versor(*v);
}

vet2D null_vet()
{
	vet2D nulo = {0, 0};
	return nulo;
}

vet2D i_vet()
{
	vet2D i = {1, 0};
	return i;
}

vet2D j_vet()
{
	vet2D j = {0, 1};
	return j;
}