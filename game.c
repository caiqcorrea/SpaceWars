//MÓDULO CRIADO PARA SER O JOGO

#include "IO/IO.h"
#include "fisica/fisica.h"
#include "graficos/graficos.h"
#include "graficos/display.h"
#include "IO/keybord.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

//Atualiza o estado atual do jogo
//Este método deve:
//	Atualizar as posições de todos os objetos em tela
//	Verificar colisão entre dois quaisquer objetos
//		Se isso acontecer, deve tomar a ação correta dependendo do tipo de objeto que colidiu
//	Verifica se os projéteis ainda estão com tempo de vida positivo
//		Se isso não acontece, deve remover o projétil do array
//
//A função devolve TRUE se o jogo continua e FALSE se o jogo terminou por qualquer motivo
//(Pode ser que mudemos o retorno no futuro)
Bool AtualizaJogo();

//Lê os parâmetros da física de um arquivo
void leFisica();

//Método que faz um teste básico da parte física gravitacional do jogo
//A primeira parte inteira do projeto é testada aqui
void testeFisicaBasica(Bool pause_teste, int usleep_timer);

//É o jogo, faz o jogo jogar
void jogoJogo(Bool pause_mode, int usleep_timer);

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
    int i, j, k;

    Bool debug_mode = FALSE, pause_mode = FALSE;
    int usleep_timer = 1500;
    unsigned int seed = time(NULL);

    for (i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            for (j = 1, k = 0; j < strlen(argv[i]); j++)
                switch (argv[i][j])
                {
                case 'd': //Ativa ou desativa o modo de debug
                    debug_mode = TRUE;
                    break;
                case 'D':
                    debug_mode = FALSE;
                    break;

                case 'p': //Ativa ou desativa pausas entre frames
                    pause_mode = TRUE;
                    break;
                case 'P':
                    pause_mode = FALSE;
                    break;

                case 't': //Modifica o timer do usleep
                    usleep_timer = atoi(argv[i + (++k)]);
                    break;

                case 's': //Modifica a semente do gerador randomico
                    if (strcmp(argv[i + (++k)], "timer"))
                        seed = atoi(argv[i + k]);
                    else
                        seed = time(NULL);
                    break;

                default:
                    throwException("main", "Argumento inválido", file_format_exception);
                    break;
                }
        }
    }
    srand(seed);
    //Mais para frente, será feita uma função de inicialização
    //para que se configure as principais variáveis no início do código

    lerBoosters();

    if (debug_mode)
        testeFisicaBasica(pause_mode, usleep_timer);
    else
        jogoJogo(pause_mode, usleep_timer);

    printf("Fim\n");
    freeAll();
    return 0;
}

Bool AtualizaJogo()
{
    int i;
    Bool todasNavesVivas;

    //Primeiro atualizamos a posição e velocidade de todos os objetos
    AtualizaObjetos();
    //Depois, devemos reduzir o tempo de todos os projéteis
    ReduzTempoProjs();
    //Verificamos se algum projétil sumiu e removemos-o se sim
    for (i = 0; i < tot_obj[PROJETIL]; i++)
        if (VerificaSeProjMorreu(projs[i])) //Se o projétil morreu
            RemoveProj(i);                  //Removemos o projétil de índice i

    //Fazemos todas as atualizações relacionadas com boosters
    AtualizaBoosters();

    //Atualizamos o tempo de recarga das naves
    AtualizaCooldown();

    ChecaTodasColisoes();
    todasNavesVivas = TodasEstaoVivas();

    //E a simulação continua enquanto o tempo for positivo e não há naves mortas
    return (todasNavesVivas);
}

void leFisica()
{
    string nomeArq;

    tot_obj[BOOSTER] = 0;
    tot_obj[PROJETIL] = 0;

    //Pedimos um arquivo e abrimos ele
    nomeArq = mallocSafe(sizeof(*nomeArq) * 200);
    printf("Digite o nome do seu arquivo: ");
    scanf("%s", nomeArq);

    lerInputs(nomeArq); //Lemos os inputs (a função fecha o arquivo para nós)
    freeSafe(nomeArq);

    defineRaios();
}

