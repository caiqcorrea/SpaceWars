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
void lerInputs(string arq);

//Função que lê o planeta e inicializa-o
void lerPlaneta(int index);

//Função que lê uma nave e inicializa-a
void lerNave(int index);

/* FUNÇÕES PARA BOOSTERS */
//Método que lê todos os booster do arquivo booster.cfg
//e define os tipos de boosters possíveis no jogo
void lerBoosters();

//Método deve ser chamado após o aparecimento de um [ e
//lê booster.cfg até achar um ]
//Ela recebe um índice que se refere a onde o booster sendo atualmente criado
//será colocado no aray de boosters possíveis
void lerBooster(int index);

#endif
