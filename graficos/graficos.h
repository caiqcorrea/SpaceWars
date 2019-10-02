/* INTERFACE PARA A BIBLIOTECA GRÁFICA PARA O JOGO SPACE WARS
 *
 *
 *
 *
 *
 */

#ifndef _GRAFICOS_H
#define _GRAFICOS_H

#include "../fisica/vetores.h"
#include "../auxiliar.h"

#define SIZE_X_WIN 1280
#define SIZE_Y_WIN (SIZE_X_WIN*SIZE_RATIO)

//#define IMG_FUNDO //definir mais tarde
//#define IMG_EXPLOSAO //definir mais tarde

typedef struct{
	PIC imgs[];
	int n_img;
	double angle; 
} Sprite;

PIC getImg(Sprite spr);

void rotateSprite(Sprite spr, double ang);

void colocaImgEm(WINDOW *win, PIC pic, vet2D pos);

//Desenha o fundo na tela *win
void desenhaFundo(WINDOW *win);

//Desenha o pleneta p na janela win
void desenhaPlaneta(WINDOW *win, Planeta p);

//Desenha a nave n na janela win
void desenhaNave(WINDOW *win, Nave n);

//Desenha o projétil p na janela win
void desenhaProj(WINDOW *win, Projetil p);

//Desenha todos os planetas na janela win
void desenhaPlanetas(WINDOW *win);

//Desenha todos as naves na janela win
void desenhaNaves(WINDOW *win);

//Desenha todos os projéteis na janela win
void desenhaProjs(WINDOW *win);

//Desenha todos os objetos na janela win
void desenhaTodos(WINDOW *win);

//Desenha uma explosao na posição pos
void desenhaExplosao(WINDOW *win, vet2D pos);

#endif
