#include "Fisica.cxx"

void LerPlaneta(){
	TERRA.pos = NULL_VET;
	scanf("%lf %lf %lf", &(TERRA.raio), &(TERRA.mass), &dt); 
}

void LerNave(*Nave n){
	scanf("%s %lf %lf %lf %lf %lf", &(n->nome), &(n->mass), &(n->vel.x), &(n->vel.y), &(n->pos.x), &(n->pos.y));
}

void LerProjetil(*Projetil p){
	scanf("%lf %lf %lf %lf %lf", &(p->mass), &(p->vel.x), &(p->vel.y), &(p->pos.x), &(p->pos.y));
}

 