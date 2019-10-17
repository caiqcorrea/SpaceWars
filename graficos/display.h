/* BIBLIOTECA QUE INTEGRA A PARTE GRÁFICA COM A PARTE FÍSICA
 *
 */

#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "../base/vetores.h"
#include "../fisica/fisica.h"

//Função que, dada uma posição utilizada pela biblioteca física, converte para
//uma posição para ser utilizada pela biblioteca gráfica.
//Por exemplo, o ponto (0,0) da física é (width/2 , height/2) da tela
//Esta função deve fazer essa conversãoS
vet2D converteParaPixel(vet2D pos_em_metros);

//Desenha o pleneta p na janela win
void desenhaPlaneta(Planeta p);

//Desenha a nave n na janela win
void desenhaNave(Nave n);

//Desenha o projétil p na janela win
void desenhaProj(Projetil p);

//Desenha todos os planetas na janela win
void desenhaPlanetas();

//Desenha todos as naves na janela win
void desenhaNaves();

//Desenha todos os projéteis na janela win
void desenhaProjs();

//Desenha todos os objetos na janela win
void desenhaTodos();

//Desenha uma explosao na posição p
void desenhaExplosao(vet2D p);

//Desenha um booster na posição p
void desenhaBooster(vet2D p);

#endif
