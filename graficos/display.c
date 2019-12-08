#include "display.h"

#include "graficos.h"
#include "../base/vetores.h"
#include "../fisica/fisica.h"
#include "../base/auxiliar.h"

#include <math.h>

static vet2D pos_booster_HUD[] =
    {
        {31, 87},
        {SIZE_X_WIN - 31, SIZE_Y_WIN - 87}};

static vet2D pos_nave_HUD[] =
    {
        {31, 23},
        {SIZE_X_WIN - 25, SIZE_Y_WIN - 23}};

static vet2D pos_life_HUD[] =
    {
        {78, 10},
        {SIZE_X_WIN - 78, SIZE_Y_WIN - 10}};

vet2D converteParaPixel(vet2D pos_em_metros)
{
    mat2D E = {{((double)SIZE_X_WIN) / SIZE_X_FIS, 0}, {0, ((double)SIZE_Y_WIN) / SIZE_Y_FIS}};
    vet2D T = {((double)SIZE_X_WIN) / 2, ((double)SIZE_Y_WIN) / 2};
    return soma(multiplicaPorMatriz(E, pos_em_metros), T);
}

/* Note a simplicidade dos códigos que lidam com objetos.
 *  Tudo isso graças ao modo como criamos essa estrutura Objeto
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

void defineRaio(Objeto *o)
{
    int diamentro, h, w;
    h = getPicMsk(o->s.img).height;
    w = getPicMsk(o->s.img).width;
    diamentro = MIN(h, w); /* O cálculo poderia ser de outra forma */
    o->r = diamentro / 3.0;

    /* O raio ainda está em pixels, devemos passar para a unidade da física */
    o->r *= SIZE_X_FIS / SIZE_X_WIN;
}

void defineRaios()
{
    int i, j;

    /* Note como esse código não precisa ser atualizado se adicionarmos
     *  mais tipos de objeto.
     */

    for (i = 0; i < NUM_TIPO_OBJ; i++)
        for (j = 0; j < tot_obj[i]; j++)
            defineRaio(GetObjeto(i, j));
}

void desenhaLifebars()
{
    int life, i;
    vet2D p;
    for (i = 0; i < tot_obj[NAVE]; i++)
    {
        p.y = pos_life_HUD[i].y;
        for (life = naves[i].HP + naves[i].boosterAtual.vidaAdicional; life > 0; life--)
        {
            p.x = pos_life_HUD[i].x + (i ? -1 : 1) * 24 * (life - 1);
            desenhaPicMsk(0, pics[IMG_BAR_AZUL + i], p);
        }
    }
}

void desenhaBoosterHUD()
{
    int i;
    for (i = 0; i < tot_obj[NAVE]; i++)
    {
        naves[i].boosterAtual.pos = pos_booster_HUD[i];
        desenhaSprite(naves[i].boosterAtual.spr, naves[i].boosterAtual.pos);
        desenhaSprite(naves[i].spr, pos_nave_HUD[i]);
    }
}