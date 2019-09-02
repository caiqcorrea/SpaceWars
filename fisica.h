#include<stdio.h>
#include "vetores.h"
#define G 6.67e-11
#define Dt 1e-6			//Escolha arbitraria de tempo pequeno o suficiente
#define SIZE_X 200 	//Tamanho da horizontal da tela
#define SIZE_Y 200 	//Tamanho da vertical da tela

/* Convenções de nomenclatura:
		Array: Variáveis do tipo v[MAX]
		Vetor: Estrutura vet2D, representa um vetor em R²
*/

/* ESTRUTURAS E VARIÁVEIS GLOBAIS */

typedef struct{
	char* nome;	//Nome da nave
	double m;		//Massa da nave
	vet2D pos;		//Posição da nave
	vet2D vel;		//Velocidade da nave
} naveStruct;

typedef struct{
	double m;		//Massa do projetil
	vet2D pos;		//Posição do projetil
	vet2D vel;		//Velocidade do projetil
} projStruct;

extern double M; //Massa do planeta, lida no arquivo principal
extern double R; //Raio do planeta, lido no arquivo principal
extern double T; //Tempo de simulação, lido no arquivo principal
vet2D posPlaneta = {0, 0}; //Posição do planeta, padronizada como (0,0)

/* PROTOTIPAGEM DAS FUNÇÕES */

vet2D Forca(vet2D p1, vet2D p2, double m1, double m2); //Calcula a força gravitacional que o objeto 2 (p2, m2) exerce no objeto 1;

void incVel(vet2D F, vet2D *v, double m); //Dada uma força e uma massa, atualiza a velocidade para o próximo passo de tempo

int incPos(vet2D v, vet2D *p); //Dada uma velocidade, atualiza a posiçao para o próximo passo de tempo
													  //Retorna 1 se o objeto colidiu com o planeta, 0 caso contrário

vet2D totForca(naveStruct nave[2], projStruct proj[], int N, int nave_ou_proj, int i); //Calcula a força gravitacional total exercida num objeto
/* Parâmetros:
		naves: Array contendo as duas naves
		projs: Array contendo todos os projéteis
		N: Número de projéteis
		nave_ou_proj: Indica se queremos calcular a força exercida numa nave(0) ou num projétil(1)
		i: Indica a nave ou projétil na qual estamos calculando a força
*/

/* IMPLEMENTAÇÃO DAS FUNÇÕES */

vet2D Forca(vet2D p1, vet2D p2, double m1, double m2){
	vet2D F, aux;
	
	aux = sub(p2, p1);
	F = escalar(-G*m1*m2/pow(2, norma(aux)), versor(aux));
	
	return F;
}

void incVel(vet2D F, vet2D *v, double m){
	*v = soma(*v, escalar(m*Dt, F));
}

int incPos(vet2D v, vet2D *p){
	*p = soma(*p, escalar(Dt, v));

//Casos em que o objeto sai da tela: trazemos ele de volta do lado oposto

	while(p->x > ((double) SIZE_X/2))
		p->x -= SIZE_X;

	while(p->x < -((double) SIZE_X/2))
		p->x += SIZE_X;

	while(p->y > ((double) SIZE_Y/2))
		p->y -= SIZE_Y;

	while(p->y < -((double) SIZE_Y/2))
		p->y += SIZE_Y;
		
	if(norma(sub(*p, posPlaneta))<R)
		return 1;
		
	return 0;
}

vet2D totForca(naveStruct nave[2], projStruct proj[], int N, int nave_ou_proj, int i){
	vet2D F;
	int j;
	
	if(nave_ou_proj){
		F = Forca(proj[i].pos, posPlaneta, proj[i].m, M);
		for(j=0 ; j<N ; j++)
			F = soma(F, Forca(proj[i].pos, nave[j].pos, proj[i].m, nave[j].m));
		for(j=0 ; j<N ; j++){
			if(j==i) continue;
			F = soma(F, Forca(proj[i].pos, proj[j].pos, proj[i].m, proj[j].m));
		}
	}
	
	else {
		F = Forca(nave[i].pos, posPlaneta, nave[i].m, M);
		F = soma(F, Forca(nave[i].pos, nave[!i].pos, nave[i].m, nave[!i].m));
		for(j=0 ; j<N ; j++)
			F = soma(F, Forca(nave[i].pos, proj[j].pos, nave[i].m, proj[j].m));
	}
	
	return F;
}
