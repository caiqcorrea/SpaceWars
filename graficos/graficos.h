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

#define MAX_NOME_SPR_FILE 128

/* ---- ESTRUTURAS E VARIÁVEIS GLOBAIS ---- */

//Esta janela deve ser utilizada para servir receber todas as atualizações
//de tela antes de passarmos para a tela principal que está sendo exibida para o usuário.
//Após colocarmos tudo o que deve ser exibido em um quadro, damos um workbenchFlush()
//para jogar essa janela na janela principal.
extern WINDOW *workbench;

//Esta é a janela que é exibida ao usuário durante todo o jogo.
//IMPORTANTE: não atualize esta janela diretamente.
//Todas as modificações feitas entre um frame e outro devem ser feitas no workbench e,
//ao mudar de quadro, damos um workbenchFlush() para atualizar a showingWindow.
extern WINDOW *showingWindow;

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

//Estrutura que contém um array de PICs, junto com o número de elementos do array
//e o tamanho dos PICs
struct Pics
{
	PIC *imgs;		//Array de imagens
	int n_imgs;	//Número de imagens
	int height, width; //Tamanho em pixels
};

//Matriz com todas as imagens usadas ao longo do programa.
//Cada linha possui uma imagem e, por padrão, suas 11 rotações
extern struct Pics pics[NUM_SPR];

typedef struct
{
	NOME_SPR img;	  //Array de rotação uniforme da imagem
	double angle;	  //Ângulo atual da sprite
} Sprite;

/* ---- FUNÇÕES ---- */

//Inicializa as janelas no formato correto
//Deve ser chamada ANTES de picsInit
//Não é recomendado chamar esta função, para inicializar corretamente use grafInit()
void winsInit();

//Carrega as imagens na matriz pics.
//As imagens devem estar em uma pasta ./pics/i
//, onde . indica o caminho até o executável e i indica o índice de uma imagem
//dentro do enum NOME_SPR.
//Dentro da pasta i, estão todas rotações da i-ésima sprite
//Ou seja, para a linha i da matriz pics, temos que todos os elementos desse linhas
//estão contidos na pasta i.
//Inicializa os PICs no formato da janela win
//Não é recomendado chamar esta função, para inicializar corretamente use grafInit()
void picsInit(WINDOW *win);

//Inicializa os pics e as janelas corretamente
void grafInit();

//Dada uma sprite, retorna a imamgem atual da sprite de acordo com o ângulo atual dela
PIC getImg(Sprite spr);

//Dada uma sprite e um ângulo ang, incrementa o ângulo da imagem em ang
void rotateSprite(Sprite *spr, double ang);

//Dada uma sprite e um ângulo ang, seta o angulo da imagem para ang
void setAngSprite(Sprite *spr, double ang);

//Dado uma sprite, uma posição e uma janela, a função deseja a imagem atual
//da sprite na posição (em pixels) da janela.
void desenhaSpriteEm(WINDOW *win, Sprite spr, vet2D pos);

//Desenha o fundo index na tela *win
//Podem haver vários fundos. Caso index seja maior que o
//número de funda, acontece um erro
void desenhaFundo_Index(WINDOW *win, int index);

//Desenha o fundo 0 na tela *win
//Note que ao desenhar o fundo, todos os conteúdos da janela são sobrescritos
void desenhaFundo(WINDOW *win);

//Esta atualiza showingWindow com o conteúdo de workbench, sobrescrevendo o que havia lá
void workbenchFlush();

#endif
