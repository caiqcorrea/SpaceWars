#include "graficos.h"

#include "../base/auxiliar.h"
#include "../base/vetores.h"
#include "../graficos/xwc.h"

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

WINDOW *workbench;
WINDOW *showingWindow;

struct Pics pics[NUM_SPR];

void winsInit(){
	showingWindow = InitGraph(SIZE_X_WIN, SIZE_Y_WIN, "showingWIndow");
	workbench = NewPic(showingWindow, SIZE_X_WIN, SIZE_Y_WIN);
}

void picsInit(WINDOW *win){
	int i, j, sizex_aux, sizey_aux, n_imgs;
	char filename[MAX_NOME_SPR_FILE];
	FILE* spr_file;
	for(i=0 ; i<1 ; i++){
		snprintf(filename, sizeof(filename), "pics/%d/size", i);
		spr_file = fopen(filename, "r");
		
		if(spr_file == NULL)
			throwException("picsInit()", strcat(filename, " não encontrado"), file_not_find_exception);
		
		fscanf(spr_file, "%d %d", &sizex_aux, &sizey_aux);
		
		if(getc(spr_file) != EOF)
			fscanf(spr_file, "%d", &n_imgs);
		else
			n_imgs = NUM_ROTACOES;
		
		pics[i].n_imgs = n_imgs;	
		pics[i].imgs = malloc(n_imgs*sizeof(PIC));
		pics[i].width = sizex_aux;
		pics[i].height = sizey_aux;
		
		for(j=0 ; j<n_imgs ; j++){
			snprintf(filename, sizeof(filename), "pics/%d/%d.xpm", i, j);
			pics[i].imgs[j] = ReadPic(win, filename, NULL);
		}
	}
}

void grafInit(){
	winsInit();
	picsInit(workbench);
}

PIC getImg(Sprite spr){
	return pics[spr.img].imgs[ ( (int) round((pics[spr.img].n_imgs*spr.angle)/(2*M_PI)) )%pics[spr.img].n_imgs ];
}

void rotateSprite(Sprite *spr, double ang){
	spr->angle += ang;
	while(spr->angle > 2*M_PI)
		spr->angle -= 2*M_PI;
}

void setAngSprite(Sprite *spr, double ang){
	spr->angle = ang;
	while(spr->angle > 2*M_PI)
		spr->angle -= 2*M_PI;
}

void desenhaSpriteEm(WINDOW *win, Sprite spr, vet2D pos){
	PutPic(win, getImg(spr), 0, 0, pics[spr.img].width, pics[spr.img].height, ((int)pos.x) - (pics[spr.img].width/2), ((int)pos.y) - (pics[spr.img].height/2));
}

void desenhaFundo_Index(WINDOW *win, int index){
	if(index >= pics[IMG_FUNDO].n_imgs || index < 0)
		throwException("desenhaFundo_Index()", "Não há fundo correspondente ao índice", index_out_of_range_exception);

	PutPic(win, pics[IMG_FUNDO].imgs[index], 0, 0, pics[IMG_FUNDO].width, pics[IMG_FUNDO].height, 0, 0);
}

void desenhaFundo(WINDOW *win){
	desenhaFundo_Index(win, 0);
}

int main(){
	double a, b;
	Sprite spr = {0, 0}; vet2D vet = {0, 256};
	grafInit();
	desenhaSpriteEm(showingWindow, spr, vet);
	while(1){
		scanf("%lf %lf", &a, &b);
		printf("%d", (int) round(a/b));
	}
	
}
