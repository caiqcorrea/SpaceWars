/* Este arquivo foi criado para ser um auxilidor com os boosters
 * Ele deve conter algumas variáveis globais, além de vários métodos
 * para criação, destruição e modificação de boosters ao longo do jogo.
 * 
 * ATENÇÃO: Este módulo apenas é um gerenciador os boosters,
 * por eles terem muitas peculiaridades próprias. Ele é um módulo da biblioteca
 * física.h
 * 
 * ATENÇÃO: para usar a maioria dos métodos desse módulo é preciso, primeiro,
 * ler o arquivo booster.cfg e atribuir valores às variáveis e arrays desse cabeçalho.
 */

#include "../base/auxiliar.h"
#include "../base/vetores.h"
#include "fisica.h"

#include <string.h>
#include <stdlib.h>

Booster *boostersPreCriados;

int totalBoostersPreCriados;

vet2D maxVel, minVel;
double maxMass, minMass;
double maxTempoRestanteTela, minTempoRestanteTela;
double maxTempoRestanteNave, minTempoRestanteNave;
double propBooster;

static void boosterRandom(Booster *b);

void defineBoosterPadrao()
{
    BoosterPadrao()->nome = mallocSafe(sizeof(char) * TAM_MAX_NOMES);
	strcpy(BoosterPadrao()->nome, "PADRÃO");
    BoosterPadrao()->vidaAdicional = 0;
    BoosterPadrao()->proj.cadencia = 10;
    BoosterPadrao()->proj.dano = 1;
    BoosterPadrao()->proj.tempoRestante = 1000;
    BoosterPadrao()->proj.mass = 100;
    BoosterPadrao()->proj.radius = 10;
    BoosterPadrao()->proj.spr.img = IMG_PROJ_PADRAO;
    BoosterPadrao()->proj.spr.angle = 0;
    BoosterPadrao()->aceleracao = 7e-2;
    BoosterPadrao()->proj.vInicial = 50;
    //As variáveis abaixo são indiferentes para o booster padrão
    BoosterPadrao()->mass = 0;
    BoosterPadrao()->pos.x = 0;
    BoosterPadrao()->pos.y = 0;
    BoosterPadrao()->vel.x = 0;
    BoosterPadrao()->vel.y = 0;
    BoosterPadrao()->radius = 0;
    BoosterPadrao()->spr.img = 0;
    BoosterPadrao()->spr.angle = 0;
    BoosterPadrao()->proj.pos.x = 0;
    BoosterPadrao()->proj.pos.y = 0;
    BoosterPadrao()->proj.vel.x = 0;
    BoosterPadrao()->proj.vel.y = 0;
    BoosterPadrao()->tempoRestanteTela = 0;
}

void criaNovoBooster()
{
    if (tot_obj[BOOSTER] == MAX_BOOSTERS)
        return; //Apenas retornamos, pois isso não é um erro

    Booster b;
    boosterRandom(&b);
    boosters[tot_obj[BOOSTER]++] = b; //Colocamos na posição atual e atualizamos o máximo
}

static void boosterRandom(Booster *b)
{
    if (totalBoostersPreCriados == 0)
        throwException("BoosterRandom",
                       "Se está tendendo criar um booster sem haver nenhum booster pré-criado",
                       null_pointer_exception);

    int i;
    do
    {
        i = rand() % totalBoostersPreCriados; //0 <= i < totalBoostersPreCriados
    } while (i == 0);                         //Garantindo que 0 < i

    //Aqui temos que 0 < i < totalBoosterPreCriados (pois boostersPreCriados[0] é o booster padrão)
    defineBoosterComo(b, boostersPreCriados[i]);

    //Cada booster que nasce, tem sua posição, velocidade e tempos restantes randomizados
    //Só não pode nascer colidindo com algo (especialmente com uma nave)
    b->mass = geraRandomicoEntre(minMass, maxMass);
	do
	{
		b->pos.x = geraRandomicoEntre(-SIZE_X_FIS, SIZE_X_FIS);
		b->pos.y = geraRandomicoEntre(-SIZE_Y_FIS, SIZE_Y_FIS);
	} while (ChecaColisaoComTodos(b->o));
	b->vel.x = geraRandomicoEntre(minVel.x, maxVel.x);
	b->vel.y = geraRandomicoEntre(minVel.y, minVel.y);
	b->tempoRestanteNave = geraRandomicoEntre(minTempoRestanteNave, maxTempoRestanteNave);
	b->tempoRestanteTela = geraRandomicoEntre(minTempoRestanteTela, maxTempoRestanteTela);

}

