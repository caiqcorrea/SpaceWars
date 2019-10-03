/* BIBLIOTECA QUE INTEGRA A PARTE GRÁFICA COM A PARTE FÍSICA
 *
 */

#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "graficos.h"
#include "../fisica/fisica.h"

extern Sprite SprNaves[MAX_NAVES];
extern Sprite SprPlanetas[MAX_PLANETAS];
extern Sprite SprProjs[MAX_PROJ];
extern Sprite SprBooster[];

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

//Desenha o pleneta p na janela win
void desenhaPlaneta(Planeta p);

//Desenha a nave n na janela win
void desenhaNave(Nave n);

//Desenha o projétil p na janela win
void desenhaProj(Projetil p);

//Desenha todos os planetas na janela win
void desenhaPlanetas();

//Desenha todos as naves na janela win
void desenhaNaves();

//Desenha todos os projéteis na janela win
void desenhaProjs();

//Desenha todos os objetos na janela win
void desenhaTodos();

//Desenha uma explosao na posição p
void desenhaExplosao(vet2D p);

//Desenha um booster na posição p
void desenhaBooster(vet2D p);

//Esta atualiza showingWindow com o conteúdo de workbench, sobrescrevendo o que havia lá
void workbenchFlush();

#endif