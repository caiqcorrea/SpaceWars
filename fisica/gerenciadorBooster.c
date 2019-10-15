/* Este é o arquivo que implementa as funções definidas em gerenciadorBooster.h
 */

#include "gerenciadorBooster.h"
#include "../base/auxiliar.h"
#include "string.h"
#include "fisica.h"
#include "../base/vetores.h"

#include <stdlib.h>

Booster *boostersPreCriados;

int totalBoostersPreCriados;

vet2D maxVel, minVel;
double maxMass, minMass;
double maxTempoRestanteTela, minTempoRestanteTela;
double maxTempoRestanteNave, minTempoRestanteNave;
double propBooster;

static void boosterRandom(Booster *b);

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
}

void removeBoosterDaTela(int index)
{
    //Apenas removemos o booster do array. Assim ele para de ser exibido na tela
    for (; index < tot_obj[BOOSTER] - 1; index++)
        boosters[index] = boosters[index + 1];
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
    viraBoosterPadrao(&(nave->boosterAtual));
}

void viraBoosterPadrao(Booster *b)
{
    defineBoosterComo(b, BoosterPadrao);
}

void defineBoosterComo(Booster *b, Booster ref)
{
    int i;

    b->nome = mallocSafe(sizeof(char) * strlen(ref.nome));
    for (i = 0; (b->nome[i] = ref.nome[i]) != '\0'; i++)
        ; //Garante que o '\0' estará no b sem ter que fazer um caso separado
    b->cadencia = ref.cadencia;
    b->vidaAdicional = ref.vidaAdicional;
    b->proj.tempoRestante = ref.proj.tempoRestante;
    b->proj.dano = ref.proj.dano;
    b->proj.pos = ref.proj.pos;
    b->proj.vel = ref.proj.vel;
    b->proj.mass = ref.proj.mass;
    b->proj.radius = ref.proj.radius;
    b->mass = ref.mass;
    b->radius = ref.radius;
    b->pos = ref.pos;
    b->vel = ref.vel;
    b->tempoRestanteNave = ref.tempoRestanteNave;
    b->tempoRestanteTela = ref.tempoRestanteTela;
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
    if(boosterVaiSpawnar())
        criaNovoBooster();
}