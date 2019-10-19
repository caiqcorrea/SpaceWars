#include "display.h"

#include "graficos.h"
#include "../base/vetores.h"
#include "../fisica/fisica.h"
#include "../base/auxiliar.h"

vet2D converteParaPixel(vet2D pos_em_metros)
{
    mat2D E = {{((double) SIZE_X_WIN)/SIZE_X_FIS, 0}, {0, ((double) SIZE_Y_WIN)/SIZE_Y_FIS}};
    vet2D T = {((double) SIZE_X_WIN)/2, ((double) SIZE_Y_WIN)/2};
    return soma(multiplicaPorMatriz(E, pos_em_metros), T); 
}

void desenhaObjeto(Objeto o){
    desenhaSprite(o.s, converteParaPixel(o.p));
}

void desenhaTodosDoTipo(TipoObj tipo){
    int i;
    for(i=0 ; i<tot_obj[tipo] ; i++)
        desenhaObjeto(*GetObjeto(tipo, i));
}

void desenhaTodos()
{
    int i;
    for(i=0 ; i<NUM_TIPO_OBJ ; i++)
        desenhaTodosDoTipo(i);
}

void desenhaFrameExplosao(int frame, vet2D p)
{
    desenhaSprite(getSpriteFromPic(IMG_EXPLOSAO, frame), p);
}
