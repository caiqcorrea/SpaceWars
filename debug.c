//MÓDULO CRIADO ESPECIALMENTE PARA DEPURAÇÕES E TESTES

#ifndef _DEBUG
#define _DEBUG

#include "IO/IO.h"
#include "fisica/fisica.h"
#include "graficos/graficos.h"
#include "graficos/display.h"
#include "fisica/gerenciadorBooster.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


//Método que faz um teste básico da parte física gravitacional do jogo
//A primeira parte inteira do projeto é testada aqui
void testeFisicaBasica();

//Lê e devolve um arquivo aberto
void lerArquivo(FILE **f);

//Imprime as propriedades de uma nave
void imprimeNave(Nave n);

//Imprime as propriedades de um projétil
void imprimeProjetil(Projetil p);

//Imprime as propriedades de um planeta
void imprimePlaneta(Planeta p);

//Imprime as propriedade de um booster
void imprimeBooster(Booster b);

//Imprime todas as naves
void imprimeNaves();

//Imprime todos os projéteis
void imprimeProjeteis();

//Imprime todos os planetas
void imprimePlanetas();

//Imprime todos os booster
void imprimeBoosters();

//Imprime todos os boostersPreCriados
void imprimeBoosterPreCriados();

//Imprime todos os tipos de objetos
void imprimeTudo();

//Método chamado ao fim do programa para desalocar tudo de todas as bibliotecas
void freeAll();

int main(int argc, char *argv[])
{
    tot_obj[BOOSTER] = 0;
    tot_obj[PROJETIL] = 0;
    tRestante = 1e7;
    srand(time(NULL));
    //Mais para frente, será feita uma função de inicialização
    //para que se configure as principais variáveis no início do código

    lerBoosters();

    testeFisicaBasica();

    printf("Fim\n");
    freeAll();

    return 0;
}

void testeFisicaBasica()
{
    string nomeArq;
    //double tempo = 0;
    //Variáveis para o teste de colisão com boosters
    //Bool umaVez = FALSE;
    //Bool outraVez = FALSE;

    //Pedimos um arquivo e abrimos ele
    nomeArq = mallocSafe(sizeof(*nomeArq) * 200);
    printf("Digite o nome do seu arquivo: ");
    scanf("%s", nomeArq);

    lerInputs(nomeArq); //Lemos os inputs (a função fecha o arquivo para nós)
    freeSafe(nomeArq);

    //imprimeTudo();       //e jogamos tudo o que foi lido na tela
    setbuf(stdin, NULL); //(apenas para evitar possíveis erros)

    grafInit();

    //Enquanto a simulação não terminar...
    while (AtualizaJogo() == TRUE)
    {
        //printf("Tempo: %.3lf\n", tempo);
        //tempo += dt;

        /*if (tempo > 1 && tot_obj[BOOSTER] > 0 && !umaVez)
        {
            boosters[0].pos.x = naves[0].pos.x + 1000;
            boosters[0].pos.y = naves[0].pos.y;
            boosters[0].vel.x = -100;
            boosters[0].vel.y = 0;
            umaVez = TRUE;
        }

        if (strcmp(naves[0].boosterAtual.nome, "PADRAO") != 0 && !outraVez)
        {
            umaVez = FALSE;
            outraVez = TRUE;
        }*/

        //Imprimimos as naves, os projéteis e os boosters
        //imprimeNaves();
        //imprimeProjeteis();
        //imprimeBoosters();
        //Imprimir o planeta toda hora é desnecessário, mas caso queira, apenas tire o //
        //imprimePlanetas();
        //printf("\n\n");

        desenhaFundoWorkbench();
        desenhaTodos();
        workbenchFlush();
        usleep(1500);
        //E pausamos até o usuário digitar ENTER
        //pause();
    }
    
    grafFree();
}

void imprimeNave(Nave n)
{
    fprintf(stdout, "Nave %s: ", n.nome);
    fprintf(stdout, "\tMassa = %.3lf\tPos = (%.4lf , %.4lf)\tVel = (%.4lf , %.4lf)\tBooster = %s\n",
            n.mass, n.pos.x, n.pos.y, n.vel.x, n.vel.y, n.boosterAtual.nome);
}

void imprimeProjetil(Projetil p)
{
    fprintf(stdout, "Projetil:\tMassa = %3.2lf\tPos = (%3.2lf , %3.2lf)\tVel = (%3.2lf , %3.2lf)\tRestam = %3.2lf\n",
            p.mass, p.pos.x, p.pos.y, p.vel.x, p.vel.y, p.tempoRestante);
}

void imprimePlaneta(Planeta p)
{
    fprintf(stdout, "Planeta: \tMassa = %3.2lf\tPos = (%3.2lf , %3.2lf)\tVel = (%3.2lf , %3.2lf)\tR = %3.2lf\n",
            p.mass, p.pos.x, p.pos.y, p.vel.x, p.vel.y, p.radius);
}

void imprimeBooster(Booster b)
{
    fprintf(stdout, "%s: \n", b.nome);
    fprintf(stdout, "\tvida = %d\tcadencia = %d\tdano = %d\ttempoEmNave = %.3lf\ttempoEmTela = %.3lf\n",
            b.vidaAdicional, b.cadencia, b.proj.dano, b.tempoRestanteNave, b.tempoRestanteTela);
    fprintf(stdout, "\ttempoVidaProj = %.3lf\tmassProj = %.3lf\tposProj = (%.3lf, %.3lf)\tvelProj = (%.3lf, %.3lf)\n",
            b.proj.tempoRestante, b.proj.mass, b.proj.pos.x, b.proj.pos.y, b.proj.vel.x, b.proj.vel.y);
    fprintf(stdout, "\tpos = (%.3lf, %.3lf)\tvel = (%.3lf, %.3lf)\n",
            b.pos.x, b.pos.y, b.vel.x, b.vel.y);
}

void imprimeNaves()
{
    int i;
    for (i = 0; i < tot_obj[NAVE]; i++)
        imprimeNave(naves[i]);
}

void imprimeProjeteis()
{
    int i;
    for (i = 0; i < tot_obj[PROJETIL]; i++)
        imprimeProjetil(projs[i]);
}

void imprimePlanetas()
{
    int i;
    for (i = 0; i < tot_obj[PLANETA]; i++)
        imprimePlaneta(planetas[i]);
}

void imprimeBoosters()
{
    int i;
    for (i = 0; i < tot_obj[BOOSTER]; i++)
        imprimeBooster(boosters[i]);
}

void imprimeBoosterPreCriados()
{
    int i;
    for (i = 0; i < totalBoostersPreCriados; i++)
        imprimeBooster(boostersPreCriados[i]);
}

void imprimeTudo()
{
    imprimePlanetas();
    imprimeNaves();
    imprimeProjeteis();
    imprimeBoosters();
}

void freeAll()
{
    freeFisica();
    grafFree();
}
#endif
