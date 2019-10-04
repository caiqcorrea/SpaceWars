/* BIBLIOTECA FÍSICA PARA O JOGO SPACE WARS
 *
 * Esta biblioteca foi criada especificamente para o jogo Space Wars,
 * Sua base são as três structs: Nave, Projétil e Planeta, que devem
 * se encontrar instanciadas dentro dos arrays: naves, projs e planetas.
 * 
 * Decidimos implementar uma struct mais básica chamada de Objeto, que existirá
 * dentro de cada uma das structs mais importantes. Ela possuirá inicialmente
 * os atributos: massa, posição e velocidade, mas a ideia é que todos os
 * atributos compartilhados pelos três structs estejam dentro de Objeto (como,
 * por exemplo, o raio).
 * Dessa forma, podemos criar funções genéricas que recebem apenas Objetos ao
 * invés de três (ou mais) funções que fazem a mesma coisa, mas uma para cada
 * tipo de struct.
 * 
 * Todos os calculos feitos são baseados na Teoria da Gravitação Universal de
 * Isaac Newton.
 * https://en.wikipedia.org/wiki/Newton%27s_law_of_universal_gravitation
 * É sabido que determinar posições exatas de orbitas complexas (como as do jogo) 
 * é um problema em aberto na matemática e na física, então o professor nos
 * recomendou usar o método numérico de Runge-Kutta.
 * 
 * Convenções de nomenclatura:
 *		Array: Variáveis do tipo v[MAX]
 *		Vetor: Estrutura vet2D, representa um vetor em R²
 */

#include "fisica.h"

#include "../base/vetores.h"
#include "../base/auxiliar.h"

#include <math.h>
#include <stddef.h>

/*--------------- V A R I Á V E I S   G L O B A I S ---------------*/

int tot_obj[NUM_TIPO_OBJ] = MAX_OBJ; //O número de objetos de cada tipo

/* Armazenaremos todos os objetos na tela através de arrays globais,
 * um para cada tipo: Nave, Planeta e Projetil.
 * Os tamanhos desses arrays estão definidos no começo desse arquivo e podem ser
 * editados hard-coded (quem sabe futuramente em tempo de execução)
 */
Nave naves[MAX_NAVES];			//O array que contém as duas naves dos jogadores
Planeta planetas[MAX_PLANETAS]; //O array que contém o planeta central
Projetil projs[MAX_PROJ];		//O array que contém os projéteis que estão atualmente na tela
Booster boosters[MAX_BOOSTERS]; //O array que contém os boosters que estão atualmente na tela

#define TERRA planetas[0] //Como só há um planeta, vamos chamá-lo de TERRA

//Intervalo de tempo da simulacao, lido no arquivo principal. Por padrão, é 0.001
double dt;

//Tempo restante de simulação;
double tRestante;

/*--------------- I M P L E M E N T A Ç Ã O   D A S   F U N Ç Õ E S ---------------*/

Objeto *GetObjeto(TipoObj tipo, int indice)
{
	if (!(0 <= indice && indice < tot_obj[tipo]))
		throwException("GetObjeto", "indexOutOfRangeException", index_out_of_range_exception);

	else
		switch (tipo)
		{
		case PLANETA:
			return &(planetas[indice].o);

		case NAVE:
			return &(naves[indice].o);

		case PROJETIL:
			return &(projs[indice].o);

		case BOOSTER:
			return &(boosters[indice].o);

		default:
			throwException("GetObjeto", "tipo nao identificado", var_type_undefined_exception);
		}

	return NULL; //Só para o compilador não reclamar
}
void SetObjeto(TipoObj tipo, int indice, Objeto o)
{
	if (!(0 <= indice && indice < tot_obj[tipo]))
		throwException("SetObjeto", "indexOutOfRangeException", index_out_of_range_exception);

	else
		switch (tipo)
		{
		case PLANETA:
			planetas[indice].o = o;
			break;

		case NAVE:
			naves[indice].o = o;
			break;

		case PROJETIL:
			projs[indice].o = o;
			break;

		case BOOSTER:
			boosters[indice].o = o;
			break;

		default:
			throwException("SetObjeto", "tipo nao identificado", var_type_undefined_exception);
		}
}

