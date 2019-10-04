/* A biblioteca IO deve conter tudo relacionado a saída e entrada
 * de dados ao programa que não seja das entradas e saída padrão.
 * 
 * Utiliza-se esta biblioteca para leitura ou escrita de arquivos.
 * Esta biblioteca não é genérica. Suas funções são todas feitas
 * com objetivo de ser parte do jogo Space Wars.
 */

#ifndef _IO_H
#define _IO_H

#include "../fisica/fisica.h"

#include <stdio.h>

/* C O N S T A N T E S */

#define BOOSTERCFG_PATH "booster.cfg"

/* F U N Ç Õ E S */

/* FUNÇÕES DE INICIALIZAÇÃO DA PARTE FÍSICA */
//Função que lê todos os inputs de um arquivo informado pelo usuário
// e atribui valores às variáveis globais
void lerInputs(FILE *arq);

//Função que lê o planeta e inicializa-o
void lerTerra(FILE *arq);

//Função que lê uma nave e inicializa-a
void lerNave(FILE *arq, Nave *n);

//Função que lê um projétil e inicializa-o
void lerProjetil(FILE *arq, Projetil *p, double tempoDeVida);

/* FUNÇÕES PARA BOOSTERS */
void leitura();

#endif