//MÓDULO CRIADO ESPECIALMENTE PARA DEPURAÇÕES E TESTES
#include "IO.c"

int main(){
    lerInputs();

    printf("Propriedades da Terra:\n");
    printf("\tMassa = %lf\n\tRaio = %lf\n\tPosição = %lf %lf\n\tVelocidade = %lf %lf\n",
        TERRA.mass, TERRA.raio, TERRA.pos.x, TERRA.pos.y, TERRA.vel.x, TERRA.vel.y);
    printf("\nPropriedades da Nave %s\n", naves[0].nome);
    printf("\tMassa = %lf\n\tPosição = %lf %lf\n\tVelocidade = %lf %lf\n",
        naves[0].mass, naves[0].pos.x, naves[0].pos.y, naves[0].vel.x, naves[0].vel.y);
    printf("\nPropriedades da Nave %s\n", naves[1].nome);
    printf("\tMassa = %lf\n\tPosição = %lf %lf\n\tVelocidade = %lf %lf\n",
        naves[1].mass, naves[1].pos.x, naves[1].pos.y, naves[1].vel.x, naves[1].vel.y);
    printf("\n");
    printf("dt = %lf\n\n", dt);
    printf("Fim\n");

    return 0;
}