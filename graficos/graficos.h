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

#define SIZE_RATIO 9.0 / 16
#define SIZE_X_WIN 1280
#define SIZE_Y_WIN (int)(SIZE_X_WIN * SIZE_RATIO)

#define NUM_ROTACOES 12

#define MAX_NOME_SPR_FILE 128

/* ---- ESTRUTURAS E VARIÁVEIS GLOBAIS ---- */

//O enum com o nome de todas as imagens na pasta /pics
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

//Array que contém todas as imagens do jogo
//Cada índice possui um struct Pics, que é uma struct com uma imagem e todas suas rotações.
//Além disso, essa estrutura possui, para auxílio, o número de rotações e
//o comprimento e altura da imagem

struct
{
	PIC *imgs;		   //Array de imagens
	MASK *msks;		   //Array de masks associado
	int n_imgs;		   //Número de imagens
	int height, width; //Tamanho em pixels
} pics[NUM_SPR];

typedef struct
{
	NOME_SPR img; //Array de rotação uniforme da imagem
	double angle; //Ângulo atual da sprite
} Sprite;

/* ---- FUNÇÕES ---- */

//Inicializa as janelas no formato correto
//Deve ser chamada ANTES de picsInit
//Não é recomendado chamar esta função, para inicializar corretamente use grafInit()
void winsInit();

//ReadPic() com error handling
PIC ReadPicSafe(WINDOW *win, char *fname, MASK m);

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

//Dada uma sprite, retorna a mascara associada a imagem atual da sprite, de acordo com seu angulo
MASK getMsk(Sprite spr);

//Dados dois indices, retorna um sprite com a img e o angulo certo para que
//getImg e getMsk retornem pics[spr].imgs[i] e pics[spr].msks[i], respectivamente
Sprite getSpriteFromPic(NOME_SPR spr, int i);

//Dada uma sprite e um ângulo ang, incrementa o ângulo da imagem em ang
void rotateSprite(Sprite *spr, double ang);

//Dada uma sprite e um ângulo ang, define o ângulo da imagem para ang
void setSpriteAng(Sprite *spr, double ang);

//Dado uma sprite, uma posição e uma janela, a função deseja a imagem atual
//da sprite na posição (em pixels) da janela.
void desenhaSpriteEm(WINDOW *win, Sprite spr, vet2D p);

//Chama desenhaSpriteEm em workbench
void desenhaSprite(Sprite spr, vet2D p);

//Desenha o fundo index na tela *win. Podem haver mais de um fundo.
//Caso index seja maior que o número de fundos, acontece um erro.
void desenhaFundo_Index(WINDOW *win, int index);

//Desenha o fundo 0 na tela *win
//Note que ao desenhar o fundo, todos os conteúdos da janela são sobrescritos
void desenhaFundo(WINDOW *win);

//Mesmo que desenhaFundo_Index e desenhaFundo, mas desenha no workbench
void desenhaFundoWorkbench_Index();
void desenhaFundoWorkbench();

//Esta função atualiza showingWindow com o conteúdo de workbench, sobrescrevendo o que havia lá
void workbenchFlush();

//Desaloca toda a memória alocada. DEVE ser chamada antes do fim do programa
void grafFree();

#endif
