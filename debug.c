//MÓDULO CRIADO ESPECIALMENTE PARA DEPURAÇÕES E TESTES

#include "../IO.h"
#include "fisica.h"

#include<stdio.h>

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
	FILE *arq;
	string nomeArq;
	double tempo = 0;
	
    //Pedimos um arquivo e abrimos ele
	nomeArq = mallocSafe(sizeof(string) * 50);
	printf("Digite o nome do seu arquivo: ");
	scanf("%s", nomeArq);
	arq = fopen(nomeArq, "r");
	freeSafe(nomeArq);

	if (arq == NULL) //Verificando se o usuário deu o nome correto
		throwException("lerInputs", "Não foi possivel abrir o arquivo.", file_not_find_exception);

    lerInputs(arq); //Lemos os inputs (a função fecha o arquivo para nós)
    imprimeTudo(); //e jogamos tudo o que foi lido na tela
    setbuf(stdin, NULL); //(apenas para evitar possíveis erros)

    //Enquanto a simulação não terminar

    while (AtualizaJogo())
    {
    	printf("Tempo: %.3lf\n", tempo);
    	tempo+= dt;
        //Imprimimos as naves e os projéteis
        imprimeNaves();
        imprimeProjeteis();
        //Imprimir o planeta toda hora é desnecessário, mas caso queira, apenas tire o //
        //imprimePlanetas();
        printf("\n\n");
       // pause(); //E pausamos até o usuário digitar ENTER
    }

    printf("Fim\n");
    freeAll();
    return 0;
}

void imprimeNave(Nave n)
{
    fprintf(stdout, "Nave %s: ", n.nome);
    fprintf(stdout, "\tMassa = %3.2lf\tPos = (%3.2lf , %3.2lf)\tVel = (%3.2lf , %3.2lf)\n",
            n.mass, n.pos.x, n.pos.y, n.vel.x, n.vel.y);
}

void imprimeProjetil(Projetil p)
{
    fprintf(stdout, "Projetil:\tMassa = %3.2lf\tPos = (%3.2lf , %3.2lf)\tVel = (%3.2lf , %3.2lf)\tRestam = %3.2lf\n",
            p.mass, p.pos.x, p.pos.y, p.vel.x, p.vel.y, p.tempoRestante);
}

void imprimePlaneta(Planeta p)
{
    fprintf(stdout, "Planeta:\tMassa = %3.2lf\tPos = (%3.2lf , %3.2lf)\tVel = (%3.2lf , %3.2lf)\tR = %3.2lf\n",
            p.mass, p.pos.x, p.pos.y, p.vel.x, p.vel.y, p.raio);
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

void imprimeTudo()
{
    imprimePlanetas();
    imprimeNaves();
    imprimeProjeteis();
}