void removeBoosterDaTela(int index)
{
    int i;
    //Apenas removemos o booster do array. Assim ele para de ser exibido na tela
    for (i = index; i < tot_obj[BOOSTER] - 1; i++)
        boosters[i] = boosters[i + 1];
    tot_obj[BOOSTER]--;
}

void capturaBooster(int index, Nave *nave)
{
    if (index > tot_obj[BOOSTER])
        throwException("capturaBooster",
                       "Um índice de booster foi passado incorretamente.",
                       index_out_of_range_exception);

    nave->boosterAtual = boosters[index];
    removeBoosterDaTela(index); //Depois retiramos ele da tela
}

void resetaBooster(Nave *nave)
{
    getBoosterPadrao(&(nave->boosterAtual));
}

void getBoosterPadrao(Booster *b)
{
    defineBoosterComo(b, *BoosterPadrao());
}

void defineBoosterComo(Booster *b, Booster ref)
{
    //Assumimos que b ainda não possui um nome mallocado!
    b->nome = mallocSafe(sizeof(char) * TAM_MAX_NOMES);
	strcpy(b->nome, ref.nome);
    b->proj.cadencia = ref.proj.cadencia;
    b->vidaAdicional = ref.vidaAdicional;
    b->proj.vInicial = ref.proj.vInicial;
    b->proj.tempoRestante = ref.proj.tempoRestante;
    b->proj.dano = ref.proj.dano;
    b->proj.pos = ref.proj.pos;
    b->proj.vel = ref.proj.vel;
    b->proj.mass = ref.proj.mass;
    b->proj.radius = ref.proj.radius;
    b->aceleracao = ref.aceleracao;
    b->mass = ref.mass;
    b->radius = ref.radius;
    b->pos = ref.pos;
    b->vel = ref.vel;
    b->tempoRestanteNave = ref.tempoRestanteNave;
    b->tempoRestanteTela = ref.tempoRestanteTela;
    b->spr.angle = ref.spr.angle;
    b->spr.img = ref.spr.img;
}

Bool boosterVaiSpawnar()
{
    double r = ((double)rand()) / RAND_MAX; // 0 <= r <= 1
    return (r <= propBooster);
}

void ChecaColisaoComBoosters()
{
    int i, j;
    for (i = 0; i < tot_obj[NAVE]; i++)
        for (j = 0; j < tot_obj[BOOSTER]; j++)
            if (ChecaColisaoEntre(*GetObjeto(NAVE, i), *GetObjeto(BOOSTER, j)))
                capturaBooster(j, &(naves[i]));
}

void AtualizaBoostersEmNaves()
{
    int i;

    for (i = 0; i < tot_obj[NAVE]; i++)
        if (strcmp(naves[i].boosterAtual.nome, "PADRAO") != 0) //Se não é o booster padrão
        {
            naves[i].boosterAtual.tempoRestanteNave -= dt;
            if (naves[i].boosterAtual.tempoRestanteNave <= 0) //Se o tempo de vida do booster estourou
                resetaBooster(&(naves[i]));                   //Resetamos o booster da nave
        }
}

void AtualizaBoostersEmTela()
{
    int i;
    for (i = 0; i < tot_obj[BOOSTER]; i++)
    {
        boosters[i].tempoRestanteTela -= dt;
        if (boosters[i].tempoRestanteTela <= 0)
            removeBoosterDaTela(i);
    }
}

void AtualizaBoosters()
{
    ChecaColisaoComBoosters();
    AtualizaBoostersEmNaves();
    AtualizaBoostersEmTela();
    if (boosterVaiSpawnar())
        criaNovoBooster();
}

Booster *BoosterPadrao()
{
    return &(boostersPreCriados[0]);
}