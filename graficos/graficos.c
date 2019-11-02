#include "graficos.h"

#include "../base/auxiliar.h"
#include "../base/vetores.h"
#include "../graficos/xwc.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//Esta janela deve ser utilizada para servir receber todas as atualizações
//de tela antes de passarmos para a tela principal que está sendo exibida para o usuário.
//Após colocarmos tudo o que deve ser exibido em um quadro, damos um workbenchFlush()
//para jogar essa janela na janela principal.
static WINDOW *workbench;

//Esta é a janela que é exibida ao usuário durante todo o jogo.
//IMPORTANTE: não atualize esta janela diretamente.
//Todas as modificações feitas entre um frame e outro devem ser feitas no workbench e,
//ao mudar de quadro, damos um workbenchFlush() para atualizar a showingWindow.
static WINDOW *showingWindow;

void winsInit()
{
	showingWindow = InitGraph(SIZE_X_WIN, SIZE_Y_WIN, "Space Wars");
	workbench = NewPic(showingWindow, SIZE_X_WIN, SIZE_Y_WIN);
}

PIC ReadPicSafe(WINDOW *win, char *fname, MASK m)
{
	PIC pic;
	if ((pic = ReadPic(win, fname, m)) == NULL)
		throwException("ReadPicSafe", strcat(fname, " não encontrado"), file_not_found_exception);
	return pic;
}

void picsInit(WINDOW *win)
{
	int i, j, sizex_aux, sizey_aux, n_imgs;
	char filename[MAX_NOME_SPR_FILE];
	FILE *spr_file;
	for (i = 0; i < NUM_SPR; i++)
	{
		snprintf(filename, sizeof(filename), "./graficos/pics/%d/size", i);
		spr_file = fopen(filename, "r");

		if (spr_file == NULL)
			throwException("picsInit", strcat(filename, " não encontrado."), file_not_found_exception);

		fscanf(spr_file, "%d %d ", &sizex_aux, &sizey_aux);

		if (!feof(spr_file))
			fscanf(spr_file, "%d", &n_imgs);
		else
			n_imgs = NUM_ROTACOES;

		pics[i].n_imgs = n_imgs;
		pics[i].imgs = mallocSafe(n_imgs * sizeof(PIC));
		pics[i].msks = mallocSafe(n_imgs * sizeof(MASK));
		pics[i].width = sizex_aux;
		pics[i].height = sizey_aux;

		for (j = 0; j < n_imgs; j++)
		{
			pics[i].msks[j] = NewMask(win, sizex_aux, sizey_aux);
			snprintf(filename, sizeof(filename), "./graficos/pics/%d/%d.xpm", i, j);
			pics[i].imgs[j] = ReadPic(win, filename, pics[i].msks[j]);
		}
	}

	fclose(spr_file);
}

void grafInit()
{
	winsInit();
	picsInit(workbench);
}

PIC getImg(Sprite spr)
{
	if (spr.img >= NUM_SPR)
		throwException("getSprite()", "Índice do Sprite inválido", index_out_of_range_exception);
	//Retorna a img com rotação mais próxima do ângulo atual
	return pics[spr.img].imgs[((int)round((pics[spr.img].n_imgs * spr.angle) / (2 * M_PI))) % pics[spr.img].n_imgs];
}

MASK getMsk(Sprite spr)
{
	if (spr.img >= NUM_SPR)
		throwException("getSprite()", "Índice do Sprite inválido", index_out_of_range_exception);
	//Retorna a img com rotação mais próxima do ângulo atual
	return pics[spr.img].msks[((int)round((pics[spr.img].n_imgs * spr.angle) / (2 * M_PI))) % pics[spr.img].n_imgs];
}

Sprite getSpriteFromPic(NOME_SPR nome, int i)
{
	Sprite spr;

	if (i > pics[nome].n_imgs)
		throwException("getImgFromPics",
					   "O número passado é maior que o número de rotações",
					   index_out_of_range_exception);

	spr.img = nome;
	spr.angle = (double)i * (2 * M_PI) / pics[nome].n_imgs;
	return spr;
}

void rotateSprite(Sprite *spr, double ang)
{
	spr->angle += ang;
	while (spr->angle > 2 * M_PI) //Deixa o ângulo na primeira determinação positiva
		spr->angle -= 2 * M_PI;
}

void setSpriteAng(Sprite *spr, double ang)
{
	spr->angle = ang;
	while (spr->angle > 2 * M_PI) //Deixa o ângulo na primeira determinação positiva
		spr->angle -= 2 * M_PI;
}

void desenhaSpriteEm(WINDOW *win, Sprite spr, vet2D p)
{
	if (spr.img > NUM_SPR)
	{
		char errmsg[100];
		snprintf(errmsg, 100,
				 "O número da sprite é %d, que é maior que o máximo: %d",
				 spr.img, NUM_SPR);
		throwException("desenhaSpriteEm", errmsg, index_out_of_range_exception);
	}
	SetMask(win, getMsk(spr));
	PutPic(win, getImg(spr), 0, 0, pics[spr.img].width, pics[spr.img].height,
		   ((int)round(p.x)) - (pics[spr.img].width / 2),
		   ((int)round(p.y)) - (pics[spr.img].height / 2));
	UnSetMask(win);
}

void desenhaSprite(Sprite spr, vet2D p)
{
	desenhaSpriteEm(workbench, spr, p);
}

void desenhaFundo_Index(WINDOW *win, int index)
{
	if (index >= pics[IMG_FUNDO].n_imgs || index < 0)
		throwException("desenhaFundo_Index", "Não há fundo correspondente ao índice", index_out_of_range_exception);
	PutPic(win, pics[IMG_FUNDO].imgs[index], 0, 0, pics[IMG_FUNDO].width, pics[IMG_FUNDO].height, 0, 0);
}

void desenhaFundo(WINDOW *win)
{
	desenhaFundo_Index(win, 0);
}

void desenhaFundoWorkbench_Index(int i){
	desenhaFundo_Index(workbench, i);
}
	
void desenhaFundoWorkbench(){
	desenhaFundo_Index(workbench, 0);
}

void workbenchFlush()
{
	PutPic(showingWindow, workbench, 0, 0, SIZE_X_WIN, SIZE_Y_WIN, 0, 0);
	WFlush();
}

void grafFree()
{
	int i, j;

	for (i = 0; i < NUM_SPR; i++)
	{
		for (j = 0; j < pics[i].n_imgs; j++) //Free em todas as imagens dentro de uma pic
			FreePic(pics[i].imgs[j]);
		freeSafe(pics[i].imgs); //Free na pic em si
	}

	FreePic(showingWindow);
	FreePic(workbench);
}
