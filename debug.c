//MÓDULO CRIADO ESPECIALMENTE PARA DEPURAÇÕES E TESTES
#include "IO.c"

//Imprime as propriedades de uma nave
void imprimeNave(Nave n);

//Imprime as propriedades de um projétil
void imprimeProjetil(Projetil p);

//Imprime as propriedades de um planeta
void imprimePlaneta(Planeta p);

//Imprime todas as naves
void imprimeNaves();

//Imprime todos os projéteis
void imprimeProjeteis();

//Imprime todos os planetas
void imprimePlanetas();

//Imprime todos os tipos de objetos
void imprimeTudo();

int main(int argc, char *argv[])
{
    lerInputs();
    imprimeTudo();
    setbuf(stdin, NULL);

    while (AtualizaJogo())
    {
        imprimeNaves();
        pause();
    }

    printf("dt = %.5lf\n\n", dt);
    printf("Fim\n");
    freeAll();
    return 0;
}

void imprimeNave(Nave n)
{
    fprintf(stdout, "Nave %s: ", n.nome);
    fprintf(stdout, "\tMassa = %.2lf\tPos = (%.2lf , %.2lf)\tVel = (%.2lf , %.2lf)\n",
            n.mass, n.pos.x, n.pos.y, n.vel.x, n.vel.y);
}

void imprimeProjetil(Projetil p)
{
    fprintf(stdout, "Projetil:\tMassa = %.2lf\tPos = (%.2lf , %.2lf)\tVel = (%.2lf , %.2lf)\tRestam = %.2lf\n",
            p.mass, p.pos.x, p.pos.y, p.vel.x, p.vel.y, p.tempoRestante);
}

void imprimePlaneta(Planeta p)
{
    fprintf(stdout, "Planeta:\tMassa = %.2lf\tPos = (%.2lf , %.2lf)\tVel = (%.2lf , %.2lf)\tR = %.2lf\n",
            p.mass, p.pos.x, p.pos.y, p.vel.x, p.vel.y, p.raio);
}

void imprimeNaves()
{
    int i;
    for (i = 0; i < NUM_NAVES; i++)
        imprimeNave(naves[i]);
}

void imprimeProjeteis()
{
    int i;
    for (i = 0; i < tot_projs; i++)
        imprimeProjetil(projs[i]);
}

void imprimePlanetas()
{
    int i;
    for (i = 0; i < NUM_PLANETAS; i++)
        imprimePlaneta(planetas[i]);
}

void imprimeTudo()
{
    imprimePlanetas();
    imprimeNaves();
    imprimeProjeteis();
}