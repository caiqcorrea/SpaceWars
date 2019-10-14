/* Este arquivo foi criado para ser um auxilidor com os boosters
 * Ele deve conter algumas variáveis globais, além de vários métodos
 * para criação, destruição e modificação de boosters ao longo do jogo.
 * 
 * ATENÇÃO: a definição do struct Booster está em fisica.h, por ser mais coerente
 * (afinal, ele é um objeto como Nave, Projétil e Planeta).
 * Esta biblioteca apenas é um gerenciador para este objeto, por ele ter muitas
 * peculiaridades próprias.
 */

#ifndef _BOOSTER_MANAGER_H
#define _BOOSTER_MANAGER_H

#include "fisica.h"

/* VARIÁVEIS GLOBAIS */

//Array com todos os boosters lidos de booster.cfg
//Ele é alocado dinamicamente, pois não sabemos quantos são os boosters lidos lá
//Na hora de criar um booster na tela, pegamos um randômico desse array
//O booster padrão deve sempre estar na posição 0 desse array e ter o nome = PADRAO
//Se, ao final da leitura de booster.cfg, não tivermos lido um booster padrão,
//devemos lançar uma exeção ao usuário.
extern Booster *boostersPreCriados;

//Número de elementos do array acima
extern int totalBoostersPreCriados;

#define BoosterPadrao boostersPreCriados[0]

//Na hora de criar aleatoriamente um booster para ir para a tela, ele terá algumas propriedades
//aleatórias que vamos decidir com base nas variáveis que seguem:
extern vet2D maxVel, minVel;
extern double maxMass, minMass;
extern double maxTempoRestanteTela, minTempoRestanteTela;
extern double maxTempoRestanteNave, minTempoRestanteNave;

/* FUNÇÕES */

//Inicializa e coloca um novo booster no array de boosters
void criaNovoBooster();

//Função que tira um booster do array caso seu tempo de tela tenha esgotado
void mataBooster(int index);

//Função chamada quando um booster é capturado por uma nave
void capturaBooster(int index, Nave *nave);

//Função que é chamada quando o tempo do booster capturado por uma nave se esgotou
//Seu nome é reseta booster porque a nave possui um booster padrão
//Esta função sobrescreve o booster com o qual a nave estava pelo booster padrão
void resetaBooster(Nave *nave);

//Função que, dado um booster b, transforma ele no booster padrão
void viraBoosterPadrao(Booster *b);

//Atribui ao primeiro booster as qualidades do segundo (a referência)
void defineBoosterComo(Booster *b, Booster ref);

#endif