vet2D Forca(Objeto o1, Objeto o2)
{
	vet2D aux = sub(o1.p, o2.p);
	if (norma(aux) == 0)
		return NULL_VET;

	return mult(-G * o1.m * o2.m / pow(norma(aux), 2), versor(aux));
}

void IncVel(vet2D F, Objeto *o)
{
	o->v = soma(o->v, mult((dt / (o->m)), F));
}

void IncPos(Objeto *o)
{
	o->p = soma(o->p, mult(dt, o->v));

	//Casos em que o objeto sai da tela: trazemos ele de volta do lado oposto
	while (o->p.x > SIZE_X_FIS / 2)
		o->p.x -= SIZE_X_FIS;

	while (o->p.x < -SIZE_X_FIS / 2)
		o->p.x += SIZE_X_FIS;

	while (o->p.y > SIZE_Y_FIS / 2)
		o->p.y -= SIZE_Y_FIS;

	while (o->p.y < -SIZE_Y_FIS / 2)
		o->p.y += SIZE_Y_FIS;
}

vet2D CalculaForcaSobre(Objeto o)
{
	vet2D F = NULL_VET;
	int i;
	TipoObj tipo;

	for (tipo = 0; tipo < NUM_TIPO_OBJ; tipo++)
		for (i = 0; i < tot_obj[tipo]; i++)
			F = soma(F, Forca(o, *GetObjeto(tipo, i)));

	return F;
}

double DistanciaEntre(Objeto o1, Objeto o2)
{
	return norma(sub(o1.p, o2.p));
}

void AtualizaObjeto(Objeto *o)
{
	IncVel(CalculaForcaSobre(*o), o);
	IncPos(o);
}

void AtualizaObjetos()
{
	int i;
	TipoObj tipo;
	//Planetas não precisam ser atualizados (pelo menos na versão atual)

	for (tipo = 0; tipo < NUM_TIPO_OBJ; tipo++)
		if (tipo != PLANETA)
			for (i = 0; i < tot_obj[tipo]; i++)
				AtualizaObjeto(GetObjeto(tipo, i));
}

void ReduzTempoProj(Projetil *p)
{
	p->tempoRestante -= dt;
}

/* Essa função foi criada para facilitar a edição de código caso queiramos, no futuro,
 * mudar a forma de reduzir o tempo dos projéteis (como reduzir apenas de alguns e de outros não)
 */
void ReduzTempoProjs()
{
	int i;
	for (i = 0; i < tot_obj[PROJETIL]; i++)
		ReduzTempoProj(&(projs[i]));
}

Bool VerificaSeProjMorreu(Projetil p)
{
	return p.tempoRestante <= 0;
}

void RemoveProj(int index)
{
	int i;
	for (i = index; i < tot_obj[PROJETIL]; i++)
		projs[i] = projs[i + 1];
	tot_obj[PROJETIL]--;
}

Bool ChecaColisaoEntre(Objeto o1, Objeto o2)
{
	return (DistanciaEntre(o1, o2) < o1.r + o2.r);
}

