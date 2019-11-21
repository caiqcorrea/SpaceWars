/* BIBLIOTECA QUE INTEGRA A PARTE GRÁFICA COM A PARTE FÍSICA
 *
 */

#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "../base/vetores.h"
#include "../fisica/fisica.h"

#define MAX_EXPLOSOES 20

//Função que, dada uma posição utilizada pela biblioteca física, converte para
//uma posição para ser utilizada pela biblioteca gráfica.
//Por exemplo, o ponto (0,0) da física é (width/2 , height/2) da tela
//Esta função deve fazer essa conversãoS
vet2D converteParaPixel(vet2D pos_em_metros);

//Desenha um objeto
void desenhaObjeto(Objeto o);

//Desenha todos os objetos de um tipo
void desenhaTodosDoTipo(TipoObj tipo);

//Desenha todos os objetos
void desenhaTodos();

//Dado um objeto, a função olha sua sprite para definir o raio do objeto
void defineRaio(Objeto *o);

//Define todos os raios de todos os objetos
void defineRaios();

#endif