void testeFisicaBasica(Bool pause_teste, int usleep_timer)
{
    double tempo = 0;

    grafInit();

    leFisica();    //Lemos os parâmetros da fisica
    imprimeTudo(); //Jogamos tudo o que foi lido na tela

    //Enquanto a simulação não terminar...
    while (AtualizaJogo() == TRUE)
    {
        giraObjetoVelTipo(NAVE);
        giraObjetoVelTipo(PROJETIL);

        printf("Tempo: %.3lf\n", tempo);
        tempo += dt;

        //Imprimimos cada coisa
        imprimeNaves();
        imprimeProjeteis();
        imprimeBoosters();
        imprimePlanetas();
        printf("\n\n");

        desenhaFundoWorkbench();
        desenhaTodos();
        workbenchFlush();
        usleep(usleep_timer);

        if (pause_teste)
            pause_aux();
    }

    freeAll();
}

void jogoJogo(Bool pause_mode, int usleep_timer)
{
    grafInit();
    leFisica();
    initKeybord(mainWindow());

    while (AtualizaJogo() == TRUE)
    {
        atualizaKeybord(mainWindow());

        if (isPressed(NAVE1_HOR))
            Rotaciona(&naves[0], TRUE);
        if (isPressed(NAVE1_ANT))
            Rotaciona(&naves[0], FALSE);
        if (isPressed(NAVE1_ACE))
            Acelera(&naves[0]);
        if (isPressed(NAVE1_DIS))
            Atira(&naves[0]);

        if (isPressed(NAVE2_HOR))
            Rotaciona(&naves[1], TRUE);
        if (isPressed(NAVE2_ANT))
            Rotaciona(&naves[1], FALSE);
        if (isPressed(NAVE2_ACE))
            Acelera(&naves[1]);
        if (isPressed(NAVE2_DIS))
            Atira(&naves[1]);

        giraObjetoVelTipo(PROJETIL);

        desenhaFundoWorkbench();
        desenhaTodos();
        workbenchFlush();
        usleep(usleep_timer);

        if (pause_mode)
            pause_aux();
    }
}

void imprimeNave(Nave n)
{
    fprintf(stdout, "Nave %s: "
                    "\tMassa = %.3lf"
                    "\tPos = (%.4lf , %.4lf)"
                    "\tVel = (%.4lf , %.4lf)"
                    "\tBooster = %s\n",
            n.nome,
            n.mass, n.pos.x, n.pos.y,
            n.vel.x, n.vel.y, n.boosterAtual.nome);
}

void imprimeProjetil(Projetil p)
{
    fprintf(stdout, "Projetil:"
                    "\tMassa = %3.2lf"
                    "\tPos = (%3.2lf , %3.2lf)"
                    "\tVel = (%3.2lf , %3.2lf)"
                    "\tRestam = %3.2lf\n",
            p.mass, p.pos.x, p.pos.y,
            p.vel.x, p.vel.y, p.tempoRestante);
}

void imprimePlaneta(Planeta p)
{
    fprintf(stdout, "Planeta:"
                    "\tMassa = %3.2lf"
                    "\tPos = (%3.2lf , %3.2lf)"
                    "\tVel = (%3.2lf , %3.2lf)"
                    "\tR = %3.2lf\n",
            p.mass, p.pos.x, p.pos.y,
            p.vel.x, p.vel.y, p.radius);
}

void imprimeBooster(Booster b)
{
    fprintf(stdout, "%s: \n"
                    "\tvida = %d"
                    "\tcadencia = %.3lf"
                    "\tdano = %d"
                    "\ttempoEmNave = %.3lf"
                    "\ttempoEmTela = %.3lf\n"
                    "\ttempoVidaProj = %.3lf"
                    "\tmassProj = %.3lf"
                    "\tposProj = (%.3lf, %.3lf)"
                    "\tvelProj = (%.3lf, %.3lf)\n"
                    "\tpos = (%.3lf, %.3lf)"
                    "\tvel = (%.3lf, %.3lf)\n",
            b.nome,
            b.vidaAdicional,
            b.proj.cadencia,
            b.proj.dano, b.tempoRestanteNave,
            b.tempoRestanteTela,
            b.proj.tempoRestante, b.proj.mass,
            b.proj.pos.x, b.proj.pos.y,
            b.proj.vel.x, b.proj.vel.y,
            b.pos.x, b.pos.y,
            b.vel.x, b.vel.y);
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