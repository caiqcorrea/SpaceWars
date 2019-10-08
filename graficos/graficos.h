/* INTERFACE PARA A BIBLIOTECA GRÁFICA PARA O JOGO SPACE WARS
 *
 *
 *
 *
 *
 */

#ifndef _GRAFICOS_H
#define _GRAFICOS_H

#include "../base/auxiliar.h"
#include "../base/vetores.h"
#include "xwc.h"

#define SIZE_X_WIN 1280
#define SIZE_Y_WIN (SIZE_X_WIN * SIZE_RATIO)

#define NUM_ROTACOES 12

typedef enum 
{
	IMG_NAVE1,
	IMG_NAVE2,
	IMG_NAVE3,
	IMG_NAVE4,
	IMG_PROJ_PADRAO,
	IMG_PROJ_CARLINHOS,
	IMG_PROJ_N_POSSIVEL,
	IMG_PROJ_AAA,
	IMG_PLANETA1,
	IMG_PLANETA2,
	IMG_EXPLOSAO,
	IMG_BOOSTER_NINA,
	IMG_BOOSTER_PICCIONE,
	IMG_BOOSTER_MANDEL,
	IMG_BOOSTER_GUBI,
	IMG_BOOSTER_CARLINHOS,
	IMG_FUNDO,
	NUM_SPR //SEMPRE POR ÚLTIMO, ESTE É APENAS O NÚMERO DE ELEMENTOS DO ENUM
} NOME_SPR;

//Matriz com todas as imagens usadas ao longo do programa.
//Cada linha possui uma imagem padrão e suas 11 rotações.
extern PIC pics[NUM_SPR][NUM_ROTACOES];

typedef struct
{
	NOME_SPR img;	  //Array de rotação uniforme da imagem
	double angle;	  //Ângulo atual da sprite
	int height, width; //Tamanho em pixels
} Sprite;

//Carrega as imagens na matriz pics.
//As imagens devem estar em uma pasta ./pics/i
//, onde . indica o caminho até o executável e i indica o índice de uma imagem
//dentro do enum NOME_SPR.
//Dentro da pasta i, estão todas rotações da i-ésima sprite
//Ou seja, para a linha i da matriz pics, temos que todos os elementos desse linhas
//estão contidos na pasta i.
void picsInit();

//Dada uma sprite, retorna a imamgem atual da sprite de acordo com o ângulo atual dela
PIC getImg(Sprite spr);

//Dada uma sprite e um ângulo ang, incrementa o ângulo da imagem em ang
void rotateSprite(Sprite spr, double ang);

//Dado uma sprite, uma posição e uma janela, a função deseja a imagem atual
//da sprite na posição (em pixels) da janela.
void desenhaSpriteEm(WINDOW *win, Sprite spe, vet2D pos);

//Desenha o fundo na tela *win
//Note que ao desenhar o fundo, todos os conteúdos da janela são sobrescritos
void desenhaFundo(WINDOW *win);

#endif