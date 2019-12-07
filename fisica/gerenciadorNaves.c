/* Este arquivo foi criado para ser um auxilidor com as naves
 * 
 * ATENÇÃO: Este módulo apenas é um gerenciador das naves,
 * por eles terem muitas peculiaridades próprias. Ele é um módulo da biblioteca
 * física.h
 */
#include "fisica.h"

#include "../base/auxiliar.h"
#include "../base/vetores.h"
#include "../graficos/graficos.h"

#include <string.h>

void getNavePadrao(Nave *n)
{
	n->nome = mallocSafe(sizeof(char) * TAM_MAX_NOMES);
	strcpy(n->nome, "NULL");
	defineBoosterComo(&(n->boosterAtual), *BoosterPadrao());
	n->HP = 1;
	n->cooldown = 1000;
	getObjetoPadrao(&(n->o));
}

void DecrementaVida(Nave *n, int valor)
{
	if (n->boosterAtual.vidaAdicional > 0)
	{
		n->boosterAtual.vidaAdicional -= valor;
		if(n->boosterAtual.vidaAdicional < 0)
			//Se ela perdeu o shield, descontamos o resto da vida
			n->HP += n->boosterAtual.vidaAdicional;
	}
	else
		n->HP -= valor;
}

void IncrementaVida(Nave *n, int valor)
{
	n->HP += valor;
}

Bool EstaViva(Nave n)
{
	return (n.HP > 0); //Se 0, está morta
}

Bool TodasEstaoVivas()
{
	int i;
	for (i = 0; i < tot_obj[NAVE]; i++)
		if (!EstaViva(naves[i]))
			return i;
	return -1;
}

void Destroi(Nave *n)
{
	DecrementaVida(n, n->HP); //Faz a vida zerar
}

void Atira(Nave *n)
{
	if (n->cooldown <= 0)
	{
		CriaProjetil(*n);
		n->cooldown = (double)n->projetil.cadencia;
	}
	//Se não, nada acontece
}

void Acelera(Nave *n)
{
	//Adicionamos boosterAtual.aceleracao na velocidade de n
	//  e mantemos sua direção
	n->vel = soma(n->vel, mult(n->boosterAtual.aceleracao * dt, rotaciona(I_VET, n->spr.angle)));
}

void Rotaciona(Nave *n, Bool horario)
{
	//Rodamos a nave um pequeno angulo
	rotateSprite(&(n->spr), (horario ? -1 : 1) * n->boosterAtual.rotacao * dt);
}

void AtualizaCooldown()
{
	int i;
	for (i = 0; i < tot_obj[NAVE]; i++)
		naves[i].cooldown -= dt;
}