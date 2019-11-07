#include "display.h"

#include "graficos.h"
#include "../base/vetores.h"
#include "../fisica/fisica.h"
#include "../base/auxiliar.h"

#include <math.h>

vet2D converteParaPixel(vet2D pos_em_metros)
{
    mat2D E = {{((double)SIZE_X_WIN) / SIZE_X_FIS, 0}, {0, ((double)SIZE_Y_WIN) / SIZE_Y_FIS}};
    vet2D T = {((double)SIZE_X_WIN) / 2, ((double)SIZE_Y_WIN) / 2};
    return soma(multiplicaPorMatriz(E, pos_em_metros), T);
}

/* Note a simplicidade dos códigos que lidam com objetos.
   Tudo isso graças ao modo como criamos essa estrutura Objeto
 */

void desenhaObjeto(Objeto o)
{
    desenhaSprite(o.s, converteParaPixel(o.p));
}

void desenhaTodosDoTipo(TipoObj tipo)
{
    int i;
    for (i = 0; i < tot_obj[tipo]; i++)
        desenhaObjeto(*GetObjeto(tipo, i));
}

void desenhaTodos()
{
    int i;
    for (i = 0; i < NUM_TIPO_OBJ; i++)
        desenhaTodosDoTipo(i);
}

void desenhaFrameExplosao(int frame, vet2D p)
{
    desenhaSprite(getSpriteFromPic(IMG_EXPLOSAO, frame), p);
}

void defineRaio(Objeto *o)
{
    int diamentro, h, w;
    h = pics[o->s.img].height;
    w = pics[o->s.img].width;
    diamentro = MIN(h, w); /* O cálculo poderia ser de outra forma */ 
    o->r = diamentro / 2.0;
    
    /* O raio ainda está em pixels, devemos passar para a unidade da física */
    o->r *= SIZE_X_FIS / SIZE_X_WIN;
}

void defineRaios()
{
    int i, j;

    /* Note como esse código não precisa ser atualizado se adicionarmos
       mais tipos de objeto 
     */

    for(i = 0; i < NUM_TIPO_OBJ; i++)
        for(j = 0; j < tot_obj[i]; j++)
           defineRaio(GetObjeto(i, j));
}