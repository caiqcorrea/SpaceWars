//MÓDULO CRIADO ESPECIALMENTE PARA DEPURAÇÕES E TESTES
#include "IO.c"
#include "fisica.c"


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

	char input[20];
	int passos;
	char pausa;
	int i;
	int j;
	int k;

	printf("Definicao das opcoes do usuario: \n");
	printf("Deseja realizar quantos passos de simulacao? Digite um número inteiro: \n");
	fgets(input, 20, stdin);
	passos = atoi(input);
	printf("Defina o intervalo de tempo dt entre cada passo: \n");
	fgets(input, 20, stdin);
	dt = atof(input);
	printf("Deseja pausar a cada iteracao? Digite s para sim, qualquer outra tecla para nao: \n");
	fgets(input, 20, stdin);
	pausa = input[0];

	for (i = 0; i < passos; i++) {

		for (j = 0; j < NUM_PLANETAS; j++) {

			printf("O planeta de nome TERRA esta na posicao (%lf, %lf)\n", planetas[0].o.p.x, planetas[0].o.p.y)
		}
		for (j = 0; j < NUM_NAVES; j++) {
			
			printf("A nave de nome %s esta na posicao (%lf, %lf) com velocidade (%lf,%lf).\n", naves[j].nome, naves[j].o.p.x, naves[j].o.p.y, naves[j].o.v.x, naves[j].o.v.y);

		}
		for (j = 0; j < tot_projs; j++) {

			printf("O projetil de numero %d esta na posicao (%lf, %lf) com velocidade (%lf,%lf) e tempo restante %lf.\n", j, projs[j].o.p.x, projs[j].o.p.y, projs[j].o.v.x, projs[j].o.v.y, projs[j].tempoRestante);

		}
		AtualizaProjeteis();
		AtualizaPosicoes();

	}


    return 0;
}