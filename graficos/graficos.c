#include "graficos.h"

#include "../base/auxiliar.h"
#include "../base/vetores.h"
#include "../graficos/xwc.h"

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

WINDOW *workbench;
WINDOW *showingWindow;

struct Pics pics[NUM_SPR];

void winsInit(){
	showingWindow = InitGraph(SIZE_X_WIN, SIZE_Y_WIN, "showingWIndow");
	workbench = NewPic(showingWindow, SIZE_X_WIN, SIZE_Y_WIN);
}

void picsInit(){
	int i, j, sizex_aux, sizey_aux, n_imgs;
	char filename[MAX_NOME_SPR_FILE];
	FILE* spr_file;
	for(i=0 ; i<1 ; i++){
		snprintf(filename, sizeof(filename), "pics/%d/size.txt", i);
		spr_file = fopen(filename, "r");
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
			pics[i].imgs[j] = ReadPic(showingWindow, filename, NULL);
		}
	}
}

void grafInit(){
	winsInit();
	picsInit();
}

PIC getImg(Sprite spr){
	int index;
	index = (int) round((pics[spr.img].n_imgs*spr.angle)/(2*M_PI))%pics[spr.img].n_imgs;
	return pics[spr.img].imgs[index];
}

void rotateSprite(Sprite *spr, double ang){
	spr->angle += ang;
}

void desenhaSpriteEm(WINDOW *win, Sprite spr, vet2D pos){
	PutPic(win, getImg(spr), 0, 0, pics[0].width, pics[0].height, ((int)pos.x) - (pics[spr.img].width/2), ((int)pos.y) - (pics[spr.img].height/2));
}

int main(){
	double a, b;
	Sprite spr = {0, 0}; vet2D vet = {0, 256};
	winsInit();
	picsInit();
	desenhaSpriteEm(showingWindow, spr, vet);
	while(1){
		scanf("%lf %lf", &a, &b);
		printf("%d", (int) round(a/b));
	}
	
}