void ChecaTodasColisoes()
{
	int i, j;
	Bool colidiu;
	//PROJÉTEIS
	for (i = 0; i < tot_obj[PROJETIL]; i++)
	{
		colidiu = FALSE;

		//com PLANETAS
		for (j = 0; !colidiu && j < tot_obj[PLANETA]; j++)
		{
			if (ChecaColisaoEntre(*GetObjeto(PROJETIL, i), *GetObjeto(PLANETA, j)))
			{
				RemoveProj(i);
				colidiu = TRUE;
			}
		}
		//com outro PROJÉTIL
		for (j = i + 1; !colidiu && j < tot_obj[PROJETIL]; j++)
		{
			//Note que começamos de i + 1 pois assumimos todos os i-1 projéteis anteriores não colidiram com ninguém
			//e i não colide consigo mesmo.
			//Conclusão: se i colide com um j, então j > i
			if (ChecaColisaoEntre(*GetObjeto(PROJETIL, i), *GetObjeto(PROJETIL, j)))
			{
				RemoveProj(j); //Como j > i, removemos ele primeiro. Assim o i-ésimo projétil do array continua sendo aquele
				RemoveProj(i); //que colidiu. Se fosse o contrário (remover i primeiro) seria o (j-1)-ésimo quem colidiu com o i-ésimo
				colidiu = TRUE;
			}
		}
		//com NAVES
		for (j = 0; !colidiu && j < tot_obj[NAVE]; j++)
		{
			if (ChecaColisaoEntre(*GetObjeto(PROJETIL, i), *GetObjeto(NAVE, j)))
			{
				RemoveProj(i);
				DecrementaVida(&(naves[j]), projs[i].dano);
				colidiu = TRUE;
			}
		}

		if (colidiu)
			i--; //Removemos o i-ésimo, então o próximo a ser visto é quem virou o i-ésimo
				//Note que se o projétil colide com um planeta (a primeira verificação) a função não entra nos outros for's
	}
	//NAVES
	for (i = 0; i < tot_obj[NAVE]; i++)
	{
		colidiu = FALSE;
		//com PLANETAS
		for (j = 0; !colidiu && j < tot_obj[PLANETA]; j++)
		{
			if (ChecaColisaoEntre(*GetObjeto(NAVE, i), *GetObjeto(PLANETA, j)))
			{
				printf("Nave %d colidindo com o planeta\n", i);
				printf("Planeta:\n");
				printf("raio = %f pos = (%f, %f) vel = (%f, %f) mass = %f\n", planetas[j].radius,
				planetas[j].pos.x, planetas[j].pos.y, planetas[j].vel.x, planetas[j].vel.y, planetas[j].mass);
				Destroi(&(naves[i]));
				colidiu = TRUE;
			}
		}
		//com outras NAVES
		for (j = i + 1; !colidiu && j < tot_obj[NAVE]; j++)
		{
			//Começamos de i+1 pois as i-1 primeiras naves não colidiram com outras naves e naves[i] não colide com si própria
			if (ChecaColisaoEntre(*GetObjeto(NAVE, i), *GetObjeto(NAVE, j)))
			{
				Destroi(&(naves[j]));
				Destroi(&(naves[i]));
				colidiu = TRUE;
			}
		}
		if (colidiu)
			break; //Se colidiu, paramos por aqui. O jogo terminou.
	}
}

void DecrementaVida(Nave *n, int valor)
{
	n->HP -= valor;
}

void IncrementaVida(Nave *n, int valor)
{
	n->HP += valor;
}

Bool EstaViva(Nave n)
{
	return (n.HP > 0); //Se 0, está morta
}

Bool TodasEstaoVivas()
{
	int i;
	for (i = 0; i < tot_obj[NAVE]; i++)
		if (!EstaViva(naves[i]))
			return FALSE;
	return TRUE;
}

void Destroi(Nave *n)
{
	printf("Destruindo a nave: %s\n", n->nome);
	printf("Ela está em: (%f, %f) com velocidade (%f, %f)\n", n->pos.x, n->pos.y, n->vel.x, n->vel.y);
	DecrementaVida(n, n->HP); //Faz a vida zerar
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
			RemoveProj(i);					//Removemos o projétil de índice i

	ChecaTodasColisoes();
	todasNavesVivas = TodasEstaoVivas();

	printf("Tempo Restante = %f\n", tRestante);
	tRestante -= dt; //Decrementamos o tempo restante de simulação
	printf("Naves vivas = %d", todasNavesVivas);

	//E a simulação continua enquanto o tempo for positivo e não há naves mortas
	return ((tRestante > 0.0) && todasNavesVivas);
}

void freeAll()
{
	int i;
	for (i = 0; i < tot_obj[NAVE]; i++)
		freeSafe(naves[i].nome);
}