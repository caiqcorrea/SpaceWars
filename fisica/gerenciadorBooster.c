/* Este é o arquivo que implementa as funções definidas em gerenciadorBooster.h
 */

#include "gerenciadorBooster.h"
#include "../base/auxiliar.h"
#include "string.h"
#include "fisica.h"
#include "../base/vetores.h"

Booster *boostersPreCriados;

int totalBoostersPreCriados;

vet2D maxVel, minVel;
double maxMass, minMass;
double maxTempoRestanteTela, minTempoRestanteTela;
double maxTempoRestanteNave, minTempoRestanteNave;

void criaNovoBooster()
{
    throwException("criaNovoBooster", "Não implementada", function_not_implemented_exception);
}

void mataBooster(int index)
{
    throwException("mataBooster", "Não implementada", function_not_implemented_exception);
}

void capturaBooster(int index, Nave *nave)
{
    throwException("capturaBooster", "Não implementada", function_not_implemented_exception);
}

void resetaBooster(Nave *nave)
{
    throwException("resetaBooster", "Não implementada", function_not_implemented_exception);
}

void viraBoosterPadrao(Booster *b)
{
    throwException("viraBoosterPadrao", "Não implementada", function_not_implemented_exception);
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
