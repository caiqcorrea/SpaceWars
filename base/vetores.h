/* Esta é a interface de uma biblioteca genérica para vetores 2D.
 * A princípio, ela pode ser utilizada para quaisquer fins.
 */

#ifndef _VETORES_H
#define _VETORES_H

/* M A C R O S */

/* Quando se quer criar um vetor nulo ou um dos vetores canônicos,
 *  pode-se chamar pelo nome deles
 */
#define NULL_VET null_vet()
#define I_VET i_vet()
#define J_VET j_vet()

/* E S T R U T U R A S */

typedef struct
{
	double x;
	double y;
} vet2D; //Vetor em 2D, representado em duas coordenadas cartesianas

typedef struct
{
	vet2D v1, v2; //Vetores colunas da matriz
} mat2D; //Uma matriz 2x2

/* F U N Ç Õ E S */

//Norma do vetor em 2D
double norma(vet2D v); 

//Multiplicação por escalar, retorna a*v
vet2D mult(double a, vet2D v); 

// Divisão por escalar, retorna v/a
vet2D divide(double a, vet2D v); 

//Transforma v no seu respectivo versor
vet2D versor(vet2D v); 

//Soma de vetores, retorna v+w
vet2D soma(vet2D v, vet2D w); 

//Subtração de vetores, retorna v-w
vet2D sub(vet2D v, vet2D w); 

// Produto escalar entre vetores, retorna <v,w>
double escalar(vet2D v, vet2D w); 

// Retorna || v X w ||
double vetorial(vet2D v, vet2D w); 

//Metodo que normaliza o vetor v
void normaliza(vet2D *v); 

//Cria e retorna um vetor nulo
vet2D null_vet(); 

//Cria e retorna um vetor cartesiano (1,0)
vet2D i_vet(); 

//Cria e retorna um vetor cartesiano (0,1)
vet2D j_vet(); 

//Dados um vetor x e uma matriz M, faz o produto Mx
vet2D multiplicaPorMatriz(mat2D M, vet2D x);

//Dado um vetor x e um angulo, rotaciona esse vetor pelo angulo
vet2D rotaciona(vet2D x, double angulo);

#endif