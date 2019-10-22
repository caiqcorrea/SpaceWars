/* Este é o arquivo header para o analisador léxico.
 *
 * 
 */

#include "../base/leitor.h"

typedef enum {
    //Variáveis globais
    PROP_BOOSTER,
    MAX_VEL,
    MIN_VEL,
    MAX_MASS,
    MIN_MASS,
    MAX_TEMPO_RESTANTE_TELA,
    MIN_TEMPO_RESTANTE_TELA,
    MAX_TEMPO_RESTANTE_NAVE,
    MIN_TEMPO_RESTANTE_NAVE,
    TOTAL_BOOSTERS,
    DT,
    

    //Início de objetos
    BOOSTER,
    NAVE,
    PLANETA,
    PROJETIL,

    //Variáveis de boosters
    NOME_BOOSTER,
    VIDA_ADICIONAL,
    CADENCIA,
    DANO_BOOSTER,
    TEMPO_PROJ,
    MASS_PROJ,
    SPRITE_BOOSTER,
    SPRITE_PROJ,

    //Variáveis de naves
    NOME_NAVE,
    MASS_NAVE,
    VEL_NAVE,
    POS_NAVE,
    SPRITE_NAVE,

    //Variáveis de planetas
    RAIO_PLANETA,
    MASS_PLANETA,
    POS_PLANETA,
    VEL_PLANETA,

    //Identificadores
    ABRE_BRAC,
    FECHA_BRAC,
    IGUAL
} TOKEN;

TOKEN leToken(){
    
}
