#include<math.h>

/* ESTRUTURAS E VARIÁVEIS GLOBAIS */

typedef struct{
	double x;
	double y;
} vet2D; //Vetor em 2D, representado em duas coordenadas cartesianas

/* PROTOTIPOS DAS FUNÇÕES */

double norma(vet2D v); //Norma do vetor em 2D

vet2D escalar(double a, vet2D v); //Multiplicação escalar, retorna a*v

vet2D versor(vet2D v); //Versor que aponta no sentido de v

vet2D soma(vet2D v, vet2D w); //Soma de vetores, retorna v+w

vet2D sub(vet2D v, vet2D w); //Subtração de vetores, retorn v-w

/* IMPLEMENTAÇÃO DAS FUNCÕES */

double norma(vet2D v){
	return sqrt(v.x*v.x + v.y*v.y);
}

vet2D escalar(double a, vet2D v){
	v.x *= a;
	v.y *= a;
	return v;
}

vet2D versor(vet2D v){
	return escalar(1/norma(v), v);
}

vet2D soma(vet2D v, vet2D w){
	v.x += w.x;
	v.y += w.y;
	return v;
}

vet2D sub(vet2D v, vet2D w){
	v.x -= w.x;
	v.y -= w.y;
	return v;
